#include "srtf.hpp"

#include <algorithm>

Processo *SRTF::selecionarProximo(std::vector<Processo *> &fila_prontos,
                                  Processo *atual, int tempo_atual) {
  if (fila_prontos.empty()) {
    return atual;
  }

  Processo *melhor_fila =
      std::ranges::min(fila_prontos, {}, &Processo::tempo_restante);

  if (atual == nullptr) {
    return melhor_fila;
  }

  if (melhor_fila->tempo_restante < atual->tempo_restante) {
    return melhor_fila;
  }

  return atual;
}