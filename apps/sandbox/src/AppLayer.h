#pragma once

#include <engine/Camera.h>
#include <engine/InputHandler.h>
#include <engine/Layer.h>
#include <engine/ecs/Scene.h>
#include <engine/resources/MaterialManager.h>
#include <engine/resources/MeshManager.h>
#include <glm.hpp>
#include <memory>
#include <string>

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
    void HandleInput(float deltaTime);

    // todo: refactor to utils? or move to scene setup code
    void LoadMaterial();
    void AddDirectionalLight();
    void CreateCubeEntity(const std::string& name, const glm::vec3& position);
    void CreateSphereEntity(const std::string& name, const glm::vec3& position);
    void CreateCapsuleEntity(const std::string& name, const glm::vec3& position);

  private:
    std::unique_ptr<se::Scene> scene_;
    std::shared_ptr<se::Material> material_;

    Camera camera_;
    InputHandler inputHandler_;

    float animationTime_ = 0.0f;
    float yaw_ = 0.0f;
    bool camera_active_ = true;
};
