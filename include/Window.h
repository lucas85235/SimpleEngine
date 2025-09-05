#pragma once
#include <string>

// Forward declaration to avoid including GLFW in the header.
struct GLFWwindow;

class Window {
public:
    Window(int width, int height, const std::string& title);
    ~Window();

    // Non-copyable
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    // Main loop helpers
    bool shouldClose() const;
    void requestClose() const;
    void swapBuffers() const;
    void pollEvents() const;

    // Input
    bool isKeyPressed(int key) const;

    // Accessors
    GLFWwindow* native() const { return handle_; }
    int width()  const { return width_;  }
    int height() const { return height_; }

private:
    GLFWwindow* handle_ = nullptr;
    int width_ = 0;
    int height_ = 0;

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    void applyViewport(int width, int height) const;
};
