#pragma once
#include <engine/Renderer.h>

#include <engine/InputHandler.h>
#include <engine/Mesh.h>
#include <engine/Shader.h>

#include "se_pch.h"

namespace se {

class OpenGLRenderer : public Renderer {
  public:
    void Initialize(Window& window) override;
    void Render(float time) override;
    void Cleanup() override;
    void OnResize(int width, int height) override;

  private:
    Window* currentWindow = nullptr;

    // Temp

    std::unique_ptr<InputHandler> inputHandler_;
    TriangleMesh mesh_;

    unsigned int vao_ = 0;
    unsigned int vbo_ = 0;
    unsigned int program_ = 0; // shader program id (kept minimal here)

    std::unique_ptr<Camera> camera_;
    GLint viewLoc_ = -1;
    GLint projLoc_ = -1;

    float deltaTime_ = 0.0f;

    void init(); // create VAO/VBO + compile shader
    void clear();
    void draw(float time);
    void updateDeltaTime(float delta_time);
    void handleInput();
    void setupMatrices();
    void setupAnimationUniforms(float time);
    void cleanTrash();
};

} // namespace se
