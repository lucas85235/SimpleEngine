#include "engine/Renderer.h"
#include "engine/Log.h"

namespace se {

    Renderer::Renderer() {}

    Renderer::~Renderer() {
        Shutdown();
    }

    void Renderer::Init() {
        if (initialized_) {
            SE_LOG_WARN("Renderer already initialized");
            return;
        }

        SE_LOG_INFO("Initializing Renderer");

        // Initialize low-level rendering systems
        RenderCommand::Init();
        SceneRenderer::Init();

        initialized_ = true;
        SE_LOG_INFO("Renderer initialized successfully");
    }

    void Renderer::Shutdown() {
        if (!initialized_)
            return;

        SE_LOG_INFO("Shutting down Renderer");

        SceneRenderer::Shutdown();

        initialized_ = false;
    }

    void Renderer::BeginFrame() {
        // Any per-frame setup can go here
    }

    void Renderer::EndFrame() {
        // Any per-frame cleanup can go here
    }

    void Renderer::Clear() {
        RenderCommand::Clear();
    }

    void Renderer::SetClearColor(float r, float g, float b, float a) {
        RenderCommand::SetClearColor({r, g, b, a});
    }

    void Renderer::BeginScene(const Camera& camera, float aspectRatio) {
        glm::mat4 projection = camera.getProjectionMatrix(aspectRatio);
        SceneRenderer::BeginScene(camera, projection);
    }

    void Renderer::EndScene() {
        SceneRenderer::EndScene();
    }

} // namespace se