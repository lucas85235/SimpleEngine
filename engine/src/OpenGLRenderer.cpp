#include <GLFW/glfw3.h>
#include <engine/MeshFactory.h>
#include <engine/OpenGLRenderer.h>
#include <gtc/type_ptr.hpp>

namespace se {

void OpenGLRenderer::Initialize(Window& window) {
    currentWindow = &window;

    // Configurar contexto OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Recriar janela com contexto OpenGL
    GLFWwindow* handle = window.native();
    glfwMakeContextCurrent(handle);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwDestroyWindow(window.native());
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLAD");
    }

    // Configurações iniciais
    glViewport(0, 0, 800, 600);
    glClearColor(1.0f, 0.2f, 0.2f, 1.0f);

    std::cout << "OpenGL " << glGetString(GL_VERSION) << " inicializado" << std::endl;

    // Temp
    init();
}

void OpenGLRenderer::Render(float delta_time) {
    // Temp
    draw(delta_time);
}

void OpenGLRenderer::Cleanup() {
    // Temp
    cleanTrash();
}

void OpenGLRenderer::OnResize(int width, int height) {
    glViewport(0, 0, width, height);
}

// ----- Temp render example ----- //

void OpenGLRenderer::init() {
    glEnable(GL_DEPTH_TEST);

    camera_ = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
    inputHandler_ = std::make_unique<InputHandler>(*camera_);

    // Find shaders folder and files
    auto assets = findAssetsFolder();
    if (!assets) {
        throw std::runtime_error(
            "Assets folder not found. Checked: ASSETS_DIR, ./assets, exe_dir/assets");
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
        mesh_ = MeshFactory::CreateCube();
        inputHandler_->initialize(glfwGetCurrentContext());
    } catch (const std::exception& e) {
        std::cerr << "Renderer initialization failed: " << e.what() << std::endl;
        throw;
    }

    shader.unbind();
}

void OpenGLRenderer::updateDeltaTime(float delta_time) {
    deltaTime_ = delta_time;
}

void OpenGLRenderer::handleInput() {
    GLFWwindow* window = glfwGetCurrentContext();
    if (window) {
        inputHandler_->processKeyboard(window, deltaTime_);
    }
}

void OpenGLRenderer::draw(float time) {
    updateDeltaTime(time);
    handleInput();

    glClearColor(0.08f, 0.10f, 0.14f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(program_);

    setupMatrices();
    setupAnimationUniforms(time);

    mesh_.draw();

    glUseProgram(0);
}

void OpenGLRenderer::setupMatrices() {
    auto viewport = 800.0f / 600.0f;

    glm::mat4 view = camera_->getViewMatrix();
    glm::mat4 proj = camera_->getProjectionMatrix(viewport);

    glUniformMatrix4fv(viewLoc_, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc_, 1, GL_FALSE, glm::value_ptr(proj));
}

void OpenGLRenderer::setupAnimationUniforms(float time) {
    float mixValue = 0.5f * (std::sin(time) * 0.5f + 0.5f); // [0, 0.5]
    GLint loc = glGetUniformLocation(program_, "uMix");
    if (loc >= 0)
        glUniform1f(loc, mixValue);
}

void OpenGLRenderer::cleanTrash() {
    if (program_)
        glDeleteProgram(program_);
    if (vbo_)
        glDeleteBuffers(1, &vbo_);
    if (vao_)
        glDeleteVertexArrays(1, &vao_);
}

} // namespace se
