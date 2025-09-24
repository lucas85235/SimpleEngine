#pragma once

#include <engine/Camera.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class InputHandler {
public:
    explicit InputHandler(Camera& camera) : camera_(&camera) {}

    void initialize(GLFWwindow* window);
    void processKeyboard(GLFWwindow* window, float deltaTime);
    void processMouse(double xpos, double ypos);

private:
    Camera* camera_ = nullptr;
    double lastX_ = 0.0;
    double lastY_ = 0.0;
    bool firstMouse_ = true;

    void setupMouseCapture(GLFWwindow* window);
    void initializeMousePosition(GLFWwindow* window);

    static void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
        auto* handler = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));
        if (handler) {
            handler->processMouse(xpos, ypos);
        }
    }
};
