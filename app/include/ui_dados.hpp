#pragma once

#include "estado_app.hpp"

// Desenha o painel de entrada de dados: tabela de processos, config
// (quantum/aging) e o combo de selecao do algoritmo.
//
// Chamado uma vez por frame, dentro do loop principal do ImGui.
void desenhar_painel_dados(EstadoApp &estado);