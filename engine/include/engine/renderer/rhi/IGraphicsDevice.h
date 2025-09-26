#pragma once
#include <memory>
#include "IBuffer.h" // Inclui nossa outra interface

// Forward declare para não precisar incluir os headers concretos
class IBuffer;

class IGraphicsDevice {
public:
    virtual ~IGraphicsDevice() = default;

    // A fábrica de recursos. O Device é responsável por criar todos os outros objetos.
    virtual std::unique_ptr<IBuffer> CreateBuffer(const BufferDesc& desc) = 0;

    // TODO: Adicionar mais métodos de criação no futuro
    // virtual std::unique_ptr<ITexture> CreateTexture(...) = 0;
    // virtual std::unique_ptr<IShader> CreateShader(...) = 0;
};