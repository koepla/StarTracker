#ifndef STARTRACKER_SERIAL_PACKAGE_H
#define STARTRACKER_SERIAL_PACKAGE_H

#include <StarTracker/Core/Assert.hpp>

#include <vector>
#include <string>

namespace StarTracker::Utils::Serial {

    enum class Command : uint8_t {

        NONE = (1 << 0),
        WAKEUP = (1 << 1),
        SLEEP = (1 << 2),
        MOVE = (1 << 3),
        CONF = (1 << 4),
        ORIGIN = (1 << 5),
        ACK = (1 << 6)
    };

    struct Header {

        __declspec(align(1)) Command Flag;
        __declspec(align(1)) uint8_t Size;
    };

    template <uint16_t N>
    class Package {

    private:
        static constexpr uint16_t BUFFER_SIZE = N - sizeof(Header);

        __declspec(align(1)) Header header;
        __declspec(align(1)) uint8_t buffer[BUFFER_SIZE];

    public:
        Package() noexcept : header{ Command::NONE, 0 } {

            static_assert(BUFFER_SIZE >= 0, "Buffer size must not be negative!");

            std::memset(buffer, 0, BUFFER_SIZE);
        }

        Package(Command flag) noexcept : header{ flag, 0 } {

            static_assert(BUFFER_SIZE >= 0, "Buffer size must not be negative!");

            std::memset(buffer, 0, BUFFER_SIZE);
        }

        /**
        * @brief Clears the buffer of the package
        *
        * @return reference to this for chained function calls
        *
        */
        Package& Clear() noexcept {

            std::memset(buffer, 0, BUFFER_SIZE);
            header.Size = 0;
            header.Flag = Command::NONE;

            return *this;
        }

        /**
        * @brief Sets the specified flag in the package header
        *
        * @param flag command flag that should be set
        *
        * @return reference to this for chained function calls
        *
        */
        Package& SetFlag(Command flag) noexcept {

            header.Flag = flag;

            return *this;
        }

        /**
        * @brief Pushes the specified data into the byte buffer
        *
        * @param data as const reference
        *
        * @return reference to this for chained function calls
        *
        */
        template <typename T>
        Package& Push(const T& data) noexcept {

            if (sizeof(T) + header.Size > BUFFER_SIZE) {

                ASSERT(false && "Push would exceed remaining buffer size");
            }

            uint8_t currentSize = header.Size;
            std::memcpy(buffer + currentSize, &data, sizeof(T));
            header.Size += sizeof(T);

            return *this;
        }

        /**
        * @brief Pushes the specified data array into the byte buffer
        *
        * @param data pointer to base of data array
        *
        * @param count number of elements
        *
        * @return reference to this for chained function calls
        *
        */
        template <typename T>
        Package& PushRange(const T* data, uint32_t count) noexcept {

            if ((sizeof(T) * count) + header.Size > BUFFER_SIZE) {

                ASSERT(false && "Push would exceed remaining buffer size");
            }

            uint8_t currentSize = header.Size;
            std::memcpy(buffer + currentSize, data, count * sizeof(T));
            header.Size += count * sizeof(T);
            return *this;
        }

        /**
        * @brief Reads from the package buffer
        *
        * @param index for the read operation
        *
        * @return read value
        *
        */
        template <typename T>
        [[nodiscard]] T Read(uint32_t index) noexcept(false) {

            if ((index) * sizeof(T) > (BUFFER_SIZE - sizeof(T))) {

                ASSERT(false && "Index out of range");
            }

            return *reinterpret_cast<T*>(buffer + index * sizeof(T));
        }

        /**
        * @brief Reads array from the package buffer
        *
        * @param offset for the read operation
        *
        * @return pointer to offset
        *
        */
        template <typename T>
        [[nodiscard]] T* ReadRange(uint32_t offset) noexcept(false) {

            if ((offset) * sizeof(T) > (BUFFER_SIZE - sizeof(T))) {

                ASSERT(false && "Offset out of range");
            }

            return reinterpret_cast<T*>(buffer + offset * sizeof(T));
        }

        /**
        * @brief Number of bytes that fill up the package buffer
        *
        * @return number of bytes
        *
        */
        [[nodiscard]] size_t GetSize() const noexcept {

            return static_cast<size_t>(header.Size);
        }

        /**
        * @brief Flag that is in the package header
        *
        * @return command flag
        *
        */
        [[nodiscard]] Command GetFlag() const noexcept {

            return header.Flag;
        }
    };

    typedef Package<8>   Pack8;
    typedef Package<16>  Pack16;
    typedef Package<32>  Pack32;
    typedef Package<64>  Pack64;
    typedef Package<128> Pack128;
}

#endif // STARTRACKER_SERIAL_PACKAGE_H