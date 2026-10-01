#include "simulador.hpp"

Simulador::Simulador(std::vector<Processo> processos,
                     std::unique_ptr<IEscalonador> escalonador)
    : processos_(std::move(processos)), escalonador_(std::move(escalonador)) {
  // Mantem uma copia para disponibilizar o estado inicial do resultado.
  resultado_.processos = processos_;
  // Uma entrada vazia nao precisa passar por nenhum tick.
  concluido_ = processos_.empty();
}

ResultadoSimulacao Simulador::executar() {
  // Executa ticks ate que todos os processos terminem.
  while (!concluido_) {
    avancar_tick();
  }
  return resultado_;
}

bool Simulador::avancar_tick() {
  if (concluido_) {
    return true;
  }

  // Coloca na fila os processos cuja chegada coincide com o relogio.
  for (auto &proc : processos_) {
    if (proc.estado == EstadoProcesso::Novo && proc.tempo_chegada == clock_) {
      proc.estado = EstadoProcesso::Pronto;
      // A prioridade dinamica comeca igual a prioridade informada.
      proc.prioridade_dinamica = proc.prioridade;
      fila_prontos_.push_back(&proc);
    }
  }

  // O algoritmo pode atualizar suas regras antes de escolher quem executa.
  escalonador_->onTick(fila_prontos_, clock_);
  Processo *escolhido =
      escalonador_->selecionarProximo(fila_prontos_, atual_, clock_);

  // Se outro processo foi escolhido, o anterior volta a aguardar na fila.
  if (escolhido != atual_ && atual_ != nullptr &&
      atual_->estado != EstadoProcesso::Finalizado) {
    atual_->estado = EstadoProcesso::Pronto;
    fila_prontos_.push_back(atual_);
  }

  atual_ = escolhido;
  if (atual_ != nullptr) {
    // Remove o escolhido da fila e marca seu estado para este tick.
    std::erase(fila_prontos_, atual_);
    atual_->estado = EstadoProcesso::Executando;
    // Guarda a primeira vez em que o processo usa a CPU.
    if (atual_->tempo_inicio == -1) {
      atual_->tempo_inicio = clock_;
    }
  }

  // -1 representa um tick em que a CPU ficou sem processo.
  const int pid_execucao = atual_ ? atual_->id : -1;

  // Conta a troca apenas quando a CPU passa diretamente entre processos.
  if (clock_ > 0 && pid_execucao_anterior_ != -1 && pid_execucao != -1 &&
      pid_execucao != pid_execucao_anterior_) {
    resultado_.trocas_contexto++;
  }

  pid_execucao_anterior_ = pid_execucao;

  // Registra quem executou neste tick e avisa quem acompanha a simulacao.
  resultado_.linha_tempo.push_back({clock_, pid_execucao});

  if (atual_ != nullptr) {
    // Cada tick de execucao consome uma unidade do tempo restante.
    atual_->tempo_restante--;
    if (atual_->tempo_restante <= 0) {
      // A conclusao ocorre no fim do tick atual.
      atual_->tempo_conclusao = clock_ + 1;
      atual_->estado = EstadoProcesso::Finalizado;
      // Retorno inclui espera; espera desconta o tempo de execucao.
      atual_->tempo_retorno = atual_->tempo_conclusao - atual_->tempo_chegada;
      atual_->tempo_espera = atual_->tempo_retorno - atual_->duracao;
      qtd_finalizados_++;
      atual_ = nullptr;
    }
  }

  // Prepara o relogio e o resultado para o proximo tick.
  clock_++;
  resultado_.processos = processos_;

  if (qtd_finalizados_ == static_cast<int>(processos_.size())) {
    // Calcula as medias somente quando todos os processos terminaram.
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