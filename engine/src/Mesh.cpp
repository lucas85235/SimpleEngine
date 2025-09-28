#include <engine/Mesh.h>
#include <engine/Log.h>

TriangleMesh::~TriangleMesh() {
    cleanup();
}

void TriangleMesh::initialize() {
    createBuffers();
    setupVertexAttributes();
    unbindBuffers();
}

void TriangleMesh::draw() {
    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

void TriangleMesh::createBuffers() {
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    
    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void TriangleMesh::setupVertexAttributes() {
    // Position attribute (location = 0)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 
                        5 * sizeof(float), (void*)0);
    
    // Color attribute (location = 1) 
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 
                        5 * sizeof(float), (void*)(2 * sizeof(float)));
}

void TriangleMesh::unbindBuffers() {
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void TriangleMesh::cleanup() {
    if (vbo_) glDeleteBuffers(1, &vbo_);
    if (vao_) glDeleteVertexArrays(1, &vao_);
}
