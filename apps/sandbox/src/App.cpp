#include "App.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h> // not strictly necessary here, but harmless

App::App()
    : window_(800, 600, "Sandbox - OpenGL 3.3 Core") {
    renderer_.init();
}

int App::run() {
    double start = glfwGetTime();
    while (!window_.shouldClose()) {
        if (window_.isKeyPressed(GLFW_KEY_ESCAPE)) {
            window_.requestClose();
        }
        double t = glfwGetTime() - start;
        renderer_.draw(static_cast<float>(t));

        window_.swapBuffers();
        window_.pollEvents();
    }
    return 0;
}
