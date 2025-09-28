#include <engine/OldRenderer.h>
#include <engine/Shader.h>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

namespace se {

OldRenderer::OldRenderer() : camera_(glm::vec3(0.0f, 0.0f, 3.0f)), inputHandler_(camera_) {}

void OldRenderer::init() {
    // Find shaders folder and files
    auto assets = findAssetsFolder();
    if (!assets) {
        throw std::runtime_error("Assets folder not found. Checked: ASSETS_DIR, ./assets, exe_dir/assets");
    }
    fs::path vsPath = *assets / "shaders" / "basic.vert";
    fs::path fsPath = *assets / "shaders" / "basic.frag";

    // Create shader
    Shader shader = Shader::fromFiles(vsPath, fsPath);
    shader.bind();
    program_ = shader.release();

    viewLoc_ = glGetUniformLocation(program_, "uView");
    projLoc_ = glGetUniformLocation(program_, "uProj");

    try {
        // Initialize component
        mesh_.initialize();
        inputHandler_.initialize(glfwGetCurrentContext());
        
    } catch (const std::exception& e) {
        std::cerr << "Renderer initialization failed: " << e.what() << std::endl;
        throw;
    }

    shader.unbind();
}

void OldRenderer::updateDeltaTime(float currentTime) {
    static float lastFrame = 0.0f;
    deltaTime_ = currentTime - lastFrame;
    lastFrame = currentTime;
}

void OldRenderer::handleInput() {
    GLFWwindow* window = glfwGetCurrentContext();
    if (window) {
        inputHandler_.processKeyboard(window, deltaTime_);
    }
}

void OldRenderer::draw(float time) {
    updateDeltaTime(time);
    handleInput();

    glClearColor(0.08f, 0.10f, 0.14f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program_);

    setupMatrices();
    setupAnimationUniforms(time);

    mesh_.draw();

    glUseProgram(0);
}

void OldRenderer::setupMatrices() {
    auto viewport = 800.0f / 600.0f;

    glm::mat4 view = camera_.getViewMatrix();
    glm::mat4 proj = camera_.getProjectionMatrix(viewport);

    glUniformMatrix4fv(viewLoc_, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc_, 1, GL_FALSE, glm::value_ptr(proj));
}

void OldRenderer::setupAnimationUniforms(float time) {
    float mixValue = 0.5f * (std::sin(time) * 0.5f + 0.5f); // [0, 0.5]
    GLint loc = glGetUniformLocation(program_, "uMix");
    if (loc >= 0) glUniform1f(loc, mixValue);
}

OldRenderer::~OldRenderer() {
    if (program_) glDeleteProgram(program_);
    if (vbo_) glDeleteBuffers(1, &vbo_);
    if (vao_) glDeleteVertexArrays(1, &vao_);
}

} // namespace se
