#pragma once
#include <GLFW/glfw3.h>
#include <string>
#include <functional>

class Window {
public:
    Window(int width, int height, const std::string& title);
    ~Window();
    
    bool shouldClose() const;
    void pollEvents();
    void swapBuffers();
    GLFWwindow* getHandle() const { return window; }
    
    void setResizeCallback(std::function<void(int, int)> callback);
    
private:
    GLFWwindow* window;
    std::function<void(int, int)> resizeCallback;
    
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
};