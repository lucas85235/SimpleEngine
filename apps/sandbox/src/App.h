#pragma once
#include <memory>
#include <engine/Window.h>
#include <engine/Renderer.h>

class App {
public:
    App();
    int run();

private:
    std::unique_ptr<se::Window> window_;
    std::unique_ptr<se::Renderer> render_;
};
