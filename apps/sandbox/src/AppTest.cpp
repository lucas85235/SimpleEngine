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
    SE_LOG_INFO("Starting engine");

    // Initialize renderer
    // renderer_.init();
}

int App::run()
{
    /*
    GraphicsAPI api = GraphicsAPI::OpenGL;

    std::cout << "========================================\n";
    std::cout << "Iniciando engine com " << (api == GraphicsAPI::OpenGL ? "OpenGL" : "Vulkan") << std::endl;

    std::unique_ptr<IGraphicsDevice> device = RendererFactory::CreateDevice(api);

    if (!device) {
        std::cerr << "Falha ao criar o GraphicsDevice!" << std::endl;
        // return;
    }
    device->
    while (!api->IsWindowOpen()) {
        // Poll and handle events (inputs, window resize, etc.)
        window_.pollEvents();

        // Start the Dear ImGui frame
        // ImGui_ImplOpenGL3_NewFrame();
        // ImGui_ImplGlfw_NewFrame();
        // ImGui::NewFrame();

        // Rendering
        // ImGui::Render();
        window_.swapBuffers();
    }

    // BufferDesc vertexBufferDesc;
    // vertexBufferDesc.size = 1024; // 1KB
    // vertexBufferDesc.usage = BufferDesc::Usage::Vertex;

    // std::unique_ptr<IBuffer> vertexBuffer = device->CreateBuffer(vertexBufferDesc);

    // 3. O código da engine funcionaria com esses objetos...
    // vertexBuffer->Update(...);

    std::cout << "Engine desligando." << std::endl;
    // 4. Graças ao std::unique_ptr, a memória é liberada automaticamente
    // na ordem correta quando saímos do escopo. Os destrutores serão chamados.
    std::cout << "========================================\n\n";
    */

    std::unique_ptr<OpenGLDevice> device;

    try {
        device = std::make_unique<OpenGLDevice>();
    } catch (const std::exception& e) {
        std::cerr << "Erro na inicialização: " << e.what() << std::endl;
        return -1;
    }

    // --- Teste de criação de buffer ---
    struct Vertex {
        float position[3];
        float color[3];
    };

    std::vector<Vertex> vertices = {
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{ 0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{ 0.0f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}
    };

    BufferDesc vertexBufferDesc;
    vertexBufferDesc.size = vertices.size() * sizeof(Vertex);
    vertexBufferDesc.usage = BufferDesc::Usage::Vertex;
    vertexBufferDesc.initialData = vertices.data();

    auto vertexBuffer = device->CreateBuffer(vertexBufferDesc);
    // ------------------------------------

    // Loop principal da aplicação
    while (device->IsWindowOpen()) {
        device->BeginFrame();

        //
        // TODO: Comandos de renderização viriam aqui
        // Por exemplo: BindShader(), BindVertexBuffer(), Draw()...
        //

        device->EndFrame();
    }


    return 0;
}
