#include "execucao.hpp"

#include "scheduler_factory.hpp"

bool pode_simular(const EstadoApp &estado) {
  return !estado.processos.empty();
}

void iniciar_simulacao(EstadoApp &estado) {
  if (!pode_simular(estado)) {
    return;
  }

  const TipoEscalonador tipo = kEscalonadores[estado.algoritmo_selecionado];
  auto escalonador = criar_escalonador(tipo, estado.config);

  estado.simulador = std::make_unique<Simulador>(estado.processos,
                                                std::move(escalonador));
  estado.ultimo_resultado = estado.simulador->resultado();
  estado.acumulador_tick = 0.0f;
  estado.simulacao_pausada = false;
  estado.tela = EstadoApp::Tela::Simulacao;
}

bool avancar_simulacao(EstadoApp &estado) {
  if (!estado.simulador) {
    return true;
  }

  const bool concluida = estado.simulador->avancar_tick();
  estado.ultimo_resultado = estado.simulador->resultado();
  return concluida;
}