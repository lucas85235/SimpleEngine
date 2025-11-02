#include "engine/renderer/SceneRenderer.h"
#include "engine/renderer/RenderCommand.h"
#include <glad/glad.h>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

namespace {
constexpr const char* kShadowVertexSource = R"(#version 330 core
layout(location = 0) in vec3 a_Position;

uniform mat4 uLightSpaceMatrix;
uniform mat4 uModel;

void main() {
    vec4 new_pos = uLightSpaceMatrix * uModel * vec4(a_Position, 1.0);
    gl_Position = new_pos;
}
)";

constexpr const char* kShadowFragmentSource = R"(#version 330 core
void main() {
    // depth only
}
)";
} // namespace

namespace se {
SceneRenderer::SceneData* SceneRenderer::sceneData_ = nullptr;
RenderStats SceneRenderer::stats_;

void SceneRenderer::Init() {
    sceneData_ = new SceneData();
    InitializeShadowResources();
}

void SceneRenderer::Shutdown() {
    DestroyShadowResources();
    delete sceneData_;
    sceneData_ = nullptr;
}

void SceneRenderer::BeginScene(const Camera& camera, const glm::mat4& projection) {
    sceneData_->ViewMatrix = camera.getViewMatrix();
    sceneData_->ProjectionMatrix = projection;
    sceneData_->ViewProjectionMatrix = projection * sceneData_->ViewMatrix;
    sceneData_->Submissions.clear();

    // Prepare directional light data and shadow matrix
    if (!sceneData_->DirectionalLight.Active) {
        sceneData_->DirectionalLight.Direction = glm::vec3(0.0f, -1.0f, 0.0f);
        sceneData_->DirectionalLight.Intensity = 0.0f;
        sceneData_->DirectionalLight.Color = glm::vec3(1.0f);
        sceneData_->ShadowsEnabled = false;
        sceneData_->LightSpaceMatrix = glm::mat4(1.0f);
    } else {
        glm::vec3 lightDir = sceneData_->DirectionalLight.Direction;
        if (glm::length(lightDir) <= 0.0f) {
            lightDir = glm::vec3(0.0f, -1.0f, 0.0f);
        }
        lightDir = glm::normalize(lightDir);
        sceneData_->DirectionalLight.Direction = lightDir;

        sceneData_->ShadowsEnabled = sceneData_->DirectionalLight.CastShadows &&
                                     sceneData_->DirectionalLight.Intensity > 0.0f;

        if (sceneData_->ShadowsEnabled) {
            const glm::vec3 focusPoint = sceneData_->DirectionalLight.Position;
            const glm::vec3 lightPos = focusPoint - lightDir * sceneData_->ShadowDistance;
            glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
            if (glm::abs(glm::dot(up, lightDir)) > 0.95f) {
                up = glm::vec3(0.0f, 0.0f, 1.0f);
            }
            glm::mat4 lightView = glm::lookAt(lightPos, focusPoint, up);
            glm::mat4 lightProj =
                glm::ortho(-sceneData_->ShadowOrthoSize, sceneData_->ShadowOrthoSize,
                           -sceneData_->ShadowOrthoSize, sceneData_->ShadowOrthoSize, 0.1f,
                           sceneData_->ShadowDistance * 2.0f);
            sceneData_->LightSpaceMatrix = lightProj * lightView;
        } else {
            sceneData_->LightSpaceMatrix = glm::mat4(1.0f);
        }
    }

    ResetStats();
}

void SceneRenderer::EndScene() {
    if (!sceneData_)
        return;

    if (sceneData_->ShadowsEnabled) {
        RenderShadowPass();
    }

    RenderScenePass();
}

void SceneRenderer::Submit(const std::shared_ptr<VertexArray>& vertexArray,
                           const std::shared_ptr<Material>& material, const glm::mat4& transform,
                           bool castsShadows, bool receiveShadows) {
    if (!sceneData_)
        return;

    Submission submission;
    submission.VertexArray = vertexArray;
    submission.Material = material;
    submission.Transform = transform;
    submission.CastsShadows = castsShadows;
    submission.ReceiveShadows = receiveShadows;
    sceneData_->Submissions.emplace_back(std::move(submission));
}

void SceneRenderer::SetDirectionalLight(const DirectionalLightData& light) {
    if (!sceneData_)
        return;

    sceneData_->DirectionalLight = light;
    sceneData_->DirectionalLight.Active = true;

    if (glm::length(sceneData_->DirectionalLight.Direction) <= 0.0f) {
        sceneData_->DirectionalLight.Direction = glm::vec3(0.0f, -1.0f, 0.0f);
    } else {
        sceneData_->DirectionalLight.Direction =
            glm::normalize(sceneData_->DirectionalLight.Direction);
    }

    sceneData_->DirectionalLight.Intensity = glm::max(light.Intensity, 0.0f);
}

void SceneRenderer::ClearDirectionalLight() {
    if (!sceneData_)
        return;

    sceneData_->DirectionalLight = DirectionalLightData{};
    sceneData_->ShadowsEnabled = false;
    sceneData_->LightSpaceMatrix = glm::mat4(1.0f);
}

SceneRenderer::DirectionalLightData SceneRenderer::GetDirectionalLight() {
    if (!sceneData_)
        return DirectionalLightData{};

    return sceneData_->DirectionalLight;
}

void SceneRenderer::InitializeShadowResources() {
    if (!sceneData_)
        return;

    sceneData_->ShadowShader = std::make_shared<Shader>(kShadowVertexSource, kShadowFragmentSource);

    glGenFramebuffers(1, &sceneData_->ShadowFramebuffer);
    glGenTextures(1, &sceneData_->ShadowDepthTexture);

    glBindTexture(GL_TEXTURE_2D, sceneData_->ShadowDepthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, sceneData_->ShadowMapSize.x,
                 sceneData_->ShadowMapSize.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    const float borderColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, sceneData_->ShadowFramebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                           sceneData_->ShadowDepthTexture, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SceneRenderer::DestroyShadowResources() {
    if (!sceneData_)
        return;

    if (sceneData_->ShadowDepthTexture) {
        glDeleteTextures(1, &sceneData_->ShadowDepthTexture);
        sceneData_->ShadowDepthTexture = 0;
    }
    if (sceneData_->ShadowFramebuffer) {
        glDeleteFramebuffers(1, &sceneData_->ShadowFramebuffer);
        sceneData_->ShadowFramebuffer = 0;
    }
    sceneData_->ShadowShader.reset();
}

void SceneRenderer::RenderShadowPass() {
    if (!sceneData_ || sceneData_->Submissions.empty())
        return;

    if (!sceneData_->ShadowShader || !sceneData_->ShadowFramebuffer)
        return;

    GLint previousViewport[4];
    glGetIntegerv(GL_VIEWPORT, previousViewport);

    glViewport(0, 0, sceneData_->ShadowMapSize.x, sceneData_->ShadowMapSize.y);
    glBindFramebuffer(GL_FRAMEBUFFER, sceneData_->ShadowFramebuffer);
    glClear(GL_DEPTH_BUFFER_BIT);

    GLboolean wasCullEnabled = glIsEnabled(GL_CULL_FACE);
    GLint previousCullFaceMode = GL_BACK;
    if (wasCullEnabled)
        glGetIntegerv(GL_CULL_FACE_MODE, &previousCullFaceMode);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    sceneData_->ShadowShader->bind();
    sceneData_->ShadowShader->setMat4("uLightSpaceMatrix", sceneData_->LightSpaceMatrix);

    for (const auto& submission : sceneData_->Submissions) {
        if (!submission.CastsShadows)
            continue;

        if (!submission.VertexArray)
            continue;

        sceneData_->ShadowShader->setMat4("uModel", submission.Transform);
        RenderCommand::DrawIndexed(submission.VertexArray.get());
    }

    glCullFace(previousCullFaceMode);
    if (!wasCullEnabled)
        glDisable(GL_CULL_FACE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(previousViewport[0], previousViewport[1], previousViewport[2], previousViewport[3]);
}

void SceneRenderer::RenderScenePass() {
    if (!sceneData_)
        return;

    glActiveTexture(GL_TEXTURE0);
    if (sceneData_->ShadowsEnabled && sceneData_->ShadowDepthTexture)
        glBindTexture(GL_TEXTURE_2D, sceneData_->ShadowDepthTexture);
    else
        glBindTexture(GL_TEXTURE_2D, 0);

    for (const auto& submission : sceneData_->Submissions) {
        if (!submission.VertexArray || !submission.Material)
            continue;

        submission.Material->Bind();
        auto shader = submission.Material->GetShader();
        if (!shader)
            continue;

        shader->setMat4("uView", sceneData_->ViewMatrix);
        shader->setMat4("uProj", sceneData_->ProjectionMatrix);
        shader->setMat4("uModel", submission.Transform);
        shader->setVec3("uLightDirection", -sceneData_->DirectionalLight.Direction);
        shader->setVec3("uLightColor", sceneData_->DirectionalLight.Color);
        shader->setFloat("uLightIntensity", sceneData_->DirectionalLight.Active
                                                ? sceneData_->DirectionalLight.Intensity
                                                : 0.0f);
        shader->setFloat("uAmbientStrength", sceneData_->AmbientStrength);
        shader->setMat4("uLightSpaceMatrix", sceneData_->LightSpaceMatrix);
        shader->setInt("uShadowMap", 0);
        shader->setFloat("uReceiveShadows", submission.ReceiveShadows ? 1.0f : 0.0f);
        shader->setFloat("uShadowsEnabled",
                         sceneData_->ShadowsEnabled && sceneData_->DirectionalLight.Active ? 1.0f
                                                                                           : 0.0f);

        RenderCommand::DrawIndexed(submission.VertexArray.get());

        stats_.DrawCalls++;
        stats_.TriangleCount += submission.VertexArray->GetIndexBuffer()->GetCount() / 3;
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}
} // namespace se
