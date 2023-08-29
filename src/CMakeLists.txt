set(TETRIS_SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
file(GLOB TETRIS_SRC "${TETRIS_SOURCES_DIR}/*.cpp")

add_executable(Tetris ${TETRIS_SRC})


target_link_libraries(Tetris PRIVATE sfml-graphics sfml-window sfml-system sfml-audio)

