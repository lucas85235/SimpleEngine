#include "json.hpp"

using json = nlohmann::json;

// Specialization for glm::vec3
namespace nlohmann {
template <>
struct adl_serializer<glm::vec3> {
    static void to_json(json& j, const glm::vec3& v) {
        j = {{"x", v.x}, {"y", v.y}, {"z", v.z}};
    }
    static void from_json(const json& j, glm::vec3& v) {
        j.at("x").get_to(v.x);
        j.at("y").get_to(v.y);
        j.at("z").get_to(v.z);
    }
};
} // namespace nlohmann

namespace se {

// Serializable structures
struct Transform {
    glm::vec3 position = {0.0f, 0.0f, 0.0f};
    glm::vec3 rotation = {0.0f, 0.0f, 0.0f};
};
struct GameObject {
    uint32_t id = 0;
    std::string name;
    Transform transform;
};

// For Transform
void to_json(json& j, const Transform& t) {
    j = {{"position", t.position}, {"rotation", t.rotation}};
}
void from_json(const json& j, Transform& t) {
    j.at("position").get_to(t.position);
    j.at("rotation").get_to(t.rotation);
}

// For GameObject
void to_json(json& j, const GameObject& go) {
    j = json{
        {"id", go.id}, 
        {"name", go.name}, 
        {"transform", go.transform}
    };
}
void from_json(const json& j, GameObject& go) {
    j.at("id").get_to(go.id);
    j.at("name").get_to(go.name);
    j.at("transform").get_to(go.transform);
}

} // namespace se
