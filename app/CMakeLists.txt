include(FetchContent)

# --- Dependencias baixadas automaticamente (nao precisa instalar nada) ---

set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG        3.4
  GIT_SHALLOW    TRUE
)

FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG        v1.91.9
  GIT_SHALLOW    TRUE
)

FetchContent_Declare(
  tinyfiledialogs
  GIT_REPOSITORY https://github.com/native-toolkit/tinyfiledialogs.git
  GIT_TAG        master
  GIT_SHALLOW    TRUE
)

FetchContent_MakeAvailable(glfw imgui tinyfiledialogs)

find_package(OpenGL REQUIRED)

add_library(imgui_lib STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui_lib PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)
target_link_libraries(imgui_lib PUBLIC glfw OpenGL::GL)

add_library(tinyfd_lib STATIC
    ${tinyfiledialogs_SOURCE_DIR}/tinyfiledialogs.c
)
target_include_directories(tinyfd_lib PUBLIC
    ${tinyfiledialogs_SOURCE_DIR}
)

# --- Executavel da interface grafica ---
# add_executable PRIMEIRO: o alvo "escalonador_gui" precisa existir
# antes de qualquer target_link_libraries/target_include_directories
# que o referencie.
add_executable(escalonador_gui
    src/main.cpp
    src/ui_dados.cpp
    src/carregamento.cpp
    src/execucao.cpp
    src/ui_resultados.cpp
)
target_include_directories(escalonador_gui PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(escalonador_gui PRIVATE imgui_lib scheduler_io tinyfd_lib)

if(WIN32 AND MSVC)
  set_target_properties(escalonador_gui PROPERTIES
      LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()