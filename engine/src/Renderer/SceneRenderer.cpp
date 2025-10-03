#include "engine/renderer/SceneRenderer.h"
#include "engine/renderer/RenderCommand.h"
#include <gtc/type_ptr.hpp>

namespace se {

    SceneRenderer::SceneData* SceneRenderer::sceneData_ = nullptr;
    RenderStats SceneRenderer::stats_;

    void SceneRenderer::Init() {
        sceneData_ = new SceneData();
        RenderCommand::Init();
    }

    void SceneRenderer::Shutdown() {
        delete sceneData_;
        sceneData_ = nullptr;
    }

    void SceneRenderer::BeginScene(const Camera& camera, const glm::mat4& projection) {
        sceneData_->ViewMatrix = camera.getViewMatrix();
        sceneData_->ProjectionMatrix = projection;
        sceneData_->ViewProjectionMatrix = projection * sceneData_->ViewMatrix;

        ResetStats();
    }

    void SceneRenderer::EndScene() {
        // Flush any remaining batched data here if needed
    }

    void SceneRenderer::Submit(const std::shared_ptr<VertexArray>& vertexArray,
                               const std::shared_ptr<Material>& material,
                               const glm::mat4& transform) {
        material->Bind();

        // Set transform matrices
        auto shader = material->GetShader();
        shader->setMat4("uView", sceneData_->ViewMatrix);
        shader->setMat4("uProj", sceneData_->ProjectionMatrix);
        shader->setMat4("uModel", transform);

        // Draw
        RenderCommand::DrawIndexed(vertexArray.get());

        // Update stats
        stats_.DrawCalls++;
        stats_.TriangleCount += vertexArray->GetIndexBuffer()->GetCount() / 3;
    }

} // namespace se