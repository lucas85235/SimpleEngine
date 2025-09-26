#include <engine/renderer/vulkan/VulkanDevice.h>
#include <engine/renderer/vulkan/VulkanBuffer.h>
#include <iostream>

VulkanDevice::VulkanDevice() {
    std::cout << "VulkanDevice criado e inicializado!" << std::endl;
    // TODO: Aqui você inicializaria a janela (GLFW/SDL) e o carregador de funções (GLAD).
}

VulkanDevice::~VulkanDevice() {
    std::cout << "VulkanDevice destruído!" << std::endl;
}

std::unique_ptr<IBuffer> VulkanDevice::CreateBuffer(const BufferDesc& desc) {
    // A "mágica" acontece aqui: retornamos um ponteiro para a implementação concreta,
    // mas o tipo de retorno é a interface!
    return std::make_unique<VulkanBuffer>(desc);
}
