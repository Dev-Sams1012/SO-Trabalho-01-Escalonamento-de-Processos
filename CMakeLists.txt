cmake_minimum_required(VERSION 3.16)
project(escalonamento_processos CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_GUI "Compilar a interface grafica (Dear ImGui)" ON)


add_subdirectory(core)
add_subdirectory(schedulers)

add_subdirectory(io)
add_subdirectory(cli)

if(BUILD_GUI)
  add_subdirectory(app)
endif()