#pragma once
#include <engine/Renderer.h>

namespace se {

class OpenGLRenderer : public Renderer {
public:
    void initialize(Window& window);
    void render();
    void cleanup();
    void onResize(int width, int height);
    
private:
    Window* currentWindow = nullptr;
};

} // namespace se
