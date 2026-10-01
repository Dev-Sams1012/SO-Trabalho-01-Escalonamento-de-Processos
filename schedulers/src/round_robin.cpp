#include "round_robin.hpp"

Processo *RoundRobin::selecionarProximo(std::vector<Processo *> &fila_prontos,
                                        Processo *atual, int /*tempo_atual*/) {
  // Quando a CPU fica livre, inicia pelo primeiro processo da fila.
  if (atual == nullptr) {
    if (fila_prontos.empty()) {
      processo_monitorado_ = nullptr;
      return nullptr;
    }
    Processo *proximo = fila_prontos.front();
    fila_prontos.erase(fila_prontos.begin());
    processo_monitorado_ = proximo;
    ticks_no_slice_ = 0;
    return proximo;
  }

  if (atual != processo_monitorado_) {
    processo_monitorado_ = atual;
    ticks_no_slice_ = 0;
  }

  ticks_no_slice_++;

  if (ticks_no_slice_ < quantum_) {
    return atual;
  }

  // Ao fim do quantum, cede a CPU se houver outro processo pronto.
  if (fila_prontos.empty()) {
    ticks_no_slice_ = 0;
    return atual;
  }

  Processo *proximo = fila_prontos.front();
  fila_prontos.erase(fila_prontos.begin());
  processo_monitorado_ = proximo;
  ticks_no_slice_ = 0;
  return proximo;
}