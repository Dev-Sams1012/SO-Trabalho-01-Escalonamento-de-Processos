add_library(schedulers STATIC
    src/fcfs.cpp
    src/sjf.cpp
    src/srtf.cpp
    src/prioc.cpp
    src/priop.cpp
)

target_include_directories(schedulers PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(schedulers PUBLIC scheduler_core)