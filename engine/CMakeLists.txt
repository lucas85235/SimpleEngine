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

FetchContent_Declare(spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.x
)
FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG v1.89.8 # ajuste a versão se preferir um tag mais recente
)

FetchContent_GetProperties(imgui)
if (NOT imgui_POPULATED)
  FetchContent_Populate(imgui)
endif()

add_library(imgui STATIC
  ${imgui_SOURCE_DIR}/imgui.cpp
  ${imgui_SOURCE_DIR}/imgui_draw.cpp
  ${imgui_SOURCE_DIR}/imgui_tables.cpp
  ${imgui_SOURCE_DIR}/imgui_widgets.cpp
  ${imgui_SOURCE_DIR}/imgui_demo.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
  ${imgui_SOURCE_DIR}
  ${imgui_SOURCE_DIR}/backends
)

target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)
target_link_libraries(imgui PUBLIC glfw glad ${OPENGL_gl_LIBRARY})

# Constrói a biblioteca da engine, SEM o código da GLAD.
add_library(simple_engine
    src/Window.cpp
    src/Renderer.cpp
    src/Shader.cpp
    src/Camera.cpp
    src/Log.cpp
    src/InputHandler.cpp
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
        spdlog::spdlog 
        spdlog::spdlog_header_only
)

# Adiciona definições de compilação necessárias
target_compile_definitions(simple_engine PUBLIC GLFW_INCLUDE_NONE)
