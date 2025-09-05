#include "App.h"
#include <GLFW/glfw3.h>

App::App()
    : window_(800, 600, "SimpleEngine - Triangle") {}

int App::run() {
    while (!window_.shouldClose()) {
        // ESC to close
        if (window_.isKeyPressed(GLFW_KEY_ESCAPE)) {
            window_.requestClose();
        }

        renderer_.draw();

        window_.swapBuffers();
        window_.pollEvents();
    }
    return 0;
}
