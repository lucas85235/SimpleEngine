#include "engine/Input.h"
#include <GLFW/glfw3.h>

namespace se {

    GLFWwindow* Input::s_Window = nullptr;

    void Input::SetWindow(GLFWwindow* window) {
        s_Window = window;
    }

    bool Input::IsKeyPressed(int keycode) {
        if (!s_Window)
            return false;
        int state = glfwGetKey(s_Window, keycode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Input::IsMouseButtonPressed(int button) {
        if (!s_Window)
            return false;
        int state = glfwGetMouseButton(s_Window, button);
        return state == GLFW_PRESS;
    }

    glm::vec2 Input::GetMousePosition() {
        if (!s_Window)
            return {0.0f, 0.0f};
        double xpos, ypos;
        glfwGetCursorPos(s_Window, &xpos, &ypos);
        return {static_cast<float>(xpos), static_cast<float>(ypos)};
    }

    float Input::GetMouseX() {
        return GetMousePosition().x;
    }

    float Input::GetMouseY() {
        return GetMousePosition().y;
    }

} // namespace se