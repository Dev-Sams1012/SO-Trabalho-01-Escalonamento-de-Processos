add_library(scheduler_io STATIC
    src/entrada.cpp
    src/formatacao.cpp
)

target_include_directories(scheduler_io PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Precisa de Processo (core) e Config (schedulers)
target_link_libraries(scheduler_io PUBLIC scheduler_core schedulers)