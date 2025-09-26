#pragma once
#include <cstdint>

// Uma struct para descrever o buffer que queremos criar.
// É melhor do que passar 10 parâmetros para uma função.
struct BufferDesc {
    uint32_t size = 0;
    enum class Usage { Vertex, Index, Uniform } usage = Usage::Vertex;
    const void* initialData = nullptr;
};

class IBuffer {
public:
    virtual ~IBuffer() = default;
    virtual void Update(const void* data, uint32_t size, uint32_t offset) = 0;
    virtual const BufferDesc& GetDesc() const = 0;
};