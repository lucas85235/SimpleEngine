#pragma once
#include "engine/Layer.h"

class AppLayer : public se::Layer {
  public:
    AppLayer();

    ~AppLayer() override;

    virtual void OnEvent(se::Event& event) override;

    virtual void OnUpdate(float ts) override;
    virtual void OnRender() override;
};
