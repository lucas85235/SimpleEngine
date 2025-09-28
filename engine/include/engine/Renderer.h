#pragma once
#include <engine/Window.h>

namespace se {

class Renderer {
public:
    virtual ~Renderer() = default;
    virtual void initialize(Window& window) = 0;
    virtual void render() = 0;
    virtual void cleanup() = 0;
    virtual void onResize(int width, int height) {}
};

} // namespace se
