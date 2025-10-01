#pragma once
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <functional>

namespace se {

class Window {
public:
    Window(int width, int height, const std::string& title);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    bool shouldClose() const;
    void requestClose() const;
    void swapBuffers() const;
    void pollEvents() const;
    bool isKeyPressed(int key) const;

    GLFWwindow* native() const { return window_; }
    void setResizeCallback(std::function<void(int, int)> callback);

private:
    GLFWwindow* window_ = nullptr;
    std::function<void(int, int)> resizeCallback;

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    void applyViewport(int width, int height) const;
};

} // namespace se
