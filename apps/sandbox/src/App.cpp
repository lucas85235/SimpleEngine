#include "App.h"
#include <GLFW/glfw3.h>
#include <engine/Log.h>

#include <iostream>
#include <memory>
#include <engine/OpenGLRenderer.h>

// ImGui headers
#include "imgui.h"
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"

App::App()
    : window_(800, 600, "Sandbox - OpenGL 3.3 Core")
{
    // Initialize logging
    se::LogInit(true);
    SE_LOG_INFO("Starting engine - build: {}", 0);

    // Initialize renderer
    render_ = std::make_unique<se::OpenGLRenderer>();
    render_->initialize(window_);
    // renderer_.init();

    // ImGui: create context and init backend
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();

    // Initialize ImGui GLFW + OpenGL3 backends
    // note: the GLSL version string must match your GL context, here 330 core
    const char *glsl_version = "#version 330 core";
    ImGui_ImplGlfw_InitForOpenGL(window_.native(), true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

int App::run()
{
    double start = glfwGetTime();
    bool show_demo_window = true;

    while (!window_.shouldClose())
    {
        if (window_.isKeyPressed(GLFW_KEY_ESCAPE))
            window_.requestClose();

        double t = glfwGetTime() - start;

        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // You can move/close it at runtime; shows many ImGui features
        ImGui::ShowDemoWindow(&show_demo_window);

        // Engine rendering draw first, then ImGui overlay
        // renderer_.draw(static_cast<float>(t));
        render_->render();

        // Render ImGui on top
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        window_.pollEvents();
        window_.swapBuffers();
    }

    // Cleanup ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    return 0;
}
