set(SOURCES
    Tetris/src/Application.cpp
)

set(HEADERS
)

add_executable(Tetris ${SOURCES} ${HEADERS})

add_dependencies(Tetris
    puzzlib
)

target_link_libraries(Tetris
    puzzlib
)

target_include_directories(Tetris PRIVATE
    ${CMAKE_SOURCE_DIR}/puzzlib
    ${CMAKE_SOURCE_DIR}/puzzlib/src
)

target_compile_definitions(Tetris PRIVATE
)