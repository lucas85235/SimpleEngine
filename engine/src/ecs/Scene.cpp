#include "engine/ecs/Scene.h"
#include "engine/Log.h"
#include "engine/ecs/Components.h"
#include "engine/ecs/RenderSystem.h"
#include "engine/ecs/Serialization.h"
#include "json.hpp"

using json = nlohmann::json;

namespace se {

Scene::Scene(const std::string& name) : name_(name) {
    SE_LOG_INFO("Scene '{}' created", name_);
    LoadMaterial();
}

Scene::~Scene() {
    Clear();
    SE_LOG_INFO("Scene '{}' destroyed", name_);
}

void Scene::OnUpdate(float deltaTime) {
    // Systems can be implemented here
    // Example: Physics system, Animation system, etc.

    // For now, just a placeholder
    (void)deltaTime;
}

void Scene::OnRender(const Camera& camera, float aspectRatio) {
    RenderSystem::Render(*this, camera, aspectRatio);
}

void Scene::Clear() {
    SE_LOG_INFO("Clearing scene '{}'", name_);
    registry_.clear();
}

void Scene::LoadSceneFromFile(const std::string& filepath) {

    auto assets_folder = findAssetsFolder();
    fs::path full_path = assets_folder.value() / filepath;

    SE_LOG_INFO("Loading scene from file: {}", full_path.string());

    std::ifstream file_input(full_path.string());
    if (!file_input.is_open()) {
        SE_LOG_ERROR("Failed to open scene file: {}", full_path.string());
        return;
    }

    nlohmann::json scene_json;
    try {
        file_input >> scene_json;
    } catch (const std::exception& e) {
        SE_LOG_ERROR("Failed to parse JSON from scene file: {}. Error: {}", full_path.string(), e.what());
        return;
    }

    try {
        Clear();

        // Deserialize scene name
        if (scene_json.contains("scene_name")) {
            name_ = scene_json.at("scene_name").get<std::string>();
        } else {
            SE_LOG_WARN("Scene file missing 'scene_name' field. Using default name.");
            name_ = "Untitled Scene";
        }

        // Deserialize game objects
        if (scene_json.contains("game_objects")) {
            auto game_objects = scene_json.at("game_objects").get<std::vector<GameObject>>();

            for (const auto& go : game_objects) {
                Entity entity = CreateEntity(go.name);
                auto mesh = se::MeshManager::GetPrimitive(se::PrimitiveMeshType::Cube);
                if (!mesh) {
                    SE_LOG_ERROR("Failed to get cube mesh!");
                    return;
                }

                entity.AddComponent<se::MeshRenderComponent>(mesh, global_material_);
                auto& transform = entity.GetComponent<se::TransformComponent>();
                transform.SetPosition(go.transform.position);

                SE_LOG_INFO("Loaded GameObject '{}'", go.name);
            }

        } else {
            SE_LOG_WARN("Scene file missing 'game_objects' field. No entities loaded.");
        }

        SE_LOG_INFO("Scene '{}' loaded successfully with {} entities", name_, GetEntityCount());

    } catch (const std::exception& e) {
        SE_LOG_ERROR("Error during scene deserialization: {}", e.what());
    }
}

void Scene::SaveSceneToFile(const std::string& filepath) {
    SE_LOG_INFO("Saving scene to file: {}", filepath);

    nlohmann::json scene_json;
    scene_json["scene_name"] = name_;

    std::vector<GameObject> game_objects;

    auto view = registry_.view<NameComponent, TransformComponent>();

    for (auto entity : view) {
        auto& nameComp = view.get<NameComponent>(entity);
        auto& transformComp = view.get<TransformComponent>(entity);

        GameObject go;
        go.id = static_cast<uint32_t>(entity);
        go.name = nameComp.Name;
        go.transform.position = transformComp.Position;
        go.transform.rotation = transformComp.Rotation;

        // For simplicity, we're not handling RendererComponent here
        // In a full implementation, you would serialize mesh and material info

        game_objects.push_back(go);
    }

    scene_json["game_objects"] = game_objects;

    std::ofstream file_output(filepath);
    if (!file_output.is_open()) {
        SE_LOG_ERROR("Failed to open file for writing: {}", filepath);
        return;
    }

    try {
        file_output << scene_json.dump(4); // Pretty print with 4 spaces
        SE_LOG_INFO("Scene '{}' saved successfully with {} entities", name_, game_objects.size());
    } catch (const std::exception& e) {
        SE_LOG_ERROR("Failed to write JSON to file: {}. Error: {}", filepath, e.what());
    }
}

Entity Scene::CreateEntity(const std::string& name) {
    Entity entity(registry_.create(), this);

    // All entities have Transform and Name components by default
    entity.AddComponent<TransformComponent>();
    entity.AddComponent<NameComponent>(name.empty() ? "Entity" : name);

    SE_LOG_INFO("Entity '{}' created with ID: {}", name, entity.GetID());

    return entity;
}

void Scene::DestroyEntity(Entity entity) {
    if (!entity.IsValid()) {
        SE_LOG_WARN("Attempted to destroy invalid entity");
        return;
    }

    auto& nameComp = entity.GetComponent<NameComponent>();
    SE_LOG_INFO("Entity '{}' destroyed", nameComp.Name);

    registry_.destroy(entity.GetHandle());
}

Entity Scene::FindEntityByName(const std::string& name) {
    auto view = registry_.view<NameComponent>();

    for (auto entity : view) {
        auto& nameComp = view.get<NameComponent>(entity);
        if (nameComp.Name == name) {
            return Entity(entity, this);
        }
    }

    SE_LOG_WARN("Entity with name '{}' not found", name);
    return Entity(); // Return invalid entity
}

void Scene::LoadMaterial() {
    auto assets_folder = findAssetsFolder();
    fs::path fragment_shader_location = assets_folder.value() / "shaders" / "basic.frag";
    fs::path vertex_shader_location = assets_folder.value() / "shaders" / "basic.vert";

    std::shared_ptr<se::Shader> shader = se::MaterialManager::GetShader(
        "DefaultShader", vertex_shader_location, fragment_shader_location);

    global_material_ = se::MaterialManager::CreateMaterial(shader);
}

} // namespace se