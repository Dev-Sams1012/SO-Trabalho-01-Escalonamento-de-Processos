#pragma once

#include "escalonador.hpp"
#include "processo.hpp"

#include <memory>
#include <vector>

struct RegistroTick {
  // Instante e processo registrado na linha do tempo.
  int tempo;
  int pid_execucao; // -1 quando a CPU ficou ociosa.
};

// Guarda a linha do tempo e as medidas finais da execucao.
struct ResultadoSimulacao {
  std::vector<Processo> processos;
  std::vector<RegistroTick> linha_tempo;
  int trocas_contexto = 0;
  double tempo_retorno_medio = 0.0;
  double tempo_espera_medio = 0.0;
};

// Coordena os processos e aplica as decisoes do escalonador a cada tick.
class Simulador {
public:
  Simulador(std::vector<Processo> processos,
            std::unique_ptr<IEscalonador> escalonador);

  ResultadoSimulacao executar();
  bool avancar_tick();
  bool concluido() const { return concluido_; }
  const ResultadoSimulacao &resultado() const { return resultado_; }

private:
  std::vector<Processo> processos_;
  std::unique_ptr<IEscalonador> escalonador_;
  // A fila guarda referencias aos processos que estao prontos.
  std::vector<Processo *> fila_prontos_;
  Processo *atual_ = nullptr; // Processo usando a CPU no tick atual.
  ResultadoSimulacao resultado_;
  int qtd_finalizados_ = 0;
  int pid_execucao_anterior_ = -1; // Usado para contar trocas de contexto.
  int clock_ = 0;                  // Instante que sera simulado.
  bool concluido_ = false;
};