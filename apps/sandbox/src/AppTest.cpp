#include "App.h"
// #include <GLFW/glfw3.h>
#include <engine/Log.h>
#include <engine/renderer/rhi/RendererFactory.h>

// ImGui headers
// #include "imgui.h"
// #include "backends/imgui_impl_glfw.h"
// #include "backends/imgui_impl_opengl3.h"

#include <iostream>

App::App()
    : window_(800, 600, "Sandbox - OpenGL 3.3 Core")
{

    // Initialize logging
    se::LogInit(true);
    SE_LOG_INFO("Starting engine - build: {}", 0);

    // Initialize renderer
    renderer_.init();

    
}

int App::run()
{
    GraphicsAPI api = GraphicsAPI::OpenGL;

    std::cout << "========================================\n";
    std::cout << "Iniciando engine com " << (api == GraphicsAPI::OpenGL ? "OpenGL" : "Vulkan") << std::endl;

    // 1. Usamos a fábrica para criar o device.
    // A variável 'device' é do tipo da interface, não da classe concreta!
    std::unique_ptr<IGraphicsDevice> device = RendererFactory::CreateDevice(api);

    if (!device) {
        std::cerr << "Falha ao criar o GraphicsDevice!" << std::endl;
        // return;
    }

    // 2. Usamos o device para criar um buffer.
    // Novamente, 'vertexBuffer' é um ponteiro para a interface IBuffer.
    // Não sabemos (e não nos importamos) se é um OpenGLBuffer ou VulkanBuffer.
    BufferDesc vertexBufferDesc;
    vertexBufferDesc.size = 1024; // 1KB
    vertexBufferDesc.usage = BufferDesc::Usage::Vertex;

    std::unique_ptr<IBuffer> vertexBuffer = device->CreateBuffer(vertexBufferDesc);

    // 3. O código da engine funcionaria com esses objetos...
    // vertexBuffer->Update(...);

    std::cout << "Engine desligando." << std::endl;
    // 4. Graças ao std::unique_ptr, a memória é liberada automaticamente
    // na ordem correta quando saímos do escopo. Os destrutores serão chamados.
    std::cout << "========================================\n\n";

    return 0;
}
