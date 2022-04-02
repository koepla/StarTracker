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
    	/**
		* @brief Allocates the specified size on the heap
        *        Tracks the allocation in the allocationStats member
		*
		* @return pointer to the allocated memory-block
		* 
		*/
        [[nodiscard]] static void* Reserve(std::size_t size) noexcept;

        /**
		* @brief Deallocates the specified block
        *        Tracks the deallocation in the allocationStats member
		* 
		*/
        static void Free(void* block) noexcept;

        /**
		* @brief Returns the number of active allocations
        *
        * @return number of active allocations
		* 
		*/
        [[nodiscard]] static std::size_t GetAllocations() noexcept;
    };
}

#endif // STARTRACKER_CORE_ALLOCATER_H