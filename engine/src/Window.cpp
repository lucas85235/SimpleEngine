#include <engine/Window.h>
#include <stdexcept>
#include <cstdio>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace se {

Window::Window(int width, int height, const std::string& title)
{
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    // Helpful error callback for debugging
    glfwSetErrorCallback([](int code, const char* desc) {
        std::cerr << "GLFW ERROR " << code << ": " << (desc ? desc : "<null>") << "\n";
    });

    // Request a modern OpenGL context (3.3 core).
    // If you want a different version, change these values.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create window and context
    window_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window_) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    // Make the context current BEFORE loading GL function pointers
    glfwMakeContextCurrent(window_);

    // Initialize GLAD (must be done after context creation)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwDestroyWindow(window_);
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLAD");
    }

    // Vsync
    glfwSwapInterval(1);

    // Framebuffer callback (keeps viewport correct)
    glfwSetFramebufferSizeCallback(window_, framebufferSizeCallback);

    // Setup initial viewport dimensions
    int fbw = 0, fbh = 0;
    glfwGetFramebufferSize(window_, &fbw, &fbh);
    applyViewport(fbw, fbh);

    // Leave input mode as normal; input capture/RAW should be handled by Renderer
    // Example (do not enable by default): glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

Window::~Window() {
    if (window_) {
        glfwDestroyWindow(window_);
        window_ = nullptr;
    }
    glfwTerminate();
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(window_) != 0;
}

void Window::requestClose() const {
    glfwSetWindowShouldClose(window_, 1);
}

void Window::swapBuffers() const {
    glfwSwapBuffers(window_);
}

void Window::pollEvents() const {
    glfwPollEvents();
}

bool Window::isKeyPressed(int key) const {
    return glfwGetKey(window_, key) == GLFW_PRESS;
}

void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    (void)window;
    glViewport(0, 0, width, height);
}

void Window::applyViewport(int width, int height) const {
    glViewport(0, 0, width, height);
}

} // namespace se
