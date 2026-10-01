#include "fcfs.hpp"

#include "desempate.hpp"

Processo *FCFS::selecionarProximo(std::vector<Processo *> &fila_prontos,
                                  Processo *atual, int /*tempo_atual*/) {
  // FCFS nao interrompe o processo que ja esta usando a CPU.
  if (atual != nullptr) {
    return atual;
  }

  if (fila_prontos.empty()) {
    return nullptr;
  }

  // Escolhe quem chegou primeiro entre os processos prontos.
  auto empatados = filtrar_por_extremo(fila_prontos, &Processo::tempo_chegada,
                                       Extremo::Minimo);
  return desempatar(empatados, atual);
}