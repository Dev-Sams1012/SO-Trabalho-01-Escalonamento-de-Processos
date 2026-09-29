#include "carregamento.hpp"

#include "entrada.hpp"
#include "tinyfiledialogs.h"

namespace {

// Converte a lista de ErroLeitura em strings prontas para exibir,
// prefixadas com o nome do arquivo de origem.
std::vector<std::string> formatar_erros(const std::string &origem,
                                        const std::vector<ErroLeitura> &erros) {
  std::vector<std::string> saida;
  for (const auto &e : erros) {
    std::string linha = origem;
    if (e.linha > 0) {
      linha += ":" + std::to_string(e.linha);
    }
    linha += ": " + e.mensagem;
    saida.push_back(linha);
  }
  return saida;
}

}  // namespace

void carregar_processos_de_arquivo(EstadoApp &estado) {
  // Filtro do dialogo: so mostra .txt por padrao, mas permite "todos os
  // arquivos" tambem, ja que o formato e texto simples sem extensao fixa.
  const char *filtros[] = {"*.txt"};
  const char *caminho = tinyfd_openFileDialog(
      /*titulo=*/"Carregar processos",
      /*caminho_padrao=*/"",
      /*num_filtros=*/1, filtros,
      /*descricao_filtro=*/"Arquivos de texto (*.txt)",
      /*permitir_multiplos=*/0);

  if (caminho == nullptr) {
    // Usuario cancelou o dialogo; nao e um erro, apenas nao faz nada.
    return;
  }

  LeituraProcessos lido = ler_processos_arquivo(caminho);
  estado.erros_carregamento = formatar_erros(caminho, lido.erros);

  if (lido.ok() && !lido.processos.empty()) {
    estado.processos = std::move(lido.processos);
    // Continua a contagem de ids a partir do maior id carregado, para
    // que processos adicionados manualmente depois nao colidam.
    estado.proximo_id = 1;
    for (const auto &p : estado.processos) {
      if (p.id >= estado.proximo_id) {
        estado.proximo_id = p.id + 1;
      }
    }
  } else if (lido.ok() && lido.processos.empty()) {
    estado.erros_carregamento.push_back(
        "arquivo nao contem nenhum processo valido");
  }
}

void carregar_config_de_arquivo(EstadoApp &estado) {
  const char *filtros[] = {"*.txt"};
  const char *caminho = tinyfd_openFileDialog(
      "Carregar configuracao", "", 1, filtros,
      "Arquivos de texto (*.txt)", 0);

  if (caminho == nullptr) {
    return;
  }

  LeituraConfig lido = ler_config_arquivo(caminho);
  estado.erros_carregamento = formatar_erros(caminho, lido.erros);

  if (lido.ok()) {
    estado.config = lido.config;
  }
}