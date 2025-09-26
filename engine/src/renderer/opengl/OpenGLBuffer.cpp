#include <engine/renderer/opengl/OpenGLBuffer.h>
#include <iostream>

OpenGLBuffer::OpenGLBuffer(const BufferDesc& desc) : m_Desc(desc) {
    std::cout << "OpenGLBuffer criado! Tamanho: " << m_Desc.size << " bytes." << std::endl;
    // TODO: Aqui entraria o código real:
    // glGenBuffers(1, &m_RendererID);
    // glBindBuffer(...);
    // glBufferData(..., m_Desc.size, m_Desc.initialData, ...);
}

OpenGLBuffer::~OpenGLBuffer() {
    std::cout << "OpenGLBuffer destruído!" << std::endl;
    // TODO: glDeleteBuffers(1, &m_RendererID);
}

void OpenGLBuffer::Update(const void* data, uint32_t size, uint32_t offset) {
    std::cout << "OpenGLBuffer::Update chamado." << std::endl;
    // TODO: glBindBuffer(...);
    // TODO: glBufferSubData(..., offset, size, data);
}
