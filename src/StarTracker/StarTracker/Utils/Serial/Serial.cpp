#include "Serial.hpp"

namespace StarTracker::Utils::Serial {

	SerialException::SerialException(std::string_view message) noexcept : message{ message } {

	}

	const char* SerialException::what() const noexcept {

		return message.data();
	}

	SerialPort::SerialPort() noexcept : hCom{ nullptr }, dwEventMask{ EV_RXCHAR }, fileName{} {

	}

	SerialPort::~SerialPort() noexcept(false) {

		if (IsOpen()) {

			Close();
		}
	}

	void SerialPort::Open(const std::string& port, uint32_t baudRate) noexcept(false) {

		const auto prefixed = prefixPort(port);
		
		hCom = CreateFileA(prefixed.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (hCom == INVALID_HANDLE_VALUE) {

			fileName = prefixed;
			throw SerialException{ "Couldn't open Serial Port " + prefixed };
		}

		setBaudRate(baudRate);
		initReceiveMask();
		initTimeouts();
	}

	void SerialPort::Close() noexcept(false) {

		if (CloseHandle(hCom) == 0) {

			throw SerialException{ "Couldn't close Serial Port" };
		}

		hCom = INVALID_HANDLE_VALUE;
	}

	uint32_t SerialPort::Read(uint8_t* buffer, uint32_t bytes2read, bool waitForRx) noexcept(false) {

		// Number of bytes read
		DWORD dwRead{};

		// Check if port is open
		if (!IsOpen()) {

			throw SerialException{ "Port is not open" };
		}

		if (waitForRx) {

			WaitComm();
		}

		// Read operation
		if (!ReadFile(hCom, reinterpret_cast<LPVOID>(buffer), static_cast<DWORD>(bytes2read), &dwRead, nullptr)) {

			throw SerialException{ "Couldn't read from port" };
		}
		else {

			return static_cast<uint32_t>(dwRead);
		}
	}

	uint32_t SerialPort::Write(uint8_t* buffer, uint32_t bytes2write) noexcept(false) {

		// Number of bytes written
		DWORD dwWritten{};

		// Check if port is open
		if (!IsOpen()) {

			throw SerialException{ "Port is not open" };
		}

		// Write operation
		if (!WriteFile(hCom, reinterpret_cast<LPVOID>(buffer), static_cast<DWORD>(bytes2write), &dwWritten, nullptr)) {

			throw SerialException{ "Couldn't write to port" };
		}
		else {

			return static_cast<uint32_t>(dwWritten);
		}
	}

	bool SerialPort::IsOpen() noexcept {

		return (hCom != INVALID_HANDLE_VALUE) && IsGood();
	}

	bool SerialPort::IsGood() noexcept {

		COMSTAT stat{};

		// In order to check if the COM-Port is still valid, we can check its status
		if (ClearCommError(hCom, nullptr, &stat)) {

			return true;
		}
		else {

			hCom = INVALID_HANDLE_VALUE;
			return false;
		}
	}

	void SerialPort::WaitComm() noexcept {

		WaitCommEvent(hCom, &dwEventMask, nullptr);
	}

	uint32_t SerialPort::Available() noexcept(false)
	{
		if (!IsOpen()) {

			throw SerialException{ "Port is not open" };
		}

		COMSTAT stat{};
		if (!ClearCommError(hCom, nullptr, &stat)) {

			throw SerialException{ "Cannot check status of serial port." };
		}

		return static_cast<uint32_t>(stat.cbInQue);
	}

	inline std::string SerialPort::prefixPort(const std::string& port) noexcept {

		if (port.find("\\\\.\\") != std::string::npos) {

			return port;
		}
		else {

			return "\\\\.\\" + port;
		}
	}

	void SerialPort::setBaudRate(uint32_t baudRate) noexcept(false) {

		DWORD baud{ 0 };

		switch (baudRate) {
		case 110: baud = CBR_110; break;
		case 300: baud = CBR_300; break;
		case 600: baud = CBR_600; break;
		case 1200: baud = CBR_1200; break;
		case 2400: baud = CBR_2400; break;
		case 4800: baud = CBR_4800; break;
		case 9600: baud = CBR_9600; break;
		case 14400: baud = CBR_14400; break;
		case 19200: baud = CBR_19200; break;
		case 38400: baud = CBR_38400; break;
		case 57600: baud = CBR_57600; break;
		case 115200: baud = CBR_115200; break;
		case 128000: baud = CBR_128000; break;
		case 256000: baud = CBR_256000; break;

		default: {

			throw SerialException{ "Invalid baudRate" };
		}
		}

		DCB dcb{ 0 };

		FillMemory(&dcb, sizeof(dcb), 0);
		if (!GetCommState(hCom, &dcb)) {

			throw SerialException{ "Couldn't set baudRate" };
		}

		// 8N1
		dcb.BaudRate = baud;
		dcb.ByteSize = 8;
		dcb.StopBits = ONESTOPBIT;
		dcb.Parity = NOPARITY;

		if (!SetCommState(hCom, &dcb)) {

			throw SerialException{ "Couldn't set baudRate" };
		}
	}

	void SerialPort::initReceiveMask() noexcept(false) {
		
		dwEventMask = EV_RXCHAR;

		if (!SetCommMask(hCom, dwEventMask)) {

			throw SerialException{ "Couldn't set Comm mask" };
		}
	}

	void SerialPort::initTimeouts() noexcept(false) {

		COMMTIMEOUTS timeouts = { 0 };
		timeouts.ReadIntervalTimeout = 1;
		timeouts.ReadTotalTimeoutConstant = 1;
		timeouts.ReadTotalTimeoutMultiplier = 1;
		timeouts.WriteTotalTimeoutConstant = 1;
		timeouts.WriteTotalTimeoutMultiplier = 1;

		if (!SetCommTimeouts(hCom, &timeouts)) {

			throw SerialException{ "Couldn't set timeouts" };
		}
	}

	std::vector<std::string> SerialPort::GetPortNames() noexcept(false) {

		std::vector<std::string> ports{};
		char target[4096];

		for (int i = 0; i < 255; i++) {

			std::string p = "COM" + std::to_string(i);
			if (QueryDosDeviceA(p.c_str(), target, 4096) != 0) {

				ports.push_back(p);
			}
			if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {

				throw SerialException{ "Insufficient buffer size (programmer's fault)" };
			}
		}

		return ports;
	}
}
