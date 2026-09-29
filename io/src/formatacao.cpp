#include "formatacao.hpp"

#include <cstdio>
#include <sstream>

namespace {

enum class Celula { Vazio, Pronto, Executando };

Celula celula_no_tick(const Processo &p, int tempo, int pid_execucao) {
  if (pid_execucao == p.id) {
    return Celula::Executando;
  }
  if (tempo >= p.tempo_chegada && tempo < p.tempo_conclusao) {
    return Celula::Pronto;
  }
  return Celula::Vazio;
}

}  // namespace

std::string formatar_diagrama(const ResultadoSimulacao &resultado) {
  std::ostringstream out;

  out << "tempo ";
  for (const auto &p : resultado.processos) {
    out << " P" << p.id << (p.id < 10 ? " " : "");
  }
  out << '\n';

  char buf[32];
  for (const auto &reg : resultado.linha_tempo) {
    std::snprintf(buf, sizeof(buf), "%2d-%2d ", reg.tempo, reg.tempo + 1);
    out << buf;

    for (const auto &p : resultado.processos) {
      switch (celula_no_tick(p, reg.tempo, reg.pid_execucao)) {
      case Celula::Executando:
        out << " ##" << (p.id < 10 ? " " : "");
        break;
      case Celula::Pronto:
        out << " --" << (p.id < 10 ? " " : "");
        break;
      case Celula::Vazio:
        out << "   " << (p.id < 10 ? " " : "");
        break;
      }
    }
    out << '\n';
  }
  return out.str();
}

std::string formatar_resumo(const ResultadoSimulacao &resultado) {
  char buf[128];
  std::ostringstream out;

  std::snprintf(buf, sizeof(buf), "tempo medio de vida (tt): %.2f\n",
                resultado.tempo_retorno_medio);
  out << buf;
  std::snprintf(buf, sizeof(buf), "tempo medio de espera (tw): %.2f\n",
                resultado.tempo_espera_medio);
  out << buf;
  out << "trocas de contexto: " << resultado.trocas_contexto << '\n';
  return out.str();
}

std::string formatar_relatorio(const std::string &nome_algoritmo,
                               const ResultadoSimulacao &resultado) {
  std::ostringstream out;
  out << "=== " << nome_algoritmo << " ===\n";
  out << formatar_resumo(resultado);
  out << '\n' << formatar_diagrama(resultado);
  return out.str();
}