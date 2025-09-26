#pragma once
#include <memory>
#include "../rhi/IGraphicsDevice.h"
#include "../opengl/OpenGLDevice.h" // Incluímos os headers concretos aqui
#include "../vulkan/VulkanDevice.h" // para poder criá-los.

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
