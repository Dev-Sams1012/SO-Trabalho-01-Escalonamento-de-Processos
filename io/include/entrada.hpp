#pragma once

// Leitura dos dados de entrada do simulador:
//  - processos: uma linha por processo, "chegada duracao prioridade"
//  - config:    linhas "quantum:2" e "aging:1"
//
// Estas funcoes sao usadas tanto pelo CLI quanto pela GUI.

#include <iosfwd>
#include <string>
#include <vector>

#include "config.hpp"
#include "processo.hpp"

// Resultado de uma leitura: ou deu certo, ou traz uma mensagem de erro
// (com o numero da linha) para mostrar ao usuario.
struct ErroLeitura {
  int linha = 0;        // 1-based; 0 = erro geral (ex.: arquivo nao abriu)
  std::string mensagem;
};

struct LeituraProcessos {
  std::vector<Processo> processos;
  std::vector<ErroLeitura> erros;
  bool ok() const { return erros.empty(); }
};

struct LeituraConfig {
  Config config{2, 1};  // valores padrao se o arquivo nao definir
  std::vector<ErroLeitura> erros;
  bool ok() const { return erros.empty(); }
};

// Le processos de qualquer stream (stdin, arquivo, string...).
// Linhas em branco e linhas iniciadas por '#' sao ignoradas.
// Os ids sao atribuidos na ordem de leitura: P1, P2, ...
LeituraProcessos ler_processos(std::istream &in);
LeituraProcessos ler_processos_arquivo(const std::string &caminho);

// Le o arquivo de configuracao "chave:valor".
LeituraConfig ler_config(std::istream &in);
LeituraConfig ler_config_arquivo(const std::string &caminho);

// Cria um Processo com todos os campos derivados inicializados
// (tempo_restante, prioridade_dinamica). Usado pelo parser e pela GUI.
Processo criar_processo(int id, int chegada, int duracao, int prioridade);