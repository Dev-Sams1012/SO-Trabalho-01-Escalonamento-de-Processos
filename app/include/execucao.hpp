#pragma once

#include "estado_app.hpp"

// Prepara uma simulacao incremental e troca para a tela do diagrama.
void iniciar_simulacao(EstadoApp &estado);

// Avanca um tick e atualiza o resultado parcial; retorna true ao concluir.
bool avancar_simulacao(EstadoApp &estado);

// True se ha processos suficientes para simular.
bool pode_simular(const EstadoApp &estado);