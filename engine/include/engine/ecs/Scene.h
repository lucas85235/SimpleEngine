
#pragma once

#include "engine/Camera.h"
#include "engine/Log.h"
#include "engine/ecs/Entity.h"
#include <engine/resources/MaterialManager.h>
#include <engine/resources/MeshManager.h>
#include <entt.hpp>
#include <json.hpp>
#include <string>

namespace se {

class Scene {
  public:
    Scene(const std::string& name = "Untitled Scene");
    ~Scene();

    void OnUpdate(float deltaTime);
    void OnRender(const Camera& camera, float aspectRatio);
    void Clear();

    void LoadSceneFromFile(const std::string& filepath);
    void SaveSceneToFile(const std::string& filepath);

    // Get scene name
    const std::string& GetName() const {
        return name_;
    }

    Entity CreateEntity(const std::string& name = "Entity");
    void DestroyEntity(Entity entity);

    // Find entity by name
    Entity FindEntityByName(const std::string& name);

    // Get all entities with specific components
    template <typename... Components>
    auto GetAllEntitiesWith() {
        return registry_.view<Components...>();
    }

    // Get entity count (number of alive entities)
    size_t GetEntityCount() const {
        return registry_.storage<entt::entity>()->size();
    }

  private:
    std::string name_;
    entt::registry registry_;
    std::shared_ptr<se::Material> global_material_;

    void LoadMaterial();

    friend class Entity;
    friend class RenderSystem;
};

// ==================== Entity Template Implementations ====================

template <typename T, typename... Args>
T& Entity::AddComponent(Args&&... args) {
    if (HasComponent<T>()) {
        SE_LOG_WARN("Entity already has component!");
        return GetComponent<T>();
    }
    return scene_->registry_.emplace<T>(entityHandle_, std::forward<Args>(args)...);
}

template <typename T>
T& Entity::GetComponent() {
    if (!HasComponent<T>()) {
        SE_LOG_ERROR("Entity does not have component!");
    }
    return scene_->registry_.get<T>(entityHandle_);
}

template <typename T>
bool Entity::HasComponent() {
    return scene_->registry_.all_of<T>(entityHandle_);
}

template <typename T>
void Entity::RemoveComponent() {
    if (!HasComponent<T>()) {
        SE_LOG_WARN("Entity does not have component!");
        return;
    }
    scene_->registry_.remove<T>(entityHandle_);
}

} // namespace se