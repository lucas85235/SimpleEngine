#include "engine/Log.h"
#include <engine/renderer/opengl/OpenGLDevice.h>
#include <engine/renderer/opengl/OpenGLBuffer.h>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

OpenGLDevice::OpenGLDevice() {
    std::cout << "OpenGLDevice criado e inicializado!" << std::endl;

    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    // Helpful error callback for debugging
    glfwSetErrorCallback([](int code, const char* desc) {
        SE_LOG_ERROR("GLFW ERROR {}: {}", code, (desc ? desc : "<null>"));
    });

    // Request a modern OpenGL context (3.3 core)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create window and context
    window_ = glfwCreateWindow(width_, height_, "Sandbox", nullptr, nullptr);
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
}

OpenGLDevice::~OpenGLDevice() {
    std::cout << "OpenGLDevice destruído!" << std::endl;
    
    if (window_) {
        glfwDestroyWindow(window_);
        window_ = nullptr;
    }
    
    glfwTerminate();
}

std::unique_ptr<IBuffer> OpenGLDevice::CreateBuffer(const BufferDesc& desc) {
    return std::make_unique<OpenGLBuffer>(desc);
}

bool OpenGLDevice::IsWindowOpen() const {
    return !glfwWindowShouldClose(window_);
}

void OpenGLDevice::BeginFrame() {
    // Process window events (keyboard, mouse, etc.)
    glfwPollEvents();

    // Clean the screen to a dark blueish color
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLDevice::EndFrame() {
    // Swap the front and back buffers to display the rendered image
    glfwSwapBuffers(window_);
}

void OpenGLDevice::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    (void)window;
    glViewport(0, 0, width, height);
}

void OpenGLDevice::applyViewport(int width, int height) const {
    glViewport(0, 0, width, height);
}
