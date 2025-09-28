#include "app.h"
#include <iostream>

App::App(int width, int height, const std::string& title) 
    : window(std::make_unique<Window>(width, height, title)) {
}

App::~App() = default;

void App::setRenderer(std::unique_ptr<Renderer> r) {
    renderer = std::move(r);
    
    // Configurar callback de redimensionamento
    window->setResizeCallback([this](int w, int h) {
        if (renderer) {
            renderer->onResize(w, h);
        }
    });
    
    renderer->initialize(*window);
}

void App::run() {
    if (!renderer) {
        throw std::runtime_error("Nenhum renderer definido");
    }
    
    while (!window->shouldClose()) {
        window->pollEvents();
        renderer->render();
        window->swapBuffers();
    }
    
    renderer->cleanup();
}