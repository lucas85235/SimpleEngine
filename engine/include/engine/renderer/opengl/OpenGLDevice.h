#pragma once

#include <engine/renderer/rhi/IGraphicsDevice.h>

struct GLFWwindow;

class OpenGLDevice : public IGraphicsDevice {
public:
    OpenGLDevice();
    ~OpenGLDevice() override;

    std::unique_ptr<IBuffer> CreateBuffer(const BufferDesc& desc) override;

    // Função para o loop principal da aplicação
    bool IsWindowOpen() const;
    void BeginFrame();
    void EndFrame();

private:
    GLFWwindow* window_ = nullptr;
    int width_ = 800;
    int height_ = 600;

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    void applyViewport(int width, int height) const;
};
