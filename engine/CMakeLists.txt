# /engine/CMakeLists.txt

# --- DEPENDÊNCIAS DE TERCEIROS ---
# Esta parte não muda
include(FetchContent)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG        3.4
)
FetchContent_MakeAvailable(glfw)

find_package(OpenGL REQUIRED)
find_package(glm CONFIG REQUIRED)

# --- BIBLIOTECAS INTERNAS ---

# [RESTAURADO] Constrói GLAD como a sua própria biblioteca estática isolada.
# Isto força o 'make' a completar este passo antes de avançar,
# o que irá expor o erro corretamente ou contornar o problema do filesystem.
add_library(glad STATIC
    "libs/glad/src/glad.c"
)
target_include_directories(glad PUBLIC
    "libs/glad/include"
)

# Constrói a biblioteca da engine, SEM o código da GLAD.
add_library(simple_engine
    src/Window.cpp
    src/Renderer.cpp
    src/Shader.cpp
    src/Camera.cpp
    # Adicione aqui todos os outros ficheiros .cpp da sua engine
)
target_include_directories(simple_engine PUBLIC
    "include"
)

# --- LIGAÇÃO DAS DEPENDÊNCIAS DA ENGINE ---

# Liga a sua engine às suas dependências.
# Agora voltamos a ligar explicitamente ao ALVO 'glad'.
target_link_libraries(simple_engine
    PUBLIC
        glad # Ligação ao alvo 'glad' restaurada
        glfw
        OpenGL::GL
        glm::glm
)

# Adiciona definições de compilação necessárias
target_compile_definitions(simple_engine PUBLIC GLFW_INCLUDE_NONE)
