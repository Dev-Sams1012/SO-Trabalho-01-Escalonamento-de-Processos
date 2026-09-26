#include "round_robin_aging.hpp"

#include "desempate.hpp"

#include <algorithm>

Processo *RoundRobinAging::escolher_por_prioridade(
    std::vector<Processo *> &fila_prontos) {
  auto empatados = filtrar_por_extremo(
      fila_prontos, &Processo::prioridade_dinamica, Extremo::Maximo);

  Processo *escolhido = desempatar(empatados, nullptr);

  fila_prontos.erase(
      std::remove(fila_prontos.begin(), fila_prontos.end(), escolhido),
      fila_prontos.end());

  return escolhido;
}

Processo *
RoundRobinAging::selecionarProximo(std::vector<Processo *> &fila_prontos,
                                   Processo *atual, int /*tempo_atual*/) {
  if (atual == nullptr) {
    if (fila_prontos.empty()) {
      processo_monitorado_ = nullptr;
      return nullptr;
    }
    Processo *proximo = escolher_por_prioridade(fila_prontos);
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

  if (fila_prontos.empty()) {
    ticks_no_slice_ = 0;
    return atual;
  }

  Processo *proximo = escolher_por_prioridade(fila_prontos);
  processo_monitorado_ = proximo;
  ticks_no_slice_ = 0;
  return proximo;
}

void RoundRobinAging::onTick(std::vector<Processo *> &fila_prontos,
                             int /*tempo_atual*/) {
  ticks_desde_ultimo_envelhecimento_++;

  if (ticks_desde_ultimo_envelhecimento_ < quantum_) {
    return;
  }

  ticks_desde_ultimo_envelhecimento_ = 0;

  for (Processo *p : fila_prontos) {
    p->prioridade_dinamica += aging_;
  }
}