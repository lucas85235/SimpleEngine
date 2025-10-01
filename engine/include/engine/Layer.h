#pragma once
#include "engine/Event.h"

namespace se {

class Layer {
  public:
    virtual ~Layer() = default;

    virtual void OnEvent(Event& event) {}

    virtual void OnUpdate(float ts) {}
    virtual void OnRender() {}
};
} // namespace se
