#include "ui_dados.hpp"

#include <string>
#include <vector>

#include "carregamento.hpp"
#include "entrada.hpp"
#include "execucao.hpp"
#include "imgui.h"
#include "processo.hpp"

void desenhar_painel_dados(EstadoApp &estado) {
  ImGui::Begin("Dados de entrada");

  // --- Configuracao (quantum / aging) ---
  ImGui::SeparatorText("Configuracao");

  if (ImGui::Button("Carregar config de arquivo...")) {
    carregar_config_de_arquivo(estado);
    estado.ultimo_resultado.reset();
  }

  ImGui::SetNextItemWidth(120);
  if (ImGui::InputInt("Quantum", &estado.config.quantum)) {
    estado.ultimo_resultado.reset();
  }
  if (estado.config.quantum < 1) estado.config.quantum = 1;

  ImGui::SameLine();
  ImGui::SetNextItemWidth(120);
  if (ImGui::InputInt("Aging", &estado.config.aging)) {
    estado.ultimo_resultado.reset();
  }
  if (estado.config.aging < 0) estado.config.aging = 0;

  // --- Selecao do algoritmo ---
  ImGui::SeparatorText("Algoritmo");

  static std::vector<std::string> nomes_algoritmos = [] {
    std::vector<std::string> nomes;
    for (TipoEscalonador tipo : kEscalonadores) {
      nomes.push_back(std::string(criar_escalonador(tipo, Config{2, 1})->nome()));
    }
    return nomes;
  }();

  if (ImGui::BeginCombo("##algoritmo",
                        nomes_algoritmos[estado.algoritmo_selecionado].c_str())) {
    for (int i = 0; i < static_cast<int>(nomes_algoritmos.size()); ++i) {
      const bool selecionado = (i == estado.algoritmo_selecionado);
      if (ImGui::Selectable(nomes_algoritmos[i].c_str(), selecionado)) {
        estado.algoritmo_selecionado = i;
        estado.ultimo_resultado.reset();
      }
      if (selecionado) {
        ImGui::SetItemDefaultFocus();
      }
    }
    ImGui::EndCombo();
  }

  // --- Tabela de processos ---
  ImGui::SeparatorText("Processos");

  if (ImGui::Button("Carregar processos de arquivo...")) {
    carregar_processos_de_arquivo(estado);
    estado.ultimo_resultado.reset();
  }
  ImGui::SameLine();
  if (ImGui::Button("Adicionar processo")) {
    estado.processos.push_back(
        criar_processo(estado.proximo_id++, /*chegada=*/0, /*duracao=*/1,
                       /*prioridade=*/1));
    estado.ultimo_resultado.reset();
  }
  ImGui::SameLine();
  if (ImGui::Button("Limpar tudo")) {
    estado.processos.clear();
    estado.ultimo_resultado.reset();
  }

  const ImGuiTableFlags flags = ImGuiTableFlags_Borders |
                                ImGuiTableFlags_RowBg |
                                ImGuiTableFlags_SizingStretchProp;

  if (ImGui::BeginTable("tabela_processos", 5, flags)) {
    ImGui::TableSetupColumn("Id", ImGuiTableColumnFlags_WidthFixed, 40.0f);
    ImGui::TableSetupColumn("Chegada");
    ImGui::TableSetupColumn("Duracao");
    ImGui::TableSetupColumn("Prioridade");
    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 80.0f);
    ImGui::TableHeadersRow();

    int indice_para_remover = -1;

    for (int i = 0; i < static_cast<int>(estado.processos.size()); ++i) {
      Processo &p = estado.processos[i];
      ImGui::TableNextRow();
      ImGui::PushID(i);

      ImGui::TableSetColumnIndex(0);
      ImGui::Text("P%d", p.id);

      ImGui::TableSetColumnIndex(1);
      ImGui::SetNextItemWidth(-1);
      if (ImGui::InputInt("##chegada", &p.tempo_chegada)) {
        estado.ultimo_resultado.reset();
      }
      if (p.tempo_chegada < 0) p.tempo_chegada = 0;

      ImGui::TableSetColumnIndex(2);
      ImGui::SetNextItemWidth(-1);
      if (ImGui::InputInt("##duracao", &p.duracao)) {
        estado.ultimo_resultado.reset();
      }
      if (p.duracao < 1) p.duracao = 1;
      p.tempo_restante = p.duracao;
      p.estado = EstadoProcesso::Novo;

      ImGui::TableSetColumnIndex(3);
      ImGui::SetNextItemWidth(-1);
      if (ImGui::InputInt("##prioridade", &p.prioridade)) {
        estado.ultimo_resultado.reset();
      }
      if (p.prioridade < 1) p.prioridade = 1;
      p.prioridade_dinamica = p.prioridade;

      ImGui::TableSetColumnIndex(4);
      if (ImGui::Button("Remover")) {
        indice_para_remover = i;
        estado.ultimo_resultado.reset();
      }

      ImGui::PopID();
    }

    if (indice_para_remover >= 0) {
      estado.processos.erase(estado.processos.begin() + indice_para_remover);
    }

    ImGui::EndTable();
  }

  if (estado.processos.empty()) {
    ImGui::TextDisabled("Nenhum processo. Clique em \"Adicionar processo\".");
  }

  // --- Erros de carregamento de arquivo ---
  if (!estado.erros_carregamento.empty()) {
    ImGui::SeparatorText("Erros ao carregar arquivo");
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 100, 100, 255));
    for (const auto &erro : estado.erros_carregamento) {
      ImGui::TextWrapped("%s", erro.c_str());
    }
    ImGui::PopStyleColor();
    if (ImGui::Button("Fechar mensagens")) {
      estado.erros_carregamento.clear();
    }
  }

  // --- Iniciar simulacao ---
  ImGui::Separator();

  const bool pode_rodar = pode_simular(estado);
  if (!pode_rodar) {
    ImGui::BeginDisabled();
  }
  if (ImGui::Button("Iniciar simulacao", ImVec2(-1, 40))) {
    executar_simulacao(estado);
  }
  if (!pode_rodar) {
    ImGui::EndDisabled();
    ImGui::TextDisabled("Adicione ao menos um processo para simular.");
  }

  ImGui::End();
}