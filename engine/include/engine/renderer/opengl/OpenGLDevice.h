#pragma once

#include <engine/renderer/rhi/IGraphicsDevice.h>

class OpenGLDevice : public IGraphicsDevice {
public:
    OpenGLDevice();
    ~OpenGLDevice() override;

    std::unique_ptr<IBuffer> CreateBuffer(const BufferDesc& desc) override;
};
