#pragma once
#include "Engine.h"
#include <string>
#include <GLFW/glfw3.h>
#include <functional>

namespace se {

class Window {
  public:
    Window(uint32_t width, uint32_t height, const std::string& title);
    Window(const ApplicationSpec& specification);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    bool shouldClose() const;
    void requestClose() const;
    void swapBuffers() const;
    void pollEvents() const;
    bool isKeyPressed(int key) const;

    GLFWwindow* native() const {
        return window_;
    }
    uint32_t width() const {
        return width_;
    }
    uint32_t height() const {
        return height_;
    }

    void setResizeCallback(std::function<void(int, int)> callback);
  
    void Destroy();

  private:
    GLFWwindow* window_ = nullptr;
    uint32_t width_ = 0;
    uint32_t height_ = 0;
    std::function<void(int, int)> resizeCallback;

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    void applyViewport(int width, int height) const;
};

} // namespace se
