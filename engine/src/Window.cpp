#include <engine/Window.h>
#include <stdexcept>
#include <cstdio>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace se {

Window::Window(int width, int height, const std::string& title) {
    if (!glfwInit()) {
        throw std::runtime_error("Falha ao inicializar GLFW");
    }
    
    // Não especificar API ainda
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    
    window_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window_) {
        glfwTerminate();
        throw std::runtime_error("Falha ao criar janela");
    }
    
    glfwSetWindowUserPointer(window_, this);
    glfwSetFramebufferSizeCallback(window_, framebufferSizeCallback);
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

void Window::setResizeCallback(std::function<void(int, int)> callback) {
    resizeCallback = callback;
}

void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    (void)window;
    glViewport(0, 0, width, height);
}

void Window::applyViewport(int width, int height) const {
    glViewport(0, 0, width, height);
}

} // namespace se
