#include "types.hpp"

namespace utility {

    OwningBuffer::OwningBuffer(usize initialSize) noexcept : size(initialSize) {
        memory = new u8[size];
    }

    OwningBuffer::OwningBuffer(u8* rawMemory, usize initialSize) noexcept : memory(rawMemory), size(initialSize) { }

    OwningBuffer::OwningBuffer(const OwningBuffer& other) noexcept {
        size = other.size;
        memory = new u8[size];
        std::memcpy(memory, other.memory, size);
    }

    OwningBuffer::OwningBuffer(OwningBuffer&& other) noexcept {
        std::swap(memory, other.memory);
        std::swap(size, other.size);
        other.memory = nullptr;
        other.size = 0;
    }

    OwningBuffer::~OwningBuffer() noexcept {
        delete[] memory;
        size = 0;
    }

    usize OwningBuffer::GetSize() const noexcept {
        return size;
    }
}// namespace utility
