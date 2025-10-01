#pragma once
#include "se_pch.h"
#include <engine/Camera.h>
#include <engine/InputHandler.h>
#include <engine/Mesh.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace se {

class OldRenderer {
public:
    OldRenderer();
    ~OldRenderer();

    void init();             // create VAO/VBO + compile shader
    void clear();            // Limpa a tela

    void draw(float time);   // draw rotating color factor
    // void swapBuffers();      // Troca os buffers de frente e trás

private:
    // Core components
    InputHandler inputHandler_;
    TriangleMesh mesh_;

    unsigned int vao_ = 0;
    unsigned int vbo_ = 0;
    unsigned int program_ = 0; // shader program id (kept minimal here)

    Camera camera_;
    GLint viewLoc_ = -1;
    GLint projLoc_ = -1;

    float deltaTime_ = 0.0f;

    void updateDeltaTime(float currentTime);
    void handleInput();
    void setupMatrices();
    void setupAnimationUniforms(float time);
};

} // namespace se
