#pragma once
#include "renderer.h"

class OpenGLRenderer : public Renderer {
public:
    void initialize(Window& window) override;
    void render() override;
    void cleanup() override;
    void onResize(int width, int height) override;
    
private:
    Window* currentWindow = nullptr;
};
