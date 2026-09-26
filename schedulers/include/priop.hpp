#pragma once

#include "escalonador.hpp"

class PRIOp : public IEscalonador {
public:
  Processo *selecionarProximo(std::vector<Processo *> &fila_prontos,
                              Processo *atual, int tempo_atual) override;

  std::string_view nome() const override {
    return "PRIOp (Prioridade Preemptivo)";
  };
};