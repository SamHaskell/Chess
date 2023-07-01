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
    ${PROJECT_SOURCE_DIR}/source/board.cpp
)

set(
    LOCAL_HEADERS
    ${PROJECT_SOURCE_DIR}/source/game.hpp
    ${PROJECT_SOURCE_DIR}/source/board.hpp
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