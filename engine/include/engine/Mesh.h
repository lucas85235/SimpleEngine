#pragma once

#include <glad/glad.h>
#include <vector>
#include <cstddef>
#include <utility>

// A generic vertex structure (position + color)
struct Vertex {
    float x, y, z;    // position (vec3)
    float r, g, b;    // color (vec3)
};

class Mesh {
public:
    Mesh() = default;

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    Mesh(Mesh&& other) noexcept { stealFrom(std::move(other)); }
    Mesh& operator=(Mesh&& other) noexcept {
        if (this != &other) {
            cleanup();
            stealFrom(std::move(other));
        }
        return *this;
    }

    ~Mesh() { cleanup(); }

    // Initialize the mesh with vertices and optional indices
    void initialize(const std::vector<Vertex>& vertices,
                    const std::vector<unsigned int>& indices = {})
    {
        createBuffers(vertices, indices);
        setupVertexAttributes();
        unbindBuffers();
    }

    // Draw the mesh
    void draw() const {
        if (!vao_) return;
        glBindVertexArray(vao_);
        if (indexCount_ > 0) {
            glDrawElements(GL_TRIANGLES, indexCount_, GL_UNSIGNED_INT, 0);
        } else {
            glDrawArrays(GL_TRIANGLES, 0, vertexCount_);
        }
        glBindVertexArray(0);
    }

private:
    GLuint vao_ = 0;
    GLuint vbo_ = 0;
    GLuint ebo_ = 0;

    GLsizei indexCount_ = 0;
    GLsizei vertexCount_ = 0;

    void createBuffers(const std::vector<Vertex>& vertices,
                       const std::vector<unsigned int>& indices)
    {
        vertexCount_ = static_cast<GLsizei>(vertices.size());
        indexCount_ = static_cast<GLsizei>(indices.size());

        glGenVertexArrays(1, &vao_);
        glBindVertexArray(vao_);

        glGenBuffers(1, &vbo_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferData(GL_ARRAY_BUFFER,
                     vertices.size() * sizeof(Vertex),
                     vertices.data(),
                     GL_STATIC_DRAW);

        if (!indices.empty()) {
            glGenBuffers(1, &ebo_);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                         indices.size() * sizeof(unsigned int),
                         indices.data(),
                         GL_STATIC_DRAW);
        }
    }

    void setupVertexAttributes() {
        // position -> location 0 (vec3)
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                              sizeof(Vertex), (void*)offsetof(Vertex, x));

        // color -> location 1 (vec3)
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                              sizeof(Vertex), (void*)offsetof(Vertex, r));
    }

    void unbindBuffers() {
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // keep EBO bound to VAO semantics (if any)
    }

    void cleanup() {
        if (ebo_) { glDeleteBuffers(1, &ebo_); ebo_ = 0; }
        if (vbo_) { glDeleteBuffers(1, &vbo_); vbo_ = 0; }
        if (vao_) { glDeleteVertexArrays(1, &vao_); vao_ = 0; }
        indexCount_ = vertexCount_ = 0;
    }

    void stealFrom(Mesh&& o) noexcept {
        vao_ = o.vao_; vbo_ = o.vbo_; ebo_ = o.ebo_;
        indexCount_ = o.indexCount_; vertexCount_ = o.vertexCount_;
        o.vao_ = o.vbo_ = o.ebo_ = 0;
        o.indexCount_ = o.vertexCount_ = 0;
    }
};
