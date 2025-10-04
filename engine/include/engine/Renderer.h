#pragma once
#include <engine/Window.h>

namespace se {

class Renderer {
  public:
    virtual ~Renderer() = default;
    virtual void Initialize(Window& window) = 0;
    virtual void Render(float delta_time) = 0;
    virtual void Cleanup() = 0;
    virtual void OnResize(int width, int height) {}
};

} // namespace se
