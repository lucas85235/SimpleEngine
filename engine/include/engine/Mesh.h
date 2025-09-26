#pragma once

#include <glad/glad.h>

class TriangleMesh {
public:
    TriangleMesh() = default;
    ~TriangleMesh();

    void initialize();
    void draw();

private:
    GLuint vao_ = 0;
    GLuint vbo_ = 0;

    // Triangle data: pos (x,y), color (r,g,b)
    static constexpr float vertices[] = {
        -0.6f, -0.5f,  1.f, 0.f, 0.f,  // Red vertex
         0.6f, -0.5f,  0.f, 1.f, 0.f,  // Green vertex
         0.0f,  0.6f,  0.f, 0.f, 1.f,  // Blue vertex
    };

    void createBuffers();
    void setupVertexAttributes();
    void unbindBuffers();
    void cleanup();
};
