#pragma once

enum class EstadoProcesso { Novo, Pronto, Executando, Finalizado };

struct Processo {
  int id;
  int tempo_chegada;
  int duracao;
  int tempo_restante;
  int prioridade;
  int prioridade_dinamica;

  EstadoProcesso estado = EstadoProcesso::Novo;

  int tempo_inicio = -1;
  int tempo_conclusao = -1;
  int tempo_espera = 0;
  int tempo_retorno = 0;
};
