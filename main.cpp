#include <cstdio>
#include <GLFW/glfw3.h>

#ifdef __APPLE__
  #include <OpenGL/gl.h>
#else
  #include <GL/gl.h>
#endif

static void framebuffer_size_callback(GLFWwindow* /*window*/, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    if (!glfwInit()) {
        std::fprintf(stderr, "Falha ao inicializar GLFW\n");
        return 1;
    }

    // NÃO forçar Core Profile 3.3 – usamos compatibilidade (immediate mode)
    // Se quiser garantir 2.1:
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    GLFWwindow* window = glfwCreateWindow(800, 600, "SimpleEngine - Triângulo", nullptr, nullptr);
    if (!window) {
        std::fprintf(stderr, "Falha ao criar janela GLFW\n");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // vsync
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Define viewport inicial
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    glViewport(0, 0, w, h);

    while (!glfwWindowShouldClose(window)) {
        // ESC para fechar
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, 1);
        }

        glClearColor(0.08f, 0.10f, 0.14f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Desenha um triângulo usando pipeline fixo (immediate mode)
        glBegin(GL_TRIANGLES);
            glColor3f(1.f, 0.f, 0.f);  glVertex2f(-0.6f, -0.5f);
            glColor3f(0.f, 1.f, 0.f);  glVertex2f( 0.6f, -0.5f);
            glColor3f(0.f, 0.f, 1.f);  glVertex2f( 0.0f,  0.6f);
        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
