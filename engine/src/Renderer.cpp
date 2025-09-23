#include <engine/Renderer.h>
#include <engine/Shader.h>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

namespace se {

Renderer::Renderer() : camera_(glm::vec3(0.0f, 0.0f, 3.0f)) {}

void Renderer::init() {
    // Triangle data: pos (x,y), color (r,g,b)
    const float verts[] = {
        -0.6f, -0.5f,  1.f, 0.f, 0.f,
         0.6f, -0.5f,  0.f, 1.f, 0.f,
         0.0f,  0.6f,  0.f, 0.f, 1.f,
    };

    // Create VAO + VBO
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    // layout (location=0) -> vec2 position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

    // layout (location=1) -> vec3 color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));

    // Cleanup binding
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

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

    // Setup input: get GLFW window and register mouse callback + disable cursor
    GLFWwindow* window = glfwGetCurrentContext();
    if (!window) {
        std::cerr << "Renderer::init: no current GLFW context\n";
    } else {        
        // store pointer to this renderer so callback can access it
        glfwSetWindowUserPointer(window, this);
        glfwSetCursorPosCallback(window, mouseCallback);

        // capture the mouse
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        // initialize last mouse pos to window center
        int w, h;
        glfwGetWindowSize(window, &w, &h);
        lastX_ = w / 2.0;
        lastY_ = h / 2.0;
    }

    shader.unbind();
}

void Renderer::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    Renderer* r = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
    if (r) r->processMouse(xpos, ypos);
    else std::cout << "[mouseCallback] no renderer in window user pointer!\n";
}

void Renderer::processMouse(double xpos, double ypos) {
    if (firstMouse_) {
        lastX_ = xpos;
        lastY_ = ypos;
        firstMouse_ = false;
    }

    double xoffset = xpos - lastX_;
    double yoffset = lastY_ - ypos; // reversed: y ranges bottom->top in GLFW
    lastX_ = xpos;
    lastY_ = ypos;

    camera_.processMouseMovement(static_cast<float>(xoffset), static_cast<float>(yoffset));
}

void Renderer::processKeyboard(float deltaTime) {
    GLFWwindow* window = glfwGetCurrentContext();
    if (!window) return;

    bool forward = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
    bool back    = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
    bool left    = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
    bool right   = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
    bool upKey   = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
    bool downKey = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS;

    camera_.processKeyboard(deltaTime, forward, back, left, right, upKey, downKey);
}

void Renderer::draw(float time) {
    static float lastFrame = 0.0f;
    float currentFrame = time;
    float delta = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // allow keyboard movement
    processKeyboard(delta);

    glClearColor(0.08f, 0.10f, 0.14f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program_);

    // view / projection
    int width, height;
    GLFWwindow* window = glfwGetCurrentContext();
    if (window) glfwGetFramebufferSize(window, &width, &height);
    else { width = 800; height = 600; } // fallback
    float aspect = width / (float)height;

    glm::mat4 view = camera_.getViewMatrix();
    glm::mat4 proj = camera_.getProjectionMatrix(aspect);

    glUniformMatrix4fv(viewLoc_, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc_, 1, GL_FALSE, glm::value_ptr(proj));

    float u = 0.5f * (std::sin(time) * 0.5f + 0.5f); // [0,0.5]
    GLint loc = glGetUniformLocation(program_, "uMix");
    if (loc >= 0) glUniform1f(loc, u);

    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
    glUseProgram(0);
}

Renderer::~Renderer() {
    if (program_) glDeleteProgram(program_);
    if (vbo_) glDeleteBuffers(1, &vbo_);
    if (vao_) glDeleteVertexArrays(1, &vao_);
}

} // namespace se
