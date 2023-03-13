#ifndef STARTRACKER_FIRMWARE_PACKAGE_H
#define STARTRACKER_FIRMWARE_PACKAGE_H

#include <Arduino.h>

#pragma pack(push, 1)

namespace StarTracker {

    /**
     * @brief Protocol Flags
     */
    enum class Command : uint8_t {
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
        uint8_t Size;
    };

    /**
     * @brief Package
     * @details Package is a data structure that is used to communicate between the host and the microcontroller.
     */
    template<int16_t Size>
    struct Package {
        PackageHeader Header;
        uint8_t Buffer[Size - sizeof(Header)];

        Package() noexcept : Header{ Command::None, 0 } {
            static_assert(Size - sizeof(Header) >= 0, "Buffer size must not be less than zero");
            memset(Buffer, 0, Size - sizeof(Header));
        }

        Package(Command flag) noexcept : Header{ flag, 0 } {
            static_assert(Size - sizeof(Header) >= 0, "Buffer size must not be less than zero");
            memset(Buffer, 0, Size - sizeof(Header));
        }

        /**
         * @brief Clears the package
         * @return Package&
         */
        Package& Clear() noexcept {
            memset(Buffer, 0, Size - sizeof(Header));
            Header.Size = 0;
            Header.Flag = Command::None;
            return *this;
        }

        /**
         * @brief Sets the flag
         * @param flag Flag
         * @return Package&
         */
        Package& SetFlag(Command flag) noexcept {
            Header.Flag = flag;
            return *this;
        }

        /**
         * @brief Push data of type T into the buffer
         * @tparam T Type
         * @param data Data
         * @return Package&
         */
        template<typename T>
        Package& Push(const T& data) noexcept {
            memcpy(Buffer + Header.Size, &data, sizeof(T));
            Header.Size += sizeof(T);
            return *this;
        }

        /**
         * @brief Push a range of data into the buffer
         * @tparam T Type
         * @param data Data
         * @return Package&
         */
        template<typename T>
        Package& PushRange(const T* data, uint8_t count) noexcept {
            memcpy(Buffer + Header.Size, data, count * sizeof(T));
            Header.Size += count * sizeof(T);
            return *this;
        }

        /**
         * @brief Reads a value from the buffer
         * @tparam T Type
         * @param index Index in the buffer
         * @return T Value
         */
        template<typename T>
        T Read(uint8_t index) noexcept {
            return *reinterpret_cast<T*>(Buffer + index * sizeof(T));
        }

        /**
         * @brief Reads a range of values from the buffer
         * @tparam T Type
         * @param index Offset in the buffer
         * @return T* Values
         */
        template<typename T>
        T* ReadRange(uint8_t offset) noexcept {
            return reinterpret_cast<T*>(Buffer + offset * sizeof(T));
        }

        /**
         * @brief Checks how many bytes are used in the buffer
         * @return size_t
         */
        size_t GetSize() const noexcept {
            return Header.Size;
        }

        /**
         * @brief Returns the flag
         * @return Flag
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
}// namespace StarTracker

#pragma pack(pop)

#endif// STARTRACKER_FIRMWARE_PACKAGE_H