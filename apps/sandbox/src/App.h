#pragma once
#include <engine/Window.h>
#include <engine/Renderer.h>

class App {
public:
    App();
    int run();

private:
    se::Window   window_;
    se::Renderer renderer_;
};
