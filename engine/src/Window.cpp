#include "engine/Window.h"
#include "se_pch.h"

namespace se {

Window::Window(uint32_t width, uint32_t height, const std::string& title)
    : width_(width), height_(height) {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    // Helpful error callback for debugging
    glfwSetErrorCallback([](int code, const char* desc) {
        std::cerr << "GLFW ERROR " << code << ": " << (desc ? desc : "<null>") << "\n";
    });

    // Não especificar API ainda
    // todo: custom condition for vulkan
    // glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);

    // Create window and context
    window_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window_) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    // glfwSetWindowUserPointer(window_, this);
    glfwSetFramebufferSizeCallback(window_, framebufferSizeCallback);
}

Window::Window(const ApplicationSpec& specification)
    : Window(specification.WindowWidth, specification.WindowHeight, specification.Name) {}

Window::~Window() {
    Destroy();
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(window_) != 0;
}

void Window::requestClose() const {
    glfwSetWindowShouldClose(window_, 1);
}

void Window::swapBuffers() const {
    //todo: only work with opengl
    glfwSwapBuffers(window_);
}

void Window::pollEvents() const {
    glfwPollEvents();
}

bool Window::isKeyPressed(int key) const {
    return glfwGetKey(window_, key) == GLFW_PRESS;
}
void Window::Destroy() {
    if (window_)
        glfwDestroyWindow(window_);
    window_ = nullptr;
}

void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    (void)window;
    glViewport(0, 0, width, height);
}

void Window::applyViewport(int width, int height) const {
    glViewport(0, 0, width, height);
}

void Window::setResizeCallback(std::function<void(int, int)> callback) {
    resizeCallback = callback;
}

} // namespace se
