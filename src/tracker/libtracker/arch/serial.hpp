#ifndef LIBTRACKER_ARCH_SERIAL_H
#define LIBTRACKER_ARCH_SERIAL_H

#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "utility/types.hpp"

namespace arch {

    class SerialException : public std::exception {
    private:
        std::string message;

    public:
        explicit SerialException(std::string_view message) noexcept;

        const char* what() const noexcept override;
    };

    class SerialPort {
    public:
        virtual ~SerialPort() noexcept = default;

        /**
         * @brief Opens the specified COM port
         * @param port name of the COM port
         * @param baudRate baudRate (self explaining)
         * @throws SerialException if the COM port does not exist or could not be opened
         */
        virtual void Open(std::string_view port, usize baudRate) noexcept(false) = 0;

        /**
         * @brief Closes the currently open COM port
         * @throws SerialException if the COM port could not be closed
         */
        virtual void Close() noexcept(false) = 0;

        /**
         * @brief Reads from the currently open COM port
         * @param buffer buffer where the read bytes are moved to
         * @param size number of bytes to read
         * @param waitForRx Wait for at least one incoming byte until read
         * @return number of bytes that where successfully read
         * @throws SerialException if the read fails
         */
        virtual usize Read(u8* buffer, usize size, bool waitForRx = true) noexcept(false) = 0;

        /**
         * @brief Writes to the currently open COM port
         * @param buffer buffer for the to be written bytes
         * @param size number of bytes to write
         * @return number of bytes that where successfully written
         * @throws SerialException if the write fails
         */
        virtual usize Write(uint8_t* buffer, usize size) noexcept(false) = 0;

        /**
         * @brief Checks if the COM port is open
         * @return bool if the port is open or not
         */
        virtual bool IsOpen() noexcept = 0;

        /**
         * @brief Checks if the COM port still exists
         * @return bool if the COM port exists or not
         */
        virtual bool IsGood() noexcept = 0;

        /**
         * @brief Number of bytes that are in the serial buffer of the COM port
         * @return number of bytes
         * @throws SerialException if the port is not open of the status of the port could not be checked
         */
        virtual usize Available() noexcept(false) = 0;

    public:
        static std::unique_ptr<SerialPort> Create() noexcept;
        static std::vector<std::string> GetPortNames() noexcept;
    };
}// namespace arch

#endif// LIBTRACKER_ARCH_SERIAL_H