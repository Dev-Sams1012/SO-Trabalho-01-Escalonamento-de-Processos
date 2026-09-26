#pragma once

#include "escalonador.hpp"

class PRIOc : public IEscalonador {
  Processo *selecionarProximo(std::vector<Processo *> &fila_prontos,
                              Processo *atual, int tempo_atual) override;

  std::string_view nome() const override {
    return "PRIOc (Prioridade Cooperativo)";
  };
};