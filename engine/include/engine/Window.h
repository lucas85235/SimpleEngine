#pragma once
#include <string>

// Forward declare GLFW types to keep header light
struct GLFWwindow;

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

    GLFWwindow* native() const { return handle_; }
    int width()  const { return width_; }
    int height() const { return height_; }

private:
    GLFWwindow* handle_ = nullptr;
    int width_ = 0;
    int height_ = 0;

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    void applyViewport(int width, int height) const;
};

} // namespace se
