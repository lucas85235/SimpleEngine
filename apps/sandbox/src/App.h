#pragma once
#include <memory>
#include <engine/VulkanApp.h>
#include <engine/Window.h>
#include <engine/OldRenderer.h>
#include <engine/Renderer.h>

class App {
public:
    App();
    int run();

private:
    std::unique_ptr<se::Window> window_;
    se::OldRenderer renderer_;
    std::unique_ptr<se::Renderer> render_;
    VulkanApp vulkan_;
};
