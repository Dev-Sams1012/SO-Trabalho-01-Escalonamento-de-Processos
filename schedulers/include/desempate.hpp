#pragma once

#include <functional>
#include <random>
#include <vector>

#include "processo.hpp"

enum class Extremo { Minimo, Maximo };

template <typename Chave>
std::vector<Processo *>
filtrar_por_extremo(const std::vector<Processo *> &candidatos, Chave chave,
                    Extremo tipo) {
  std::vector<Processo *> resultado;
  if (candidatos.empty()) {
    return resultado;
  }

  auto valor_extremo = std::invoke(chave, candidatos.front());
  for (Processo *p : candidatos) {
    const auto valor = std::invoke(chave, p);
    const bool melhora = (tipo == Extremo::Minimo) ? (valor < valor_extremo)
                                                   : (valor > valor_extremo);
    if (melhora) {
      valor_extremo = valor;
    }
  }

  for (Processo *p : candidatos) {
    if (std::invoke(chave, p) == valor_extremo) {
      resultado.push_back(p);
    }
  }
  return resultado;
}

inline Processo *desempatar(const std::vector<Processo *> &empatados,
                            Processo *atual) {
  if (empatados.empty()) {
    return nullptr;
  }
  if (empatados.size() == 1) {
    return empatados.front();
  }

  for (Processo *p : empatados) {
    if (p == atual) {
      return p;
    }
  }

  auto restantes = filtrar_por_extremo(empatados, &Processo::tempo_restante,
                                       Extremo::Minimo);
  if (restantes.size() == 1) {
    return restantes.front();
  }

  static std::mt19937 rng{std::random_device{}()};
  std::uniform_int_distribution<std::size_t> dist(0, restantes.size() - 1);
  return restantes[dist(rng)];
}