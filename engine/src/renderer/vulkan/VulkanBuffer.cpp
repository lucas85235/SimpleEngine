#include <engine/renderer/vulkan/VulkanBuffer.h>
#include <iostream>

VulkanBuffer::VulkanBuffer(const BufferDesc& desc) : m_Desc(desc) {
    std::cout << "VulkanBuffer criado! Tamanho: " << m_Desc.size << " bytes." << std::endl;
    // TODO: Aqui entraria o código real:
    // glGenBuffers(1, &m_RendererID);
    // glBindBuffer(...);
    // glBufferData(..., m_Desc.size, m_Desc.initialData, ...);
}

VulkanBuffer::~VulkanBuffer() {
    std::cout << "VulkanBuffer destruído!" << std::endl;
    // TODO: glDeleteBuffers(1, &m_RendererID);
}

void VulkanBuffer::Update(const void* data, uint32_t size, uint32_t offset) {
    std::cout << "VulkanBuffer::Update chamado." << std::endl;
    // TODO: glBindBuffer(...);
    // TODO: glBufferSubData(..., offset, size, data);
}
