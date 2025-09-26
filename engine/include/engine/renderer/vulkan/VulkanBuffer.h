#pragma once
#include <engine/renderer/rhi/IBuffer.h>

class VulkanBuffer : public IBuffer {
public:
    VulkanBuffer(const BufferDesc& desc);
    ~VulkanBuffer() override;

    void Update(const void* data, uint32_t size, uint32_t offset) override;
    const BufferDesc& GetDesc() const override { return m_Desc; }

private:
    BufferDesc m_Desc;
    // No código real, aqui teríamos: GLuint m_RendererID;
};

