#include "prioc.hpp"

#include <algorithm>

Processo *PRIOc::selecionarProximo(std::vector<Processo *> &fila_prontos,
                                   Processo *atual, int tempo_atual) {
  if (atual != nullptr) {
    return atual;
  }

  if (fila_prontos.empty()) {
    return nullptr;
  }

  return std::ranges::max(fila_prontos, {}, &Processo::prioridade);
}