#include "prioc.hpp"

#include "desempate.hpp"

Processo *PRIOc::selecionarProximo(std::vector<Processo *> &fila_prontos,
                                   Processo *atual, int /*tempo_atual*/) {
  // A prioridade cooperativa nao interrompe o processo atual.
  if (atual != nullptr) {
    return atual;
  }

  if (fila_prontos.empty()) {
    return nullptr;
  }

  // Prioridade maior tem preferencia.
  auto empatados =
      filtrar_por_extremo(fila_prontos, &Processo::prioridade, Extremo::Maximo);
  return desempatar(empatados, atual);
}