project(
    Wigner
    VERSION 0.1
    DESCRIPTION "Wigner Engine"
    LANGUAGES C CXX
)

################### VENDOR ###################

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(vendor/glfw)

set(
    VENDOR_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/vendor/glad/include
    ${PROJECT_SOURCE_DIR}/vendor/glfw/include
    ${PROJECT_SOURCE_DIR}/vendor/glm
)

set(
    VENDOR_SOURCES
    ${PROJECT_SOURCE_DIR}/vendor/glad/src/glad.c
)

################### LIBRARY ##################

set(
    LOCAL_HEADERS

    ${PROJECT_SOURCE_DIR}/source/${PROJECT_NAME}/Base/Defines.hpp
    ${PROJECT_SOURCE_DIR}/source/${PROJECT_NAME}/Base/Asserts.hpp
    ${PROJECT_SOURCE_DIR}/source/${PROJECT_NAME}/Base/Logging.hpp
    ${PROJECT_SOURCE_DIR}/source/${PROJECT_NAME}/Base/Base.hpp

    ${PROJECT_SOURCE_DIR}/source/${PROJECT_NAME}/Core/Entrypoint.hpp
    ${PROJECT_SOURCE_DIR}/source/${PROJECT_NAME}/Core/Application.hpp
    ${PROJECT_SOURCE_DIR}/source/${PROJECT_NAME}/Core/Window.hpp

    ${PROJECT_SOURCE_DIR}/source/${PROJECT_NAME}/Events/Events.hpp
)

set(
    LOCAL_SOURCES

    ${PROJECT_SOURCE_DIR}/source/${PROJECT_NAME}/Base/Logging.cpp

    ${PROJECT_SOURCE_DIR}/source/${PROJECT_NAME}/Core/Application.cpp
    ${PROJECT_SOURCE_DIR}/source/${PROJECT_NAME}/Core/Window.cpp

    ${PROJECT_SOURCE_DIR}/source/${PROJECT_NAME}/Events/Events.cpp
)

add_library(
    ${PROJECT_NAME} STATIC
    ${LOCAL_SOURCES}
    ${VENDOR_SOURCES}
    ${LOCAL_HEADERS}
)

target_include_directories(
    ${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/source
    ${VENDOR_INCLUDE_DIRS}
)

target_link_libraries(
    ${PROJECT_NAME} PUBLIC
    glfw ${GLFW_LIBRARIES}
)

target_compile_features(
    ${PROJECT_NAME} PUBLIC
    cxx_std_17
)