cmake_minimum_required(VERSION 3.16)

project(SimpleEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Fetch GLFW without extras
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

add_executable(SimpleEngine
  src/main.cpp
  src/App.cpp
  src/Window.cpp
  src/Renderer.cpp
)

target_include_directories(SimpleEngine PRIVATE include)

target_link_libraries(SimpleEngine
  PRIVATE
    glfw
    OpenGL::GL
)

# Warnings (optional)
if (MSVC)
  target_compile_options(SimpleEngine PRIVATE /W4 /permissive-)
else()
  target_compile_options(SimpleEngine PRIVATE -Wall -Wextra -Wpedantic)
endif()
