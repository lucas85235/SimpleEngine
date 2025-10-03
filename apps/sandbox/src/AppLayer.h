
#pragma once

#include <engine/Camera.h>
#include <engine/InputHandler.h>
#include <engine/Layer.h>
#include <engine/Mesh.h>
#include <engine/Shader.h>
#include <engine/renderer/Material.h>
#include <engine/renderer/VertexArray.h>
#include <memory>

class AppLayer : public se::Layer {
public:
    AppLayer();
    ~AppLayer() override;

    void OnAttach() override;
    void OnDetach() override;
    void OnEvent(se::Event& event) override;
    void OnUpdate(float ts) override;
    void OnRender() override;
    void OnImGuiRender() override;

private:
    void SetupScene();
    void HandleInput(float deltaTime);

private:
    // Camera and input
    Camera camera_;
    InputHandler inputHandler_;

    // Rendering resources
    std::shared_ptr<se::VertexArray> meshVAO_;
    std::shared_ptr<se::Material> material_;

    // Animation time
    float animationTime_ = 0.0f;
};