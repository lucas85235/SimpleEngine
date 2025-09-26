#include <engine/renderer/opengl/OpenGLDevice.h>
#include <engine/renderer/opengl/OpenGLBuffer.h>
#include <iostream>

OpenGLDevice::OpenGLDevice() {
    std::cout << "OpenGLDevice criado e inicializado!" << std::endl;
    // TODO: Aqui você inicializaria a janela (GLFW/SDL) e o carregador de funções (GLAD).
}

OpenGLDevice::~OpenGLDevice() {
    std::cout << "OpenGLDevice destruído!" << std::endl;
}

std::unique_ptr<IBuffer> OpenGLDevice::CreateBuffer(const BufferDesc& desc) {
    // A "mágica" acontece aqui: retornamos um ponteiro para a implementação concreta,
    // mas o tipo de retorno é a interface!
    return std::make_unique<OpenGLBuffer>(desc);
}
