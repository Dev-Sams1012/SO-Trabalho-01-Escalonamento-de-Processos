#include "simulador.hpp"

Simulador::Simulador(std::vector<Processo> processos,
                     std::unique_ptr<IEscalonador> escalonador)
    : processos_(std::move(processos)), escalonador_(std::move(escalonador)) {
  resultado_.processos = processos_;
  concluido_ = processos_.empty();
}

void Simulador::definir_callback_tick(
    std::function<void(const RegistroTick &)> callback) {
  callback_tick_ = std::move(callback);
}

ResultadoSimulacao Simulador::executar() {
  while (!concluido_) {
    avancar_tick();
  }
  return resultado_;
}

bool Simulador::avancar_tick() {
  if (concluido_) {
    return true;
  }

  for (auto &proc : processos_) {
    if (proc.estado == EstadoProcesso::Novo && proc.tempo_chegada == clock_) {
      proc.estado = EstadoProcesso::Pronto;
      proc.prioridade_dinamica = proc.prioridade;
      fila_prontos_.push_back(&proc);
    }
  }

  escalonador_->onTick(fila_prontos_, clock_);
  Processo *escolhido =
      escalonador_->selecionarProximo(fila_prontos_, atual_, clock_);

  if (escolhido != atual_ && atual_ != nullptr &&
      atual_->estado != EstadoProcesso::Finalizado) {
    atual_->estado = EstadoProcesso::Pronto;
    fila_prontos_.push_back(atual_);
  }

  atual_ = escolhido;
  if (atual_ != nullptr) {
    std::erase(fila_prontos_, atual_);
    atual_->estado = EstadoProcesso::Executando;
    if (atual_->tempo_inicio == -1) {
      atual_->tempo_inicio = clock_;
    }
  }

  const int pid_execucao = atual_ ? atual_->id : -1;
  if (clock_ > 0 && pid_execucao != pid_execucao_anterior_) {
    resultado_.trocas_contexto++;
  }
  pid_execucao_anterior_ = pid_execucao;

  resultado_.linha_tempo.push_back({clock_, pid_execucao});
  if (callback_tick_) {
    callback_tick_(resultado_.linha_tempo.back());
  }

  if (atual_ != nullptr) {
    atual_->tempo_restante--;
    if (atual_->tempo_restante <= 0) {
      atual_->tempo_conclusao = clock_ + 1;
      atual_->estado = EstadoProcesso::Finalizado;
      atual_->tempo_retorno = atual_->tempo_conclusao - atual_->tempo_chegada;
      atual_->tempo_espera = atual_->tempo_retorno - atual_->duracao;
      qtd_finalizados_++;
      atual_ = nullptr;
    }
  }

  clock_++;
  resultado_.processos = processos_;

  if (qtd_finalizados_ == static_cast<int>(processos_.size())) {
    double soma_tempo_retorno = 0.0;
    double soma_tempo_espera = 0.0;
    for (const auto &proc : processos_) {
      soma_tempo_retorno += proc.tempo_retorno;
      soma_tempo_espera += proc.tempo_espera;
    }

    const double total = static_cast<double>(processos_.size());
    resultado_.tempo_retorno_medio = soma_tempo_retorno / total;
    resultado_.tempo_espera_medio = soma_tempo_espera / total;
    concluido_ = true;
  }

  return concluido_;
}