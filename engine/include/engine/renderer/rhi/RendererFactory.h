#pragma once

#include <engine/renderer/rhi/IGraphicsDevice.h>
#include <engine/renderer/opengl/OpenGLDevice.h>
#include <engine/renderer/vulkan/VulkanDevice.h>

enum class GraphicsAPI {
    OpenGL,
    Vulkan
};

class RendererFactory {
public:
    static std::unique_ptr<IGraphicsDevice> CreateDevice(GraphicsAPI api) {
        if (api == GraphicsAPI::OpenGL) {
            return std::make_unique<OpenGLDevice>();
        } else if (api == GraphicsAPI::Vulkan) {
            return std::make_unique<VulkanDevice>();
        }
        return nullptr;
    }
};
