#include "priop.hpp"

#include <algorithm>

Processo *PRIOp::selecionarProximo(std::vector<Processo *> &fila_prontos,
                                   Processo *atual, int tempo_atual) {
  if (fila_prontos.empty()) {
    return atual;
  }

  Processo *maior_prioridade =
      std::ranges::max(fila_prontos, {}, &Processo::prioridade);

  if (atual == nullptr) {
    return maior_prioridade;
  }

  if (maior_prioridade->prioridade > atual->prioridade) {
    return maior_prioridade;
  } 

  return atual;
}