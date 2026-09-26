#pragma once

#include "escalonador.hpp"

class FCFS : public IEscalonador {
public:
  Processo *selecionarProximo(std::vector<Processo *> &fila_prontos,
                              Processo *atual, int tempo_atual) override;

  std::string_view nome() const override {
    return "FCFS (First Come, First Served)";
  };
};