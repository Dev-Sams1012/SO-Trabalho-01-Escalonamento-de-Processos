#include "sjf.hpp"

#include "desempate.hpp"

Processo *SJF::selecionarProximo(std::vector<Processo *> &fila_prontos,
                                 Processo *atual, int /*tempo_atual*/) {
  // SJF eh cooperativo: deixa o processo atual terminar.
  if (atual != nullptr) {
    return atual;
  }

  if (fila_prontos.empty()) {
    return nullptr;
  }

  // Entre os prontos, escolhe o processo de menor duracao total.
  auto empatados =
      filtrar_por_extremo(fila_prontos, &Processo::duracao, Extremo::Minimo);
  return desempatar(empatados, atual);
}