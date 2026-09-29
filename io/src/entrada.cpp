#include "entrada.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>

namespace {

std::string aparar(const std::string &s) {
  auto ini = std::find_if_not(s.begin(), s.end(),
                              [](unsigned char c) { return std::isspace(c); });
  auto fim = std::find_if_not(s.rbegin(), s.rend(), [](unsigned char c) {
               return std::isspace(c);
             }).base();
  return (ini < fim) ? std::string(ini, fim) : std::string();
}

// Le um inteiro da string inteira (rejeita "12abc").
bool para_inteiro(const std::string &s, int &saida) {
  if (s.empty()) {
    return false;
  }
  try {
    std::size_t usados = 0;
    const int v = std::stoi(s, &usados);
    if (usados != s.size()) {
      return false;
    }
    saida = v;
    return true;
  } catch (...) {
    return false;
  }
}

}  // namespace

Processo criar_processo(int id, int chegada, int duracao, int prioridade) {
  Processo p{};
  p.id = id;
  p.tempo_chegada = chegada;
  p.duracao = duracao;
  p.tempo_restante = duracao;
  p.prioridade = prioridade;
  p.prioridade_dinamica = prioridade;
  return p;
}

LeituraProcessos ler_processos(std::istream &in) {
  LeituraProcessos res;
  std::string linha;
  int numero_linha = 0;
  int proximo_id = 1;

  while (std::getline(in, linha)) {
    ++numero_linha;
    const std::string limpa = aparar(linha);
    if (limpa.empty() || limpa[0] == '#') {
      continue;
    }

    std::istringstream campos(limpa);
    std::string a, b, c, sobra;
    campos >> a >> b >> c;
    const bool tem_sobra = static_cast<bool>(campos >> sobra);

    int chegada = 0, duracao = 0, prioridade = 0;
    if (c.empty()) {
      res.erros.push_back(
          {numero_linha, "esperado 3 inteiros: chegada duracao prioridade"});
    } else if (tem_sobra) {
      res.erros.push_back({numero_linha, "campos demais (esperado 3)"});
    } else if (!para_inteiro(a, chegada) || !para_inteiro(b, duracao) ||
               !para_inteiro(c, prioridade)) {
      res.erros.push_back({numero_linha, "valor nao e um inteiro valido"});
    } else if (chegada < 0) {
      res.erros.push_back({numero_linha, "instante de criacao nao pode ser negativo"});
    } else if (duracao <= 0) {
      res.erros.push_back({numero_linha, "duracao deve ser maior que zero"});
    } else if (prioridade <= 0) {
      res.erros.push_back({numero_linha, "prioridade deve ser positiva (> 0)"});
    } else {
      res.processos.push_back(
          criar_processo(proximo_id++, chegada, duracao, prioridade));
    }
  }
  return res;
}

LeituraProcessos ler_processos_arquivo(const std::string &caminho) {
  std::ifstream arq(caminho);
  if (!arq) {
    LeituraProcessos res;
    res.erros.push_back({0, "nao foi possivel abrir o arquivo: " + caminho});
    return res;
  }
  return ler_processos(arq);
}

LeituraConfig ler_config(std::istream &in) {
  LeituraConfig res;
  std::string linha;
  int numero_linha = 0;

  while (std::getline(in, linha)) {
    ++numero_linha;
    const std::string limpa = aparar(linha);
    if (limpa.empty() || limpa[0] == '#') {
      continue;
    }

    const auto dois_pontos = limpa.find(':');
    if (dois_pontos == std::string::npos) {
      res.erros.push_back({numero_linha, "formato esperado: chave:valor"});
      continue;
    }

    std::string chave = aparar(limpa.substr(0, dois_pontos));
    std::transform(chave.begin(), chave.end(), chave.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    const std::string valor_txt = aparar(limpa.substr(dois_pontos + 1));

    int valor = 0;
    if (!para_inteiro(valor_txt, valor)) {
      res.erros.push_back({numero_linha, "valor nao e um inteiro valido"});
      continue;
    }

    if (chave == "quantum") {
      if (valor <= 0) {
        res.erros.push_back({numero_linha, "quantum deve ser maior que zero"});
      } else {
        res.config.quantum = valor;
      }
    } else if (chave == "aging") {
      if (valor < 0) {
        res.erros.push_back({numero_linha, "aging nao pode ser negativo"});
      } else {
        res.config.aging = valor;
      }
    } else {
      res.erros.push_back({numero_linha, "chave desconhecida: " + chave});
    }
  }
  return res;
}

LeituraConfig ler_config_arquivo(const std::string &caminho) {
  std::ifstream arq(caminho);
  if (!arq) {
    LeituraConfig res;
    res.erros.push_back({0, "nao foi possivel abrir o arquivo: " + caminho});
    return res;
  }
  return ler_config(arq);
}