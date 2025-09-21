#include <SimpleEngine/Renderer.h>
#include <SimpleEngine/Shader.h>
#include <cmath>

// GLAD first
#include <glad/glad.h>

namespace se {

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
    shader.unbind();
}

void Renderer::draw(float time) {
    glClearColor(0.08f, 0.10f, 0.14f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program_);

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
