// Interface grafica do simulador de escalonamento (Dear ImGui + GLFW + OpenGL3).
//
// Fase 1: entrada de dados (tabela de processos, quantum/aging, algoritmo).

#include <cstdio>

#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "estado_app.hpp"
#include "execucao.hpp"
#include "ui_dados.hpp"
#include "ui_resultados.hpp"
#include "ui_gantt.hpp"

static void erro_glfw(int codigo, const char *descricao) {
  std::fprintf(stderr, "GLFW erro %d: %s\n", codigo, descricao);
}

int main() {
  glfwSetErrorCallback(erro_glfw);
  if (!glfwInit()) {
    return 1;
  }

  const char *glsl_version = "#version 330";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow *janela =
      glfwCreateWindow(1100, 700, "Simulador de Escalonamento de Processos",
                       nullptr, nullptr);
  if (!janela) {
    glfwTerminate();
    return 1;
  }
  glfwMakeContextCurrent(janela);
  glfwSwapInterval(1);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(janela, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  EstadoApp estado;

  while (!glfwWindowShouldClose(janela)) {
    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (estado.tela == EstadoApp::Tela::Simulacao &&
        !estado.simulacao_pausada && estado.simulador &&
        !estado.simulador->concluido()) {
      estado.acumulador_tick += ImGui::GetIO().DeltaTime;
      if (estado.acumulador_tick >= 0.45f) {
        estado.acumulador_tick -= 0.45f;
        avancar_simulacao(estado);
      }
    }

    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Always);
    switch (estado.tela) {
    case EstadoApp::Tela::Entrada:
      desenhar_painel_dados(estado);
      break;
    case EstadoApp::Tela::Simulacao:
      desenhar_painel_gantt(estado);
      break;
    case EstadoApp::Tela::Resultados:
      desenhar_painel_resultados(estado);
      break;
    }

    ImGui::Render();
    int largura, altura;
    glfwGetFramebufferSize(janela, &largura, &altura);
    glViewport(0, 0, largura, altura);
    glClearColor(0.10f, 0.10f, 0.12f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(janela);
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwDestroyWindow(janela);
  glfwTerminate();
  return 0;
}