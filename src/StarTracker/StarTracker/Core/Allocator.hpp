#ifndef STARTRACKER_CORE_ALLOCATER_H
#define STARTRACKER_CORE_ALLOCATER_H

#include <cstdlib>

namespace StarTracker::Core {

    struct AllocationStats {

        std::size_t ActiveAllocations;
    };

    class Allocator {

    private:
        static inline AllocationStats allocationStats{};

    public:
        [[nodiscard]] static void* Reserve(std::size_t size) noexcept;
        static void Free(void* block) noexcept;

        [[nodiscard]] static std::size_t GetAllocations() noexcept;
    };
}

#endif // STARTRACKER_CORE_ALLOCATER_H