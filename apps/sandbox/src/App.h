#pragma once
#include <SimpleEngine/Window.h>
#include <SimpleEngine/Renderer.h>

class App {
public:
    App();
    int run();

private:
    se::Window   window_;
    se::Renderer renderer_;
};
