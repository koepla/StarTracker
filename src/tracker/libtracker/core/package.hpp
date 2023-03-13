#ifndef LIBTRACKER_SERIAL_PACKAGE_H
#define LIBTRACKER_SERIAL_PACKAGE_H

#include <cstdint>
#include <cstring>
#include <memory>

#include "core.hpp"
#include "utility/types.hpp"

#pragma pack(push, 1)

/**
 * @brief Protocol Flags
 */
enum class Command : u8 {
    None = (1 << 0),
    Wakeup = (1 << 1),
    Sleep = (1 << 2),
    Move = (1 << 3),
    Configure = (1 << 4),
    Origin = (1 << 5),
    Ack = (1 << 6),
    Advance = (1 << 7)
};

/**
 *	@brief Package Header
 */
struct PackageHeader {
    Command Flag;
    u8 Size;
};

/**
 * @brief Package
 * @details Package is a data structure that is used to communicate between the host and the microcontroller.
 */
template<s16 Size>
struct Package {
    PackageHeader Header;
    u8 Buffer[Size - sizeof(Header)]{};

    Package() noexcept : Header{ Command::None, 0 } {
        static_assert(Size - sizeof(Header) >= 0, "Buffer size must not be negative!");
        std::memset(Buffer, 0, Size - sizeof(Header));
    }

    explicit Package(Command flag) noexcept : Header{ flag, 0 } {
        static_assert(Size - sizeof(Header) >= 0, "Buffer size must not be negative!");
        std::memset(Buffer, 0, Size - sizeof(Header));
    }

    /**
     * @brief Clears the buffer of the package
     * @return reference to this for chained function calls
     */
    Package& Clear() noexcept {
        std::memset(Buffer, 0, Size - sizeof(Header));
        Header.Size = 0;
        Header.Flag = Command::None;
        return *this;
    }

    /**
     * @brief Sets the specified flag in the package header
     * @param flag Command flag that should be set
     * @return reference to this for chained function calls
     */
    Package& SetFlag(Command flag) noexcept {
        Header.Flag = flag;
        return *this;
    }

    /**
     * @brief Pushes the specified data into the byte buffer
     * @param data as const reference
     * @return reference to this for chained function calls
     */
    template<typename T>
    Package& Push(const T& data) noexcept {
        if (sizeof(T) + Header.Size > Size - sizeof(Header)) {
            LIBTRACKER_ASSERT(false, "Push would exceed remaining buffer size");
        }
        std::memcpy(Buffer + Header.Size, &data, sizeof(T));
        Header.Size += sizeof(T);
        return *this;
    }

    /**
     * @brief Pushes the specified data array into the byte buffer
     * @param data pointer to base of data array
     * @param count number of elements
     * @return reference to this for chained function calls
     */
    template<typename T>
    Package& PushRange(const T* data, u32 count) noexcept {
        if ((sizeof(T) * count) + Header.Size > Size - sizeof(Header)) {
            LIBTRACKER_ASSERT(false, "Push would exceed remaining buffer size");
        }
        std::memcpy(Buffer + Header.Size, data, count * sizeof(T));
        Header.Size += count * sizeof(T);
        return *this;
    }

    /**
     * @brief Reads from the package buffer
     * @param index for the read operation
     * @return read value
     */
    template<typename T>
    T Read(u32 index) noexcept(false) {
        if (index * sizeof(T) > (Size - sizeof(Header) - sizeof(T))) {
            LIBTRACKER_ASSERT(false, "Index out of range");
        }
        return *reinterpret_cast<T*>(Buffer + index * sizeof(T));
    }

    /**
     * @brief Reads array from the package buffer
     * @param offset for the read operation
     * @return pointer to offset
     */
    template<typename T>
    T* ReadRange(u32 offset) noexcept(false) {
        if (offset * sizeof(T) > (Size - sizeof(Header) - sizeof(T))) {
            LIBTRACKER_ASSERT(false, "Offset out of range");
        }
        return reinterpret_cast<T*>(Buffer + offset * sizeof(T));
    }

    /**
     * @brief Number of bytes that fill up the package buffer
     * @return number of bytes
     */
    usize GetSize() const noexcept {
        return Header.Size;
    }

    /**
     * @brief Flag that is in the package header
     * @return command flag
     */
    Command GetFlag() const noexcept {
        return Header.Flag;
    }
};

using Pack8 = Package<8>;
using Pack16 = Package<16>;
using Pack32 = Package<32>;
using Pack64 = Package<64>;
using Pack128 = Package<128>;

const char* CommandToString(Command command) noexcept;

#pragma pack(pop)

#endif// LIBTRACKER_SERIAL_PACKAGE_H