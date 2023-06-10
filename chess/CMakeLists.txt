project(
    Chess
    VERSION 0.1
    DESCRIPTION "Chess App"
    LANGUAGES CXX
)

set(
    LOCAL_SOURCES
    ${PROJECT_SOURCE_DIR}/source/chess.cpp
)

set(
    LOCAL_HEADERS
    
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

target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    Wigner
)