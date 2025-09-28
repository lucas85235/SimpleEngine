#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main() {
    // Inicializar GLFW
    if (!glfwInit()) {
        std::cerr << "Falha ao inicializar GLFW" << std::endl;
        return -1;
    }

    // Configurar versão OpenGL (3.3 Core Profile)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Criar janela
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Mínimo", nullptr, nullptr);
    if (!window) {
        std::cerr << "Falha ao criar janela" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Tornar contexto atual
    glfwMakeContextCurrent(window);

    // Inicializar GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Falha ao inicializar GLEW" << std::endl;
        return -1;
    }

    // Configurar viewport
    glViewport(0, 0, 800, 600);

    // Loop principal
    while (!glfwWindowShouldClose(window)) {
        // Processar eventos
        glfwPollEvents();

        // Limpar tela com cor azul
        glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Trocar buffers
        glfwSwapBuffers(window);
    }

    // Limpeza
    glfwTerminate();
    return 0;
}

// Compilar com:
// g++ -o opengl_minimal opengl_minimal.cpp -lGL -lGLEW -lglfw