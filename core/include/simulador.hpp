#pragma once

#include "escalonador.hpp"
#include "processo.hpp"

#include <functional>
#include <memory>
#include <vector>

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
  bool avancar_tick();
  bool concluido() const { return concluido_; }
  const ResultadoSimulacao &resultado() const { return resultado_; }

  void
  definir_callback_tick(std::function<void(const RegistroTick &)> callback);

private:
  std::vector<Processo> processos_;
  std::unique_ptr<IEscalonador> escalonador_;
  std::function<void(const RegistroTick &)> callback_tick_;
  std::vector<Processo *> fila_prontos_;
  Processo *atual_ = nullptr;
  ResultadoSimulacao resultado_;
  int qtd_finalizados_ = 0;
  int pid_execucao_anterior_ = -1;
  int clock_ = 0;
  bool concluido_ = false;
};