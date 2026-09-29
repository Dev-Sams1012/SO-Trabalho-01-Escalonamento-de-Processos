#include "execucao.hpp"

#include "scheduler_factory.hpp"

bool pode_simular(const EstadoApp &estado) {
  return !estado.processos.empty();
}

void executar_simulacao(EstadoApp &estado) {
  if (!pode_simular(estado)) {
    return;
  }

  const TipoEscalonador tipo = kEscalonadores[estado.algoritmo_selecionado];
  auto escalonador = criar_escalonador(tipo, estado.config);

  // Simulador consome (move) os processos e o escalonador — por isso
  // passamos uma COPIA de estado.processos, para que a tabela de
  // entrada (Fase 1) continue intacta depois de rodar.
  Simulador sim(estado.processos, std::move(escalonador));
  estado.ultimo_resultado = sim.executar();
}