cmake_minimum_required(
    VERSION 3.10.0
)

project(
    ChessDev
    VERSION 0.1
    LANGUAGES CXX
)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")

# Cortex Engine static lib compiled here
add_subdirectory(engine)

# Client exe compiled here
add_subdirectory(chess)