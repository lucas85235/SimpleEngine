#include "App.h"
#include <GLFW/glfw3.h>
#include <SimpleEngine/Log.h>

App::App()
    : window_(800, 600, "Sandbox - OpenGL 3.3 Core") {

    se::LogInit(true);
    SE_LOG_INFO("Starting engine - build: {}", 0);

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

        window_.pollEvents();
        window_.swapBuffers();
    }
    return 0;
}
