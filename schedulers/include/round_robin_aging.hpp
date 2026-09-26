#pragma once

#include "escalonador.hpp"
#include "processo.hpp"

class RoundRobinAging : public IEscalonador {
public:
  RoundRobinAging(int quantum, int aging) : quantum_(quantum), aging_(aging) {}

  Processo *selecionarProximo(std::vector<Processo *> &fila_prontos,
                              Processo *atual, int tempo_atual) override;

  void onTick(std::vector<Processo *> &fila_prontos, int tempo_atual) override;

  std::string_view nome() const override {
    return "Round-Robin com prioridade e envelhecimento";
  }

private:
  int quantum_;
  int aging_;

  int ticks_no_slice_ = 0;
  int ticks_desde_ultimo_envelhecimento_ = 0;
  Processo *processo_monitorado_ = nullptr;

  Processo *escolher_por_prioridade(std::vector<Processo *> &fila_prontos);
};