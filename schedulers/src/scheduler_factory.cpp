#include "scheduler_factory.hpp"

#include <stdexcept>

#include "fcfs.hpp"
#include "prioc.hpp"
#include "priop.hpp"
#include "round_robin.hpp"
#include "round_robin_aging.hpp"
#include "sjf.hpp"
#include "srtf.hpp"

std::unique_ptr<IEscalonador> criar_escalonador(TipoEscalonador tipo, const Config &config) {
  switch (tipo) {
    case TipoEscalonador::FCFS:
      return std::make_unique<FCFS>();
    case TipoEscalonador::SJF:
      return std::make_unique<SJF>();
    case TipoEscalonador::SRTF:
      return std::make_unique<SRTF>();
    case TipoEscalonador::PrioridadeCooperativa:
      return std::make_unique<PRIOc>();
    case TipoEscalonador::PrioridadePreemptiva:
      return std::make_unique<PRIOp>();
    case TipoEscalonador::RoundRobin:
      return std::make_unique<RoundRobin>(config.quantum);
    case TipoEscalonador::RoundRobinEnvelhecimento:
      return std::make_unique<RoundRobinAging>(config.quantum, config.aging);
  }

  throw std::invalid_argument("algoritmo de escalonamento desconhecido");
}