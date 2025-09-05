cmake_minimum_required(VERSION 3.16)

project(SimpleEngine
    VERSION 0.1.0
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_WARNINGS "Enable extra compiler warnings" ON)
option(USE_SYSTEM_GLAD "Use system-installed GLAD via pkg-config" OFF)

# Saída dos artefatos
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

# ========= GLFW =========
# Desliga qualquer coisa que possa compilar a cópia do GLAD do GLFW (exemplos/tests)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)
add_subdirectory(third_party/glfw)

# ========= GLAD =========
# Preferimos a cópia vendorizada (GLAD v1: include/glad/glad.h + src/glad.c)
if (USE_SYSTEM_GLAD)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GLAD REQUIRED glad)
    add_library(glad::glad INTERFACE IMPORTED)
    target_include_directories(glad::glad INTERFACE ${GLAD_INCLUDE_DIRS})
    target_link_libraries(glad::glad INTERFACE ${GLAD_LIBRARIES})
else()
    set(GLAD_SRC "${CMAKE_SOURCE_DIR}/third_party/glad/src/glad.c")
    set(GLAD_INC "${CMAKE_SOURCE_DIR}/third_party/glad/include")
    if (NOT (EXISTS "${GLAD_SRC}" AND EXISTS "${GLAD_INC}"))
        message(FATAL_ERROR "GLAD vendorizado não encontrado em third_party/glad.")
    endif()
    add_library(glad STATIC "${GLAD_SRC}")
    target_include_directories(glad PUBLIC "${GLAD_INC}")
    add_library(glad::glad ALIAS glad)
endif()

# ========= OpenGL =========
find_package(OpenGL REQUIRED)

# ========= Engine =========
add_library(Engine
    src/Application.cpp
    src/Shader.cpp
    src/Window.cpp
)

# Includes públicos (ajuste se preferir outro layout)
target_include_directories(Engine
    PUBLIC
        ${CMAKE_SOURCE_DIR}/includes
)

# Define útil para paths absolutos em runtime (ex.: shaders/)
target_compile_definitions(Engine
    PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
)

# Linkagem
target_link_libraries(Engine
    PRIVATE
        glad::glad
        glfw
        OpenGL::GL
)

# Remover QUALQUER definição global indesejada de GLAD_GL_IMPLEMENTATION
if (MSVC)
    target_compile_options(Engine PRIVATE /U GLAD_GL_IMPLEMENTATION)
else()
    target_compile_options(Engine PRIVATE -UGLAD_GL_IMPLEMENTATION)
endif()

# Warnings
if (ENABLE_WARNINGS)
    if (MSVC)
        target_compile_options(Engine PRIVATE /W4 /permissive-)
    else()
        target_compile_options(Engine PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endif()

# ========= Executável =========
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE Engine)

# Também “des-define” no executável, por segurança
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /U GLAD_GL_IMPLEMENTATION)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -UGLAD_GL_IMPLEMENTATION)
endif()

# Organização em IDEs (opcional)
source_group(TREE "${CMAKE_SOURCE_DIR}" FILES
    src/Application.cpp
    src/Shader.cpp
    src/Window.cpp
    includes/Application.h
    includes/Shader.h
    includes/Window.h
    main.cpp
)

message(STATUS "==== SimpleEngine ====")
message(STATUS "Using system GLAD : ${USE_SYSTEM_GLAD}")
message(STATUS "======================")
