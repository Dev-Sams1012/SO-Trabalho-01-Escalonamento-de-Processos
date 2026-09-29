add_executable(escalonador_cli
    src/main.cpp
)

target_link_libraries(escalonador_cli PRIVATE scheduler_io)