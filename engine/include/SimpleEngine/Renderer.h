#pragma once
#include <SimpleEngine/Camera.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace se {

class Renderer {
public:
    Renderer();
    ~Renderer();

    void init();             // create VAO/VBO + compile shader
    void draw(float time);   // draw rotating color factor

private:
    unsigned int vao_ = 0;
    unsigned int vbo_ = 0;
    unsigned int program_ = 0; // shader program id (kept minimal here)

    Camera camera_;
    GLint viewLoc_ = -1;
    GLint projLoc_ = -1;

    // mouse control helpers
    double lastX_ = 0.0, lastY_ = 0.0;
    bool firstMouse_ = true;

    // internal
    void processMouse(double xpos, double ypos);
    void processKeyboard(float deltaTime);
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
};

} // namespace se
