#pragma once
#include <engine/renderer/rhi/IGraphicsDevice.h>

class VulkanDevice : public IGraphicsDevice {
public:
    VulkanDevice();
    ~VulkanDevice() override;

    std::unique_ptr<IBuffer> CreateBuffer(const BufferDesc& desc) override;
};
