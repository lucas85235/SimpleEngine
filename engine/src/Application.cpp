#include "engine/Application.h"
#include "engine/Log.h"
#include "se_pch.h"

#include <iostream>
#include <memory>
#include <engine/OpenGLRenderer.h>
#include <engine/VulkanRenderer.h>
#include <glm/glm.hpp>

// ImGui headers
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"

namespace se {

static Application* application = nullptr;

Application::Application(const ApplicationSpec& specification) {
    SE_LOG_INFO("Starting engine - build");

    // Initialize renderer
    window_ = std::make_unique<se::Window>(specification);
    renderer_ = std::make_unique<se::OpenGLRenderer>();

    renderer_->initialize(*window_);

    // ImGui: create context and init backend
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();

    // Initialize ImGui GLFW + OpenGL3 backends
    // note: the GLSL version string must match your GL context, here 330 core
    const char* glsl_version = "#version 330 core";
    ImGui_ImplGlfw_InitForOpenGL(window_->native(), true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}
Application::~Application() {
    window_->Destroy();

    glfwTerminate();

    application = nullptr;
}

int Application::Run() {
    running_ = true;
    float lastTime = GetTime();
    bool show_demo_window = true;

    while (running_) {
        if (window_->isKeyPressed(GLFW_KEY_ESCAPE))
            window_->requestClose();

        if (window_->shouldClose()) {
            Stop();
            break;
        }

        // update the engine timestep
        float currentTime = GetTime();
        float timestep = glm::clamp(currentTime - lastTime, 0.001f, 0.1f);

        lastTime = currentTime;

        for (const std::unique_ptr<Layer>& layer : layer_stack_)
            layer->OnUpdate(timestep);

        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // You can move/close it at runtime; shows many ImGui features
        ImGui::ShowDemoWindow(&show_demo_window);

        // Engine rendering draw first, then ImGui overlay
        // renderer_.draw(static_cast<float>(timestep));
        renderer_->render();

        // Render ImGui on top
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        window_->swapBuffers();
        window_->pollEvents();
    }

    // Cleanup ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    renderer_->cleanup();

    return 0;
}
void Application::Stop() {
    running_ = false;
}
void Application::PushLayer() {
    // TODO(rafael): implement
}
void Application::PushOverlay() {
    // TODO(rafael): implement
}

Application& Application::Get() {
    return *application;
}

float Application::GetTime() {
    return (float)glfwGetTime();
}
} // namespace se
