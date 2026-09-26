#include "priop.hpp"

#include "desempate.hpp"

Processo *PRIOp::selecionarProximo(std::vector<Processo *> &fila_prontos,
                                   Processo *atual, int /*tempo_atual*/) {
  std::vector<Processo *> candidatos = fila_prontos;
  if (atual != nullptr) {
    candidatos.push_back(atual);
  }

  if (candidatos.empty()) {
    return nullptr;
  }

  auto empatados =
      filtrar_por_extremo(candidatos, &Processo::prioridade, Extremo::Maximo);
  return desempatar(empatados, atual);
}