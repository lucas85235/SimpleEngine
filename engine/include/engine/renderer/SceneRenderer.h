#pragma once

#include "engine/Camera.h"
#include "engine/renderer/Material.h"
#include "engine/renderer/VertexArray.h"
#include <glm.hpp>
#include <memory>

namespace se {

struct RenderStats {
    uint32_t DrawCalls = 0;
    uint32_t TriangleCount = 0;

    void Reset() {
        DrawCalls = 0;
        TriangleCount = 0;
    }
};

class SceneRenderer {
  public:
    static void Init();
    static void Shutdown();

    static void BeginScene(const Camera& camera, const glm::mat4& projection);
    static void EndScene();

    static void Submit(const std::shared_ptr<VertexArray>& vertexArray,
                       const std::shared_ptr<Material>& material,
                       const glm::mat4& transform = glm::mat4(1.0f));

    static RenderStats GetStats() {
        return stats_;
    }
    static void ResetStats() {
        stats_.Reset();
    }

    static void SetLightDirection(const glm::vec3& direction);
    static const glm::vec3& GetLightDirection();

  private:
    struct SceneData {
        glm::mat4 ViewMatrix;
        glm::mat4 ProjectionMatrix;
        glm::mat4 ViewProjectionMatrix;
        glm::vec3 LightDirection = glm::vec3(0.0f, 0.0f, 1.0f);
    };

    static SceneData* sceneData_;
    static RenderStats stats_;
};

} // namespace se