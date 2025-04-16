#include <cstdlib>
#include <cstring>

#include <fcntl.h>
#include <fmt/format.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "darwin-serial-port.hpp"

namespace arch {

    std::unique_ptr<SerialPort> SerialPort::Create() noexcept {
        return std::make_unique<darwin::DarwinSerialPort>();
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

    namespace darwin {

        DarwinSerialPort::~DarwinSerialPort() noexcept { }

        void DarwinSerialPort::Open(std::string_view port, usize baudRate) noexcept(false) {
            throw SerialException{ "Not implemented!" };
        }

        void DarwinSerialPort::Close() noexcept(false) {
            throw SerialException{ "Not implemented!" };
        }

        usize DarwinSerialPort::Read(u8* buffer, usize size, bool waitForRx) noexcept(false) {
            throw SerialException{ "Not implemented!" };
        }

        usize DarwinSerialPort::Write(u8* buffer, usize size) noexcept(false) {
            throw SerialException{ "Not implemented!" };
        }

        bool DarwinSerialPort::IsOpen() noexcept {
            return false;
        }

        bool DarwinSerialPort::IsGood() noexcept {
            return false;
        }

        usize DarwinSerialPort::Available() noexcept(false) {
            throw SerialException{ "Not implemented!" };
        }
    }// namespace darwin
}// namespace arch
