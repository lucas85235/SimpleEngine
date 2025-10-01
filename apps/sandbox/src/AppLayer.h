#pragma once
#include "engine/Layer.h"

class AppLayer : se::Layer {
public:
  AppLayer();
  virtual ~AppLayer();

  virtual void OnEvent(se::Event &event) override;

  virtual void OnUpdate(float ts) override;
  virtual void OnRender() override;
};
