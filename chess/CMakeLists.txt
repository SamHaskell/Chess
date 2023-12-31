project(
    chess
    VERSION 0.1
    DESCRIPTION "Chess App"
    LANGUAGES CXX
)

set(
    LOCAL_SOURCES
    ${PROJECT_SOURCE_DIR}/source/main.cpp
    ${PROJECT_SOURCE_DIR}/source/game.cpp
    ${PROJECT_SOURCE_DIR}/source/position.cpp
    ${PROJECT_SOURCE_DIR}/source/board.cpp
    ${PROJECT_SOURCE_DIR}/source/move.cpp
    ${PROJECT_SOURCE_DIR}/source/movegen.cpp
    ${PROJECT_SOURCE_DIR}/source/piece.cpp
)

set(
    LOCAL_HEADERS
    ${PROJECT_SOURCE_DIR}/source/game.hpp
    ${PROJECT_SOURCE_DIR}/source/position.hpp
    ${PROJECT_SOURCE_DIR}/source/board.hpp
    ${PROJECT_SOURCE_DIR}/source/move.hpp
    ${PROJECT_SOURCE_DIR}/source/movegen.hpp
    ${PROJECT_SOURCE_DIR}/source/piece.hpp
)

add_executable(
    ${PROJECT_NAME}
    ${LOCAL_SOURCES}
    ${LOCAL_HEADERS}
)

target_compile_features(
    ${PROJECT_NAME} PRIVATE
    cxx_std_17
)

target_include_directories(
    ${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/source
)

target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    Wigner
)