#include "AppLayer.h"
#include <GLFW/glfw3.h>
#include <engine/Application.h>
#include <engine/Input.h>
#include <engine/Log.h>
#include <engine/MeshFactory.h>
#include <engine/renderer/RenderCommand.h>
#include <engine/renderer/SceneRenderer.h>
#include <gtc/matrix_transform.hpp>
#include <imgui.h>

AppLayer::AppLayer()
    : Layer("AppLayer"), camera_(glm::vec3(0.0f, 0.0f, 5.0f)), inputHandler_(camera_) {}

AppLayer::~AppLayer() {}

void AppLayer::OnAttach() {
    SE_LOG_INFO("AppLayer attached");

    // Setup scene (load shaders, create meshes, etc.)
    SetupScene();

    // Initialize input handler
    auto& app = se::Application::Get();
    inputHandler_.initialize(app.GetWindow().GetNativeWindow());
}

void AppLayer::OnDetach() {
    SE_LOG_INFO("AppLayer detached");
}

void AppLayer::OnEvent(se::Event& event) {
    // Handle events if needed
}

void AppLayer::OnUpdate(float ts) {
    animationTime_ += ts;

    // Handle input
    HandleInput(ts);

    // Update camera or other game logic here
}

void AppLayer::OnRender() {
    auto& app = se::Application::Get();
    auto& renderer = app.GetRenderer();

    // Clear screen
    renderer.SetClearColor(0.08f, 0.10f, 0.14f, 1.0f);
    renderer.Clear();

    // Begin scene with camera
    glm::vec2 window_size = {se::Application::Get().GetWindow().GetWidth(), se::Application::Get().GetWindow().GetHeight()};
    float aspectRatio = window_size.x / window_size.y;
    renderer.BeginScene(camera_, aspectRatio);

    // Update material animation
    float mixValue = 0.5f * (std::sin(animationTime_) * 0.5f + 0.5f);
    material_->SetFloat("uMix", mixValue);

    // Submit mesh for rendering
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, animationTime_ * glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    se::SceneRenderer::Submit(meshVAO_, material_, model);

    // End scene
    renderer.EndScene();
}

void AppLayer::OnImGuiRender() {
    ImGui::Begin("App Layer");
    ImGui::Text("Hello from AppLayer!");
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::Text("Animation Time: %.2f", animationTime_);

    // Camera info using getters
    ImGui::Separator();
    ImGui::Text("Camera Info:");
    glm::vec3 cameraPos = camera_.GetPosition();
    ImGui::Text("Position: (%.2f, %.2f, %.2f)", cameraPos.x, cameraPos.y, cameraPos.z);
    ImGui::Text("Yaw: %.2f, Pitch: %.2f", camera_.GetYaw(), camera_.GetPitch());
    ImGui::Text("Zoom: %.2f", camera_.GetZoom());

    // Render stats
    auto stats = se::Application::Get().GetRenderer().GetStats();
    ImGui::Separator();
    ImGui::Text("Render Stats:");
    ImGui::Text("Draw Calls: %u", stats.DrawCalls);
    ImGui::Text("Triangles: %u", stats.TriangleCount);

    ImGui::End();
}

void AppLayer::SetupScene() {
    SE_LOG_INFO("Setting up scene");

    // Find assets folder
    auto assets = findAssetsFolder();
    if (!assets) {
        SE_LOG_ERROR("Assets folder not found");
        throw std::runtime_error("Assets folder not found");
    }

    // Load shaders
    auto vsPath = *assets / "shaders" / "basic.vert";
    auto fsPath = *assets / "shaders" / "basic.frag";

    auto shader = se::Shader::CreateFromFiles(vsPath, fsPath);

    // Create material
    material_ = std::make_shared<se::Material>(shader);
    material_->SetFloat("uMix", 0.0f);

    // Create mesh
    Mesh mesh = MeshFactory::CreateCylinder();

    const auto& vertices = mesh.getVertices();
    const auto& indices = mesh.getIndices();

    // Create buffers
    auto vbo = std::make_shared<se::VertexBuffer>(
        vertices.data(), static_cast<uint32_t>(vertices.size() * sizeof(float)));
    vbo->SetLayout({{se::ShaderDataType::Float3, "aPos"}, {se::ShaderDataType::Float3, "aColor"}});

    auto ebo =
        std::make_shared<se::IndexBuffer>(indices.data(), static_cast<uint32_t>(indices.size()));

    // Create VAO
    meshVAO_ = std::make_shared<se::VertexArray>();
    meshVAO_->AddVertexBuffer(vbo);
    meshVAO_->SetIndexBuffer(ebo);

    SE_LOG_INFO("Scene setup complete");
}

void AppLayer::HandleInput(float deltaTime) {
    auto& app = se::Application::Get();
    GLFWwindow* window = app.GetWindow().GetNativeWindow();

    if (window) {
        inputHandler_.processKeyboard(window, deltaTime);
    }
}