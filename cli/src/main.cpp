// Versao de linha de comando do simulador.
//
// Uso:
//   escalonador_cli [--config arquivo] [--algoritmo N | --all] < processos.txt
//
// Le os processos da entrada padrao (stdin) e escreve o relatorio na saida
// padrao (stdout), como pede o enunciado.

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>

#include "entrada.hpp"
#include "formatacao.hpp"
#include "scheduler_factory.hpp"
#include "simulador.hpp"

namespace {

void mostrar_uso(const char *prog) {
  std::cerr
      << "Uso: " << prog
      << " [--config arquivo] [--algoritmo N | --all] < processos.txt\n\n"
         "  --config arquivo  arquivo com quantum:N e aging:N "
         "(padrao: config.txt, se existir)\n"
         "  --algoritmo N     1=FCFS 2=SJF 3=SRTF 4=Prioridade (coop.)\n"
         "                    5=Prioridade (preemptiva) 6=Round-Robin\n"
         "                    7=Round-Robin + prioridade/envelhecimento\n"
         "  --all             executa os 7 algoritmos (padrao)\n"
         "  --help            mostra esta ajuda\n";
}

void mostrar_erros(const std::string &origem,
                   const std::vector<ErroLeitura> &erros) {
  for (const auto &e : erros) {
    std::cerr << origem;
    if (e.linha > 0) {
      std::cerr << ":" << e.linha;
    }
    std::cerr << ": erro: " << e.mensagem << '\n';
  }
}

void executar_um(TipoEscalonador tipo, const Config &config,
                 const std::vector<Processo> &processos) {
  auto escalonador = criar_escalonador(tipo, config);
  const std::string nome(escalonador->nome());

  Simulador sim(processos, std::move(escalonador));
  const ResultadoSimulacao resultado = sim.executar();
  std::cout << formatar_relatorio(nome, resultado) << '\n';
}

}  // namespace

int main(int argc, char **argv) {
  std::string caminho_config;
  int algoritmo = 0;  // 0 = todos

  for (int i = 1; i < argc; ++i) {
    const std::string_view arg = argv[i];
    if (arg == "--help" || arg == "-h") {
      mostrar_uso(argv[0]);
      return 0;
    } else if (arg == "--all") {
      algoritmo = 0;
    } else if (arg == "--config" && i + 1 < argc) {
      caminho_config = argv[++i];
    } else if (arg == "--algoritmo" && i + 1 < argc) {
      algoritmo = std::atoi(argv[++i]);
      if (algoritmo < 1 || algoritmo > static_cast<int>(kEscalonadores.size())) {
        std::cerr << "erro: --algoritmo deve ser de 1 a "
                  << kEscalonadores.size() << '\n';
        return 2;
      }
    } else {
      std::cerr << "erro: argumento invalido: " << arg << "\n\n";
      mostrar_uso(argv[0]);
      return 2;
    }
  }

  // --- config (quantum / aging) ---
  Config config{2, 1};
  if (caminho_config.empty()) {
    caminho_config = "config.txt";
    if (std::ifstream(caminho_config)) {
      auto lido = ler_config_arquivo(caminho_config);
      mostrar_erros(caminho_config, lido.erros);
      if (!lido.ok()) return 1;
      config = lido.config;
    }
  } else {
    auto lido = ler_config_arquivo(caminho_config);
    mostrar_erros(caminho_config, lido.erros);
    if (!lido.ok()) return 1;
    config = lido.config;
  }

  // --- processos (stdin) ---
  auto lidos = ler_processos(std::cin);
  mostrar_erros("stdin", lidos.erros);
  if (!lidos.ok()) return 1;
  if (lidos.processos.empty()) {
    std::cerr << "erro: nenhum processo informado na entrada\n";
    return 1;
  }

  // --- executa ---
  if (algoritmo == 0) {
    for (TipoEscalonador tipo : kEscalonadores) {
      executar_um(tipo, config, lidos.processos);
    }
  } else {
    executar_um(kEscalonadores[algoritmo - 1], config, lidos.processos);
  }
  return 0;
}