#include <GLFW/glfw3.h>
#include <cstdio>

int main() {
    if (!glfwInit()) {
        std::fprintf(stderr, "Falha ao inicializar GLFW\n");
        return 1;
    }

    // Contexto OpenGL 3.3 Core (não vamos usar GL ainda, só criar janela)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(800, 600, "SimpleEngine - Janela", nullptr, nullptr);
    if (!window) {
        std::fprintf(stderr, "Falha ao criar janela GLFW\n");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // vsync

    while (!glfwWindowShouldClose(window)) {
        // Sem chamadas OpenGL aqui (mantemos o minimalismo)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
