#pragma once
#include "Window.h"
#include "Renderer.h"

class App {
public:
    App();
    int run();

private:
    Window   window_;
    Renderer renderer_;
};
