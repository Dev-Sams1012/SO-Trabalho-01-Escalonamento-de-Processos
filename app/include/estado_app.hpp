#pragma once

#include <optional>

#include "simulador.hpp"
#include <vector>
#include <string>

#include "config.hpp"
#include "processo.hpp"
#include "scheduler_factory.hpp"

// Estado completo da interface grafica: tudo o que o usuario configurou
// e (nas proximas fases) o resultado da ultima simulacao.
//
// Vive por toda a execucao do programa (uma instancia em main()), e cada
// painel (ui_dados, ui_resultados, ...) le e escreve nos mesmos campos.
struct EstadoApp {
  // --- Dados de entrada (Fase 1) ---
  std::vector<Processo> processos;
  Config config{2, 1};              // quantum, aging
  int algoritmo_selecionado = 0;    // indice em kEscalonadores
  int proximo_id = 1;

  std::vector<std::string> erros_carregamento;
  std::optional<ResultadoSimulacao> ultimo_resultado;
};