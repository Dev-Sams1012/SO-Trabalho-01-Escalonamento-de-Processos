#pragma once

#include <string>
#include <vector>

#include "processo.hpp"

class IEscalonador {
public:
  virtual ~IEscalonador() = default;

  virtual Processo *selecionarProximo(std::vector<Processo *> &fila_prontos,
                                      Processo *atual, int tempo_atual) = 0;

  virtual void onTick(std::vector<Processo *> & /*fila_prontos*/,
                      int /*tempo_atual*/) {}

  virtual std::string nome() const = 0;
};
