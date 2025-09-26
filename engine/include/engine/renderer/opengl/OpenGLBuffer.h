#pragma once

#include <engine/renderer/rhi/IBuffer.h>

class OpenGLBuffer : public IBuffer {
public:
    OpenGLBuffer(const BufferDesc& desc);
    ~OpenGLBuffer() override;

    void Update(const void* data, uint32_t size, uint32_t offset) override;
    const BufferDesc& GetDesc() const override { return m_Desc; }

private:
    BufferDesc m_Desc;
    // No código real, aqui teríamos: GLuint m_RendererID;
};
