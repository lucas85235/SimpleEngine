#pragma once

#include <glm.hpp>

struct GLFWwindow;

namespace se {
class Input {
  public:
    static bool IsKeyPressed(int keycode);

    static bool IsMouseButtonPressed(int button);

    static glm::vec2 GetMousePosition();

    static float GetMouseX();

    static float GetMouseY();

    // Internal: Used by Window to set the context
    static void SetWindow(GLFWwindow* window);

  private:
    Input() = delete;

    static GLFWwindow* window_;
};
} // namespace se
