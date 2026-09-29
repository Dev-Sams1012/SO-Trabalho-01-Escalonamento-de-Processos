#pragma once

#include "estado_app.hpp"

// Abre o dialogo nativo do sistema operacional para escolher um arquivo
// de processos, tenta ler e, se bem-sucedido, substitui
// estado.processos. Erros (arquivo invalido, cancelado, etc.) sao
// gravados em estado.erros_carregamento.
void carregar_processos_de_arquivo(EstadoApp &estado);

// Mesma ideia, mas para o arquivo de configuracao (quantum/aging).
void carregar_config_de_arquivo(EstadoApp &estado);