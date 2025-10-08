#include "engine/renderer/SceneRenderer.h"
#include "engine/renderer/RenderCommand.h"
#include <gtc/type_ptr.hpp>

namespace se {
    SceneRenderer::SceneData *SceneRenderer::sceneData_ = nullptr;
    RenderStats SceneRenderer::stats_;

    void SceneRenderer::Init() {
        sceneData_ = new SceneData();
        RenderCommand::Init();
    }

    void SceneRenderer::Shutdown() {
        delete sceneData_;
        sceneData_ = nullptr;
    }

    void SceneRenderer::BeginScene(const Camera &camera, const glm::mat4 &projection) {
        sceneData_->ViewMatrix = camera.getViewMatrix();
        sceneData_->ProjectionMatrix = projection;
        sceneData_->ViewProjectionMatrix = projection * sceneData_->ViewMatrix;

        ResetStats();
    }

    void SceneRenderer::EndScene() {
        // Flush any remaining batched data here if needed
    }

    void SceneRenderer::Submit(const std::shared_ptr<VertexArray> &vertexArray,
                               const std::shared_ptr<Material> &material, const glm::mat4 &transform) {
        material->Bind();

        // Set transform matrices
        auto shader = material->GetShader();
        shader->setMat4("uView", sceneData_->ViewMatrix);
        shader->setMat4("uProj", sceneData_->ProjectionMatrix);
        shader->setMat4("uModel", transform);
        shader->setVec3("uLightDirection", sceneData_->LightDirection);

        // Draw
        RenderCommand::DrawIndexed(vertexArray.get());

        // Update stats
        stats_.DrawCalls++;
        stats_.TriangleCount += vertexArray->GetIndexBuffer()->GetCount() / 3;
    }

    void SceneRenderer::SetLightDirection(const glm::vec3 &direction) {
        if (!sceneData_) {
            return;
        }

        if (glm::length(direction) > 0.0f) {
            sceneData_->LightDirection = glm::normalize(direction);
        } else {
            sceneData_->LightDirection = glm::vec3(0.0f, 0.0f, 1.0f);
        }
    }

    const glm::vec3 &SceneRenderer::GetLightDirection() {
        static const glm::vec3 fallback(0.0f, 0.0f, 1.0f);
        return sceneData_ ? sceneData_->LightDirection : fallback;
    }
} // namespace se
