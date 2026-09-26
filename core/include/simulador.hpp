#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "escalonador.hpp"
#include "processo.hpp"

struct RegistroTick {
  int tempo;
  int pid_execucao;
};

struct ResultadoSimulacao {
  std::vector<Processo> processos;
  std::vector<RegistroTick> linha_tempo;
  int trocas_contexto = 0;
  double tempo_retorno_medio = 0.0;
  double tempo_espera_medio = 0.0;
};

class Simulador {
public:
  Simulador(std::vector<Processo> processos,
            std::unique_ptr<IEscalonador> escalonador);

  ResultadoSimulacao executar();

  void
  definir_callback_tick(std::function<void(const RegistroTick &)> callback);

private:
  std::vector<Processo> processos_;
  std::unique_ptr<IEscalonador> escalonador_;
  std::function<void(const RegistroTick &)> callback_tick_;
};