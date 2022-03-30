#include "Allocator.hpp"

namespace StarTracker::Core {

    void* Allocator::Reserve(std::size_t size) noexcept {

        auto block = std::malloc(size);
        allocationStats.ActiveAllocations++;
        return block;
    }

    void Allocator::Free(void* block) noexcept {

        std::free(block);
        allocationStats.ActiveAllocations--;
    }

    std::size_t Allocator::GetAllocations() noexcept {

        return allocationStats.ActiveAllocations;
    }
}