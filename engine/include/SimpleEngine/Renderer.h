#pragma once

namespace se {

class Renderer {
public:
    Renderer() = default;
    ~Renderer();

    void init();             // create VAO/VBO + compile shader
    void draw(float time);   // draw rotating color factor

private:
    unsigned int vao_ = 0;
    unsigned int vbo_ = 0;
    unsigned int program_ = 0; // shader program id (kept minimal here)
};

} // namespace se
