#include "ui_resultados.hpp"

#include "imgui.h"

void desenhar_painel_resultados(EstadoApp &estado) {
  ImGui::Begin("Resultados", nullptr,
               ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                   ImGuiWindowFlags_NoCollapse);

  if (!estado.ultimo_resultado.has_value()) {
    ImGui::TextDisabled("Nenhuma simulacao executada ainda.");
    ImGui::End();
    return;
  }

  const ResultadoSimulacao &r = *estado.ultimo_resultado;

  ImGui::SeparatorText("Resumo");
  ImGui::Text("Tempo medio de retorno (turnaround): %.2f u.t.",
              r.tempo_retorno_medio);
  ImGui::Text("Tempo medio de espera: %.2f u.t.", r.tempo_espera_medio);
  ImGui::Text("Trocas de contexto: %d", r.trocas_contexto);

  ImGui::SeparatorText("Por processo");

  const ImGuiTableFlags flags = ImGuiTableFlags_Borders |
                                ImGuiTableFlags_RowBg |
                                ImGuiTableFlags_SizingStretchProp;

  if (ImGui::BeginTable("tabela_resultados", 6, flags)) {
    ImGui::TableSetupColumn("Id");
    ImGui::TableSetupColumn("Chegada (u.t.)");
    ImGui::TableSetupColumn("Duracao (u.t.)");
    ImGui::TableSetupColumn("Conclusao (u.t.)");
    ImGui::TableSetupColumn("Espera (u.t.)");
    ImGui::TableSetupColumn("Retorno (u.t.)");
    ImGui::TableHeadersRow();

    for (const Processo &p : r.processos) {
      ImGui::TableNextRow();
      ImGui::TableNextColumn(); ImGui::Text("P%d", p.id);
      ImGui::TableNextColumn(); ImGui::Text("%d", p.tempo_chegada);
      ImGui::TableNextColumn(); ImGui::Text("%d", p.duracao);
      ImGui::TableNextColumn(); ImGui::Text("%d", p.tempo_conclusao);
      ImGui::TableNextColumn(); ImGui::Text("%d", p.tempo_espera);
      ImGui::TableNextColumn(); ImGui::Text("%d", p.tempo_retorno);
    }

    ImGui::EndTable();
  }

  ImGui::Separator();
  if (ImGui::Button("Voltar ao diagrama")) {
    estado.tela = EstadoApp::Tela::Simulacao;
  }
  ImGui::SameLine();
  if (ImGui::Button("Nova simulacao")) {
    estado.simulador.reset();
    estado.ultimo_resultado.reset();
    estado.simulacao_pausada = false;
    estado.tela = EstadoApp::Tela::Entrada;
  }

  ImGui::End();
}