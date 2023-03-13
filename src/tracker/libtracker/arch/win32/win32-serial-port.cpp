#include <array>

#include <fmt/format.h>

#include "core/core.hpp"
#include "win32-serial-port.hpp"


namespace arch {

    std::unique_ptr<SerialPort> SerialPort::Create() noexcept {
        return std::make_unique<win32::Win32SerialPort>();
    }

    std::vector<std::string> SerialPort::GetPortNames() noexcept {
        std::vector<std::string> portNames{};
        std::array<char, 4096> target{};

        for (usize i = 0; i < 255; i++) {
            auto portName = fmt::format("COM{}", i);
            if (QueryDosDeviceA(portName.c_str(), target.data(), target.size()) != 0) {
                portNames.emplace_back(std::move(portName));
            }
            if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
                LIBTRACKER_ASSERT(false, "Insufficient buffer size");
            }
        }
        return portNames;
    }

    namespace {

        std::string prefixPort(std::string_view port) {
            using namespace std::string_view_literals;
            if (port.substr(0, 4) != R"(\\.\)"sv) {
                return fmt::format(R"(\\.\{})", port);
            }
            return std::string{ port };
        }

        bool isValidBaudRate(usize baudRate) {
            switch (baudRate) {
                case CBR_110:
                case CBR_300:
                case CBR_600:
                case CBR_1200:
                case CBR_2400:
                case CBR_4800:
                case CBR_9600:
                case CBR_14400:
                case CBR_19200:
                case CBR_38400:
                case CBR_57600:
                case CBR_115200:
                case CBR_128000:
                case CBR_256000:
                    return true;
                default:
                    return false;
            }
        }
    }// namespace

    namespace win32 {

        Win32SerialPort::~Win32SerialPort() noexcept {
            if (file != INVALID_HANDLE_VALUE) {
                CloseHandle(file);
            }
        }

        void Win32SerialPort::Open(std::string_view port, usize baudRate) noexcept(false) {
            const auto prefixed = prefixPort(port);
            file = CreateFileA(prefixed.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING,
                               FILE_ATTRIBUTE_NORMAL, nullptr);

            if (file == INVALID_HANDLE_VALUE) {
                throw SerialException{ "Couldn't open Serial Port" };
            }

            DCB dcb{};
            if (!GetCommState(file, &dcb)) {
                throw SerialException{ "GetCommState failed" };
            }

            if (!isValidBaudRate(baudRate)) {
                throw SerialException{ "Invalid baud rate" };
            }
            // 8N1
            dcb.BaudRate = baudRate;
            dcb.ByteSize = 8;
            dcb.StopBits = ONESTOPBIT;
            dcb.Parity = NOPARITY;

            if (!SetCommState(file, &dcb)) {
                throw SerialException{ "SetCommState failed" };
            }

            if (!SetCommMask(file, EV_RXCHAR)) {
                throw SerialException{ "SetCommMask failed" };
            }

            COMMTIMEOUTS timeouts{};
            timeouts.ReadIntervalTimeout = 1;
            timeouts.ReadTotalTimeoutConstant = 1;
            timeouts.ReadTotalTimeoutMultiplier = 1;
            timeouts.WriteTotalTimeoutConstant = 1;
            timeouts.WriteTotalTimeoutMultiplier = 1;

            if (!SetCommTimeouts(file, &timeouts)) {
                throw SerialException{ "SetCommTimeouts failed" };
            }
        }

        void Win32SerialPort::Close() noexcept(false) {
            if (CloseHandle(file) == 0) {
                throw SerialException{ "Couldn't close SerialPort" };
            }
            file = INVALID_HANDLE_VALUE;
        }

        usize Win32SerialPort::Read(u8* buffer, usize size, bool waitForRx) noexcept(false) {
            DWORD bytesRead{};
            if (!IsOpen()) {
                throw SerialException{ "Port is not open" };
            }

            if (waitForRx) {
                DWORD mask = EV_RXCHAR;
                WaitCommEvent(file, &mask, nullptr);
            }

            if (!ReadFile(file, buffer, static_cast<DWORD>(size), &bytesRead, nullptr)) {
                throw SerialException{ "Couldn't read from port" };
            }

            return bytesRead;
        }

        usize Win32SerialPort::Write(u8* buffer, usize size) noexcept(false) {
            DWORD bytesWritten{};
            if (!IsOpen()) {
                throw SerialException{ "Port is not open" };
            }

            if (!WriteFile(file, buffer, static_cast<DWORD>(size), &bytesWritten, nullptr)) {
                throw SerialException{ "Couldn't write to port" };
            }

            return bytesWritten;
        }

        bool Win32SerialPort::IsOpen() noexcept {
            return file != INVALID_HANDLE_VALUE && IsGood();
        }

        bool Win32SerialPort::IsGood() noexcept {
            // In order to check if the COM-Port is still valid, we can check its status
            COMSTAT stat{};
            if (ClearCommError(file, nullptr, &stat)) {
                return true;
            }

            file = INVALID_HANDLE_VALUE;
            return false;
        }

        usize Win32SerialPort::Available() noexcept(false) {
            if (!IsOpen()) {
                throw SerialException{ "Port is not open" };
            }

            COMSTAT stat{};
            if (!ClearCommError(file, nullptr, &stat)) {
                throw SerialException{ "Couldn't check status of serial port." };
            }

            return stat.cbInQue;
        }
    }// namespace win32
}// namespace arch
