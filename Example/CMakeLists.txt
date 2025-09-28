cmake_minimum_required(VERSION 3.16)
project(GraphicsAbstraction)

set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(Vulkan REQUIRED)

add_executable(graphics_app
    main.cpp
    app.cpp
    window.cpp
    opengl_renderer.cpp
    vulkan_renderer.cpp
)

target_link_libraries(graphics_app 
    glfw 
    OpenGL::GL 
    Vulkan::Vulkan
)