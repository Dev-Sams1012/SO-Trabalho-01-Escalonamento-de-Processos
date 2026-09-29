#pragma once

#include "estado_app.hpp"

// Executa o algoritmo selecionado sobre os processos atuais e grava
// o resultado em estado.ultimo_resultado.
//
// Nao faz nada (e nao altera ultimo_resultado) se a lista de processos
// estiver vazia — chame pode_simular(estado) antes, se quiser evitar a
// chamada nesse caso.
void executar_simulacao(EstadoApp &estado);

// True se ha processos suficientes para simular.
bool pode_simular(const EstadoApp &estado);