#pragma once

#include <glad/glad.h>

class TriangleMesh {
private:
    GLuint vao_ = 0;
    GLuint vbo_ = 0;

    // Triangle data: pos (x,y), color (r,g,b)
    static constexpr float vertices[] = {
        -0.6f, -0.5f,  1.f, 0.f, 0.f,  // Red vertex
         0.6f, -0.5f,  0.f, 1.f, 0.f,  // Green vertex
         0.0f,  0.6f,  0.f, 0.f, 1.f,  // Blue vertex
    };

public:
    void initialize() {
        createBuffers();
        setupVertexAttributes();
        unbindBuffers();
    }
    
    void draw() const {
        glBindVertexArray(vao_);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
    }
    
    ~TriangleMesh() {
        cleanup();
    }

private:
    void createBuffers() {
        glGenVertexArrays(1, &vao_);
        glBindVertexArray(vao_);
        
        glGenBuffers(1, &vbo_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    }
    
    void setupVertexAttributes() {
        // Position attribute (location = 0)
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 
                            5 * sizeof(float), (void*)0);
        
        // Color attribute (location = 1) 
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 
                            5 * sizeof(float), (void*)(2 * sizeof(float)));
    }
    
    void unbindBuffers() {
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    
    void cleanup() {
        if (vbo_) glDeleteBuffers(1, &vbo_);
        if (vao_) glDeleteVertexArrays(1, &vao_);
    }
};
