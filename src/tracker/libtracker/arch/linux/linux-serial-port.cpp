#include <chrono>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <type_traits>

#include <fcntl.h>
#include <fmt/format.h>
#include <linux/serial.h>
#include <sys/ioctl.h>
#include <termio.h>
#include <unistd.h>

#include "core/application.hpp"
#include "core/core.hpp"
#include "linux-serial-port.hpp"

namespace arch {

    std::unique_ptr<SerialPort> SerialPort::Create() noexcept {
        return std::make_unique<linux::LinuxSerialPort>();
    }

    std::vector<std::string> SerialPort::GetPortNames() noexcept {
        std::vector<std::string> portNames{};
        for (usize i = 0; i < 128; i++) {
            auto portName = fmt::format("/dev/ttyUSB{}", i);
            const auto portDescriptor = open(portName.c_str(), O_RDWR | O_NOCTTY);
            if (portDescriptor < 0) {
                continue;
            }
            portNames.emplace_back(std::move(portName));
            close(portDescriptor);
        }
        return portNames;
    }

    namespace {

        speed_t unixBaudRate(usize baudRate) {
            switch (baudRate) {
                case 110:
                    return B110;
                case 300:
                    return B300;
                case 600:
                    return B600;
                case 1200:
                    return B1200;
                case 2400:
                    return B2400;
                case 4800:
                    return B4800;
                case 9600:
                    return B9600;
                case 19200:
                    return B19200;
                case 38400:
                    return B38400;
                case 57600:
                    return B57600;
                case 115200:
                    return B115200;
                default:
                    LIBTRACKER_ASSERT(false, "Invalid baud");
                    return 0;
            }
        }
    }// namespace

    namespace linux {

        LinuxSerialPort::~LinuxSerialPort() noexcept {
            if (fileDescriptor != -1) {
                close(fileDescriptor);
            }
        }

        void LinuxSerialPort::Open(std::string_view port, usize baudRate) noexcept(false) {
            fileDescriptor = open(port.data(), O_RDWR);
            if (fileDescriptor < 0) {
                throw SerialException{ "Couldn't open port" };
            }

            termios tty{};
            if (tcgetattr(fileDescriptor, &tty) != 0) {
                throw SerialException{ strerror(errno) };
            }

            tty.c_cflag &= ~PARENB;
            tty.c_cflag &= ~CSTOPB;
            tty.c_cflag &= ~CSIZE;
            tty.c_cflag |= CS8;
            tty.c_cflag &= ~CRTSCTS;
            tty.c_cflag |= CREAD | CLOCAL;
            tty.c_lflag &= ~ICANON;
            tty.c_lflag &= ~ECHO;
            tty.c_lflag &= ~ECHOE;
            tty.c_lflag &= ~ECHONL;
            tty.c_lflag &= ~ISIG;
            tty.c_iflag &= ~(IXON | IXOFF | IXANY);
            tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);
            tty.c_oflag &= ~OPOST;
            tty.c_oflag &= ~ONLCR;
            tty.c_cc[VTIME] = 10;
            tty.c_cc[VMIN] = 0;

            const auto unixBaud = unixBaudRate(baudRate);
            cfsetispeed(&tty, unixBaud);
            cfsetospeed(&tty, unixBaud);

            if (tcsetattr(fileDescriptor, TCSANOW, &tty) != 0) {
                throw SerialException{ strerror(errno) };
            }
        }

        void LinuxSerialPort::Close() noexcept(false) {
            if (close(fileDescriptor != 0)) {
                throw SerialException{ "Couldn't close SerialPort" };
            }
            fileDescriptor = -1;
        }

        usize LinuxSerialPort::Read(u8* buffer, usize size, bool waitForRx) noexcept(false) {
            if (!IsOpen()) {
                throw SerialException{ "Port is not open" };
            }
            const auto bytesReceived = read(fileDescriptor, buffer, size);
            if (bytesReceived == -1) {
                LIBTRACKER_ERROR("Couldn't read from SerialPort: {}", strerror(errno));
            }
            return bytesReceived;
        }

        usize LinuxSerialPort::Write(u8* buffer, usize size) noexcept(false) {
            if (!IsOpen()) {
                throw SerialException{ "Port is not open" };
            }
            const auto bytesWritten = write(fileDescriptor, buffer, size);
            if (bytesWritten == -1) {
                LIBTRACKER_ERROR("Couldn't write to SerialPort: {}", strerror(errno));
            }
            return bytesWritten;
        }

        bool LinuxSerialPort::IsOpen() noexcept {
            return fileDescriptor != -1 && IsGood();
        }

        bool LinuxSerialPort::IsGood() noexcept {
            termios tty{};
            return tcgetattr(fileDescriptor, &tty) == 0;
        }

        usize LinuxSerialPort::Available() noexcept(false) {
            int available;
            if (ioctl(fileDescriptor, FIONREAD, &available) != 0) {
                throw SerialException{ strerror(errno) };
            }
            return static_cast<usize>(available);
        }
    }// namespace linux
}// namespace arch
