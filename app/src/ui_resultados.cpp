#include "ui_resultados.hpp"

#include "imgui.h"

void desenhar_painel_resultados(EstadoApp &estado) {
  ImGui::Begin("Resultados");

  if (!estado.ultimo_resultado.has_value()) {
    ImGui::TextDisabled("Nenhuma simulacao executada ainda.");
    ImGui::End();
    return;
  }

  const ResultadoSimulacao &r = *estado.ultimo_resultado;

  ImGui::SeparatorText("Resumo");
  ImGui::Text("Tempo medio de retorno (turnaround): %.2f", r.tempo_retorno_medio);
  ImGui::Text("Tempo medio de espera: %.2f", r.tempo_espera_medio);
  ImGui::Text("Trocas de contexto: %d", r.trocas_contexto);

  ImGui::SeparatorText("Por processo");

  const ImGuiTableFlags flags = ImGuiTableFlags_Borders |
                                ImGuiTableFlags_RowBg |
                                ImGuiTableFlags_SizingStretchProp;

  if (ImGui::BeginTable("tabela_resultados", 6, flags)) {
    ImGui::TableSetupColumn("Id");
    ImGui::TableSetupColumn("Chegada");
    ImGui::TableSetupColumn("Duracao");
    ImGui::TableSetupColumn("Conclusao");
    ImGui::TableSetupColumn("Espera");
    ImGui::TableSetupColumn("Retorno");
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

  ImGui::End();
}