#include <engine/OpenGLRenderer.h>
#include <GLFW/glfw3.h>
#include <iostream>

namespace se {

void OpenGLRenderer::initialize(Window& window) {
    currentWindow = &window;
    
    // Configurar contexto OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Recriar janela com contexto OpenGL
    GLFWwindow* handle = window.native();
    glfwMakeContextCurrent(handle);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwDestroyWindow(window.native());
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLAD");
    }

    // Configurações iniciais
    glViewport(0, 0, 800, 600);
    glClearColor(1.0f, 0.2f, 0.2f, 1.0f);
    
    std::cout << "OpenGL " << glGetString(GL_VERSION) << " inicializado" << std::endl;
}

void OpenGLRenderer::render() {
    glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLRenderer::cleanup() {
    // Limpeza específica do OpenGL se necessário
}

void OpenGLRenderer::onResize(int width, int height) {
    glViewport(0, 0, width, height);
}

} // namespace se
