#include "simulador.hpp"

Simulador::Simulador(std::vector<Processo> processos,
                     std::unique_ptr<IEscalonador> escalonador)
    : processos_(std::move(processos)), escalonador_(std::move(escalonador)) {}

void Simulador::definir_callback_tick(
    std::function<void(const RegistroTick &)> callback) {
  callback_tick_ = std::move(callback);
}

ResultadoSimulacao Simulador::executar() {
  ResultadoSimulacao resultado;

  const int total = static_cast<int>(processos_.size());
  if (total == 0) {
    resultado.tempo_retorno_medio = 0.0;
    resultado.tempo_espera_medio = 0.0;
    return resultado;
  }

  std::vector<Processo *> fila_prontos;
  Processo *atual = nullptr;

  int qtd_finalizados = 0;
  int pid_execucao_anterior = -1;
  int clock = 0;

  while (qtd_finalizados < total) {

    for (auto &proc : processos_) {
      if (proc.estado == EstadoProcesso::Novo && proc.tempo_chegada == clock) {
        proc.estado = EstadoProcesso::Pronto;
        proc.prioridade_dinamica = proc.prioridade;
        fila_prontos.push_back(&proc);
      }
    }

    escalonador_->onTick(fila_prontos, clock);

    Processo *escolhido =
        escalonador_->selecionarProximo(fila_prontos, atual, clock);

    if (escolhido != atual && atual != nullptr &&
        atual->estado != EstadoProcesso::Finalizado) {
      atual->estado = EstadoProcesso::Pronto;
      fila_prontos.push_back(atual);
    }

    atual = escolhido;

    if (atual != nullptr) {
      std::erase(fila_prontos, atual);
      atual->estado = EstadoProcesso::Executando;

      if (atual->tempo_inicio == -1) {
        atual->tempo_inicio = clock;
      }
    }

    const int pid_execucao = atual ? atual->id : -1;

    if (clock > 0 && pid_execucao != pid_execucao_anterior) {
      resultado.trocas_contexto++;
    }
    pid_execucao_anterior = pid_execucao;

    resultado.linha_tempo.push_back({clock, pid_execucao});
    if (callback_tick_) {
      callback_tick_(resultado.linha_tempo.back());
    }

    if (atual != nullptr) {
      atual->tempo_restante--;
      if (atual->tempo_restante <= 0) {
        atual->tempo_conclusao = clock + 1;
        atual->estado = EstadoProcesso::Finalizado;
        atual->tempo_retorno = atual->tempo_conclusao - atual->tempo_chegada;
        atual->tempo_espera = atual->tempo_retorno - atual->duracao;
        qtd_finalizados++;
        atual = nullptr;
      }
    }

    clock++;
  }

  double soma_tempo_retorno = 0.0;
  double soma_tempo_espera = 0.0;

  for (const auto &p : processos_) {
    soma_tempo_retorno += p.tempo_retorno;
    soma_tempo_espera += p.tempo_espera;
    resultado.processos.push_back(p);
  }

  resultado.tempo_retorno_medio = soma_tempo_retorno / total;
  resultado.tempo_espera_medio = soma_tempo_espera / total;

  return resultado;
}