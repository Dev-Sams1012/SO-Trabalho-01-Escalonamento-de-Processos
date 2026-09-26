#pragma once

#include "escalonador.hpp"

class SJF : public IEscalonador {
  Processo *selecionarProximo(std::vector<Processo *> &fila_prontos,
                              Processo *atual, int tempo_atual) override;

  std::string_view nome() const override { return "SJF (Shortest Job Firs)"; };
};