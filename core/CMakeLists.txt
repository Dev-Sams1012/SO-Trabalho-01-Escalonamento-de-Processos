add_library(scheduler_core STATIC
    src/simulador.cpp
)

target_include_directories(scheduler_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(scheduler_core PUBLIC cxx_std_20)
