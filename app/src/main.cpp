// Interface grafica do simulador de escalonamento (Dear ImGui + GLFW + OpenGL3).
//
// Fase 0: apenas abre a janela e confirma que o backend esta linkado.
// As proximas fases adicionam entrada de dados, resultados e animacao.

#include <cstdio>

#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "scheduler_factory.hpp"

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
  glfwSwapInterval(1);  // vsync

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(janela, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  while (!glfwWindowShouldClose(janela)) {
    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Simulador");
    ImGui::Text("Backend carregado: %d algoritmos disponiveis.",
                static_cast<int>(kEscalonadores.size()));
    ImGui::End();

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