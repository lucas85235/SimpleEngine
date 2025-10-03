#pragma once
#include "Engine.h"
#include "Layer.h"
#include "Renderer.h"
#include "Window.h"

namespace se {

class Application {
  public:
    Application(const ApplicationSpec& specification);
    ~Application();

    int Run();
    void Stop();

    void PushLayer();
    void PushOverlay();

    template <typename TLayer>
        requires std::is_base_of_v<Layer, TLayer>
    void PushLayer() {
        layer_stack_.push_back(std::unique_ptr<Layer>(std::make_unique<TLayer>().release()));
    }

    static Application& Get();
    static float GetTime();

  private:
    ApplicationSpec specification_;

    bool running_ = false;
    bool minimized_ = false;
    float last_frame_time_ = 0.0f;
    Window window_;
    Renderer renderer_;
    std::vector<std::unique_ptr<Layer>> layer_stack_;
};

Application* CreateApplication(ApplicationCommandLineArgs args);
} // namespace se
