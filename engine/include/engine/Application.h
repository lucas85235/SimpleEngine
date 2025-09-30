#pragma once
#include "se_pch.h"
#include "Engine.h"
#include "engine/Window.h"
#include "engine/Renderer.h"

namespace se{

    class Application {
	public:
        Application(const ApplicationSpec& specification);
        ~Application();

        int Run();

    private:
        ApplicationSpec specification_;

        bool running_ = true;
        bool minimized_ = false;
        float last_frame_time_ = 0.0f;
        Window window_;
        Renderer renderer_;

    private:
        static Application* instance_;
    };
}