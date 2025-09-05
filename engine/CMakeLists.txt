# Builds the engine as a library
add_library(simple_engine
    src/Window.cpp
    src/Renderer.cpp
)

target_include_directories(simple_engine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link as PUBLIC so apps can include GLFW headers if needed
target_link_libraries(simple_engine
    PUBLIC
        glfw
        OpenGL::GL
)

# Warnings (optional)
if (MSVC)
  target_compile_options(simple_engine PRIVATE /W4 /permissive-)
else()
  target_compile_options(simple_engine PRIVATE -Wall -Wextra -Wpedantic)
endif()
