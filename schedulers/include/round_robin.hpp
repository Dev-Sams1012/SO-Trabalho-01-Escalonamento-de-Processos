#pragma once

#include "escalonador.hpp"

class RoundRobin : public IEscalonador {
public:
  explicit RoundRobin(int quantum) : quantum_(quantum) {}

  Processo *selecionarProximo(std::vector<Processo *> &fila_prontos,
                              Processo *atual, int tempo_atual) override;

  std::string_view nome() const override { return "Round-Robin"; }

private:
  int quantum_;

  int ticks_no_slice_ = 0;
  Processo *processo_monitorado_ = nullptr;
};