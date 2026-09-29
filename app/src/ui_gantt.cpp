#include "ui_gantt.hpp"
#include "imgui.h"
#include <string>
#include <vector>

void desenhar_painel_gantt(EstadoApp &estado) {
  ImGui::Begin("Diagrama de Gantt");

  if (!estado.ultimo_resultado.has_value()) {
    ImGui::TextDisabled("Nenhuma simulacao executada ainda.");
    ImGui::End();
    return;
  }

  const ResultadoSimulacao &res = *estado.ultimo_resultado;

  if (res.linha_tempo.empty()) {
    ImGui::TextDisabled("A linha do tempo esta vazia.");
    ImGui::End();
    return;
  }

  // A tabela precisa de 1 coluna para os nomes dos processos + 1 coluna para cada tick de tempo
  const int num_colunas = 1 + static_cast<int>(res.linha_tempo.size());

  // Ativamos rolagem horizontal e bordas
  const ImGuiTableFlags flags = 
      ImGuiTableFlags_Borders | 
      ImGuiTableFlags_RowBg | 
      ImGuiTableFlags_ScrollX;

  if (ImGui::BeginTable("tabela_gantt", num_colunas, flags)) {
    // Trava a primeira coluna (Processos) e a primeira linha (Cabecalho) durante a rolagem
    ImGui::TableSetupScrollFreeze(1, 1);

    // Configura a coluna fixa dos nomes
    ImGui::TableSetupColumn("Processo", ImGuiTableColumnFlags_WidthFixed, 65.0f);

    // Configura as colunas de tempo (ex: "0-1", "1-2")
    for (const auto &tick : res.linha_tempo) {
      std::string label = std::to_string(tick.tempo) + "-" + std::to_string(tick.tempo + 1);
      ImGui::TableSetupColumn(label.c_str(), ImGuiTableColumnFlags_WidthFixed, 35.0f);
    }

    ImGui::TableHeadersRow();

    // Uma linha para cada processo
    for (const Processo &p : res.processos) {
      ImGui::TableNextRow();
      
      // Desenha o nome do processo na primeira coluna
      ImGui::TableNextColumn();
      ImGui::Text("P%d", p.id);

      // Preenche o restante das colunas iterando sobre o tempo
      for (const auto &tick : res.linha_tempo) {
        ImGui::TableNextColumn();

        if (tick.pid_execucao == p.id) {
          // Processo executando neste tick (Cor Azul + ##)
          ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, IM_COL32(40, 115, 180, 255));
          ImGui::Text(" ##");
        } 
        else if (tick.tempo >= p.tempo_chegada && tick.tempo < p.tempo_conclusao) {
          // Processo pronto/esperando na fila (--)
          ImGui::TextDisabled(" --");
        }
      }
    }

    ImGui::EndTable();
  }

  ImGui::End();
}