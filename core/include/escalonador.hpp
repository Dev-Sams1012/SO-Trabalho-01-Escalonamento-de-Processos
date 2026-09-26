#pragma once

#include "processo.hpp"

#include <string_view>
#include <vector>

class IEscalonador {
public:
  virtual ~IEscalonador() = default;

  virtual Processo *selecionarProximo(std::vector<Processo *> &fila_prontos,
                                      Processo *atual, int tempo_atual) = 0;

  virtual void onTick(std::vector<Processo *> & /*fila_prontos*/,
                      int /*tempo_atual*/) {}

  virtual std::string_view nome() const = 0;
};
