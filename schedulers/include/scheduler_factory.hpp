#pragma once

#include <array>
#include <memory>

#include "config.hpp"
#include "escalonador.hpp"

enum class TipoEscalonador {
  FCFS,
  SJF,
  SRTF,
  PrioridadeCooperativa,
  PrioridadePreemptiva,
  RoundRobin,
  RoundRobinEnvelhecimento,
};

inline constexpr std::array<TipoEscalonador, 7> kEscalonadores = {
    TipoEscalonador::FCFS,
    TipoEscalonador::SJF,
    TipoEscalonador::SRTF,
    TipoEscalonador::PrioridadeCooperativa,
    TipoEscalonador::PrioridadePreemptiva,
    TipoEscalonador::RoundRobin,
    TipoEscalonador::RoundRobinEnvelhecimento,
};

std::unique_ptr<IEscalonador> criar_escalonador(TipoEscalonador tipo,
                                                const Config &config);