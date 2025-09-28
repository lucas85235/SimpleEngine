#pragma once
#include "window.h"
#include "renderer.h"
#include <memory>

class App {
public:
    App(int width, int height, const std::string& title);
    ~App();
    
    void setRenderer(std::unique_ptr<Renderer> renderer);
    void run();
    
private:
    std::unique_ptr<Window> window;
    std::unique_ptr<Renderer> renderer;
};