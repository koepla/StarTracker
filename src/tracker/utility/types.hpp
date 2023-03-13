#ifndef UTILITY_TYPES_H
#define UTILITY_TYPES_H

/**
 * Collection of common fixed-size basic types
 * @author Elias Engelbert Plank
 * @date 2023/01/11
 */

#include <cfloat>
#include <cstdint>
#include <cstring>
#include <memory>
#include <string>
#include <string_view>
#include <array>

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;
using usize = std::size_t;

using s8 = std::int8_t;
using s16 = std::int16_t;
using s32 = std::int32_t;
using s64 = std::int64_t;

using f32 = float;
using f64 = double;

using namespace std::string_literals;
using namespace std::string_view_literals;

namespace utility {

    /**
     * Owning buffer that initializes, or takes ownership of memory
     * Frees memory when out of scope
     */
    class OwningBuffer {
    public:
        /**
         * Allocates a buffer of `initialSize` bytes on the heap
         * @param initialSize Size of the memory buffer
         */
        explicit OwningBuffer(usize initialSize) noexcept;

        /**
         * Takes ownership of the specified rawMemory, which means that the memory is freed when the object goes out of scope
         * @param rawMemory Pointer to the memory
         * @param initialSize Size of the memory block
         */
        explicit OwningBuffer(u8 *rawMemory, usize initialSize) noexcept;

        /**
         * Copy the memory of the other buffer
         * @param other Other buffer
         */
        OwningBuffer(const OwningBuffer &other) noexcept;

        /**
         * Move the memory of the other buffer
         * @param other Other buffer
         */
        OwningBuffer(OwningBuffer &&other) noexcept;

        /**
         * Frees the memory
         */
        ~OwningBuffer() noexcept;

        /**
         * Interpret the internal memory as the specified type, this is just a wrapper for reinterpret_cast
         * @tparam UnderlyingPointerType Type for cast
         * @return Pointer to the memory
         */
        template<typename UnderlyingPointerType = u8>
        inline UnderlyingPointerType *Memory() const noexcept {
            return reinterpret_cast<UnderlyingPointerType *>(memory);
        }

        /**
         * Size of the memory in bytes
         * @return size
         */
        usize GetSize() const noexcept;

    private:
        u8 *memory;
        usize size;
    };
}// namespace utility

#endif// UTILITY_TYPES_H