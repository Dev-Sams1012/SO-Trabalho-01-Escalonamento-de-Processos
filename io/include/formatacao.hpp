#pragma once

// Formata o resultado da simulacao no padrao pedido
//   tempo medio de vida (tt), tempo medio de espera (tw),
//   numero de trocas de contexto e diagrama de tempo vertical.

#include <string>

#include "simulador.hpp"

// Diagrama vertical, uma linha por segundo:
//   "##" = executando, "--" = pronto (esperando), "  " = ainda nao
//   chegou ou ja terminou.
std::string formatar_diagrama(const ResultadoSimulacao &resultado);

// Resumo: tt, tw e trocas de contexto.
std::string formatar_resumo(const ResultadoSimulacao &resultado);

// Relatorio completo de um algoritmo (nome + resumo + diagrama).
std::string formatar_relatorio(const std::string &nome_algoritmo,
                               const ResultadoSimulacao &resultado);