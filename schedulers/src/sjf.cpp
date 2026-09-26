#include "sjf.hpp"

#include <algorithm>

Processo *SJF::selecionarProximo(std::vector<Processo *> &fila_prontos,
                                 Processo *atual, int tempo_atual) {
  if (atual != nullptr) {
    return atual;
  }

  if (fila_prontos.empty()) {
    return nullptr;
  }

  return std::ranges::min(fila_prontos, {}, &Processo::duracao);
}