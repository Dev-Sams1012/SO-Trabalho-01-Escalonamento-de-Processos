#include "prioc.hpp"

#include "desempate.hpp"

Processo *PRIOc::selecionarProximo(std::vector<Processo *> &fila_prontos,
                                   Processo *atual, int /*tempo_atual*/) {
  if (atual != nullptr) {
    return atual;
  }

  if (fila_prontos.empty()) {
    return nullptr;
  }

  auto empatados =
      filtrar_por_extremo(fila_prontos, &Processo::prioridade, Extremo::Maximo);
  return desempatar(empatados, atual);
}