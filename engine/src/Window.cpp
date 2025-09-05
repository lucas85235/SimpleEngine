#include "Window.h"
#include <stdexcept>
#include <cstdio>
#include <GLFW/glfw3.h>

#ifdef __APPLE__
  #include <OpenGL/gl.h>
#else
  #include <GL/gl.h>
#endif

Window::Window(int width, int height, const std::string& title)
    : width_(width), height_(height) {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    // Use an OpenGL 2.1 compatible context (immediate mode friendly)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    handle_ = glfwCreateWindow(width_, height_, title.c_str(), nullptr, nullptr);
    if (!handle_) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(handle_);
    glfwSwapInterval(1); // vsync
    glfwSetFramebufferSizeCallback(handle_, framebufferSizeCallback);

    // Setup initial viewport
    int fbw = 0, fbh = 0;
    glfwGetFramebufferSize(handle_, &fbw, &fbh);
    applyViewport(fbw, fbh);
}

Window::~Window() {
    if (handle_) {
        glfwDestroyWindow(handle_);
        handle_ = nullptr;
    }
    glfwTerminate();
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(handle_) != 0;
}

void Window::requestClose() const {
    glfwSetWindowShouldClose(handle_, 1);
}

void Window::swapBuffers() const {
    glfwSwapBuffers(handle_);
}

void Window::pollEvents() const {
    glfwPollEvents();
}

bool Window::isKeyPressed(int key) const {
    return glfwGetKey(handle_, key) == GLFW_PRESS;
}

void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    // Retrieve our Window instance if needed; here we just update the viewport
    (void)window;
    glViewport(0, 0, width, height);
}

void Window::applyViewport(int width, int height) const {
    glViewport(0, 0, width, height);
}
