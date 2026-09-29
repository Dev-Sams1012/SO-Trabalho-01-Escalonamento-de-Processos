#include "ui_gantt.hpp"
#include "imgui.h"
#include <string>
#include <vector>

void desenhar_painel_gantt(EstadoApp &estado) {
  ImGui::Begin("Simulacao - Diagrama de Gantt", nullptr,
               ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                   ImGuiWindowFlags_NoCollapse);

  if (!estado.ultimo_resultado.has_value()) {
    ImGui::TextDisabled("Nenhuma simulacao executada ainda.");
    ImGui::End();
    return;
  }

  const ResultadoSimulacao &res = *estado.ultimo_resultado;
  const int tempo_simulado = res.linha_tempo.empty()
                                 ? 0
                                 : res.linha_tempo.back().tempo + 1;
  ImGui::Text("Tempo simulado: %d u.t.", tempo_simulado);

  if (res.linha_tempo.empty()) {
    ImGui::TextDisabled("Aguardando o primeiro tick...");
  } else {
    const int num_colunas = 1 + static_cast<int>(res.linha_tempo.size());
    const ImGuiTableFlags flags = ImGuiTableFlags_Borders |
                                  ImGuiTableFlags_RowBg |
                                  ImGuiTableFlags_ScrollX;

    if (ImGui::BeginTable("tabela_gantt", num_colunas, flags)) {
      ImGui::TableSetupScrollFreeze(1, 1);
      ImGui::TableSetupColumn("Processo", ImGuiTableColumnFlags_WidthFixed,
                              65.0f);

      for (const auto &tick : res.linha_tempo) {
        std::string label = std::to_string(tick.tempo) + "-" +
                            std::to_string(tick.tempo + 1);
        ImGui::TableSetupColumn(label.c_str(),
                                ImGuiTableColumnFlags_WidthFixed, 35.0f);
      }

      ImGui::TableHeadersRow();
      for (const Processo &p : res.processos) {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text("P%d", p.id);

        for (const auto &tick : res.linha_tempo) {
          ImGui::TableNextColumn();
          if (tick.pid_execucao == p.id) {
            ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg,
                                   IM_COL32(40, 115, 180, 255));
            ImGui::Text(" ##");
          } else if (tick.tempo >= p.tempo_chegada &&
                     (p.tempo_conclusao < 0 ||
                      tick.tempo < p.tempo_conclusao)) {
            ImGui::TextDisabled(" --");
          }
        }
      }

      ImGui::EndTable();
    }
  }

  ImGui::Separator();
  if (estado.simulador && estado.simulador->concluido()) {
    ImGui::Text("Simulacao concluida.");
    if (ImGui::Button("Ver resultados")) {
      estado.tela = EstadoApp::Tela::Resultados;
    }
  } else {
    if (ImGui::Button(estado.simulacao_pausada ? "Continuar" : "Pausar")) {
      estado.simulacao_pausada = !estado.simulacao_pausada;
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancelar e voltar")) {
      estado.simulador.reset();
      estado.ultimo_resultado.reset();
      estado.simulacao_pausada = false;
      estado.tela = EstadoApp::Tela::Entrada;
    }
  }

  ImGui::End();
}