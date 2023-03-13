#ifndef STARTRACKER_ARCH_LINUX_SERIAL_H
#define STARTRACKER_ARCH_LINUX_SERIAL_H

#include "arch/serial.hpp"

namespace arch::linux {

    class LinuxSerialPort : public SerialPort {
    private:
        int fileDescriptor = -1;

    public:
        LinuxSerialPort() noexcept = default;
        ~LinuxSerialPort() noexcept override;
        /**
         * @brief Opens the specified COM port
         * @param port name of the COM port
         * @param baudRate baudRate (self explaining)
         * @throws SerialException if the COM port does not exist or could not be opened
         */
        void Open(std::string_view port, usize baudRate) noexcept(false) override;

        /**
         * @brief Closes the currently open COM port
         * @throws SerialException if the COM port could not be closed
         */
        void Close() noexcept(false) override;

        /**
         * @brief Reads from the currently open COM port
         * @param buffer buffer where the read bytes are moved to
         * @param size number of bytes to read
         * @param waitForRx Wait for at least one incoming byte until read
         * @return number of bytes that where successfully read
         * @throws SerialException if the read fails
         */
        usize Read(u8* buffer, usize size, bool waitForRx) noexcept(false) override;

        /**
         * @brief Writes to the currently open COM port
         * @param buffer buffer for the to be written bytes
         * @param size number of bytes to write
         * @return number of bytes that where successfully written
         * @throws SerialException if the write fails
         */
        usize Write(u8* buffer, usize size) noexcept(false) override;

        /**
         * @brief Checks if the COM port is open
         * @return bool if the port is open or not
         */
        bool IsOpen() noexcept override;

        /**
         * @brief Checks if the COM port still exists
         * @return bool if the COM port exists or not
         */
        bool IsGood() noexcept override;

        /**
         * @brief Number of bytes that are in the serial buffer of the COM port
         * @return number of bytes
         * @throws SerialException if the port is not open of the status of the port could not be checked
         */
        usize Available() noexcept(false) override;
    };
}// namespace arch::linux


#endif// STARTRACKER_ARCH_LINUX_SERIAL_H