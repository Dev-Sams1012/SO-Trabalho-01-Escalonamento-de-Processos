#pragma once

#include "processo.hpp"

#include <string_view>
#include <vector>

// Define as operacoes comuns aos algoritmos de escalonamento.
class IEscalonador {
public:
  virtual ~IEscalonador() = default;

  // Retorna o processo que deve usar a CPU neste instante.
  virtual Processo *selecionarProximo(std::vector<Processo *> &fila_prontos,
                                      Processo *atual, int tempo_atual) = 0;

  // Permite atualizar regras do algoritmo a cada instante.
  virtual void onTick(std::vector<Processo *> & /*fila_prontos*/,
                      int /*tempo_atual*/) {}

  virtual std::string_view nome() const = 0;
};
