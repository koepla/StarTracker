#include "Serial.hpp"

#include <chrono>

namespace Protocol {

	SerialException::SerialException(std::string&& message) noexcept : message(message) {


	}

	const char* SerialException::what() const noexcept {

		return this->message.c_str();
	}

	Serial::Serial() : hCom(nullptr), isOpen(false), dwEventMask(EV_RXCHAR | EV_ERR) {

	}

	Serial::~Serial() {

		if (IsOpen()) {

			Close();
		}
	}

	void Serial::Open(const std::string& port, uint32_t baudrate) noexcept(false) {

		auto prefixed = prefixPort(port);
		
		hCom = CreateFileA(prefixed.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

		if (hCom == INVALID_HANDLE_VALUE) {

			isOpen = false;
			throw SerialException("Couldn't open Serial Port " + prefixed);
		}
		else {
			
			isOpen = true;
		}

		setBaudrate(baudrate);
		initReceiveMask();
		initTimeouts(64);
	}

	void Serial::Close() noexcept(false) {

		if (CloseHandle(hCom) == 0) {

			throw SerialException("Couldn't close Serial Port");
		}
		isOpen = false;
	}

	uint32_t Serial::Read(uint8_t* buffer, uint32_t bytes2read) noexcept(false) {

		initTimeouts(bytes2read);

		// Number of bytes read
		DWORD dwread;

		// Check if port is open
		if (!IsOpen()) {

			throw SerialException("Port is not open");
		}

		WaitComm();

		// Read operation
		if (!ReadFile(hCom, reinterpret_cast<LPVOID>(buffer), static_cast<DWORD>(bytes2read), &dwread, NULL)) {

			throw SerialException("Couldn't read from port");
		}
		else {

			return static_cast<uint32_t>(dwread);
		}
	}

	uint32_t Serial::Write(uint8_t* buffer, uint32_t bytes2write) noexcept(false) {

		// Number of bytes written
		DWORD dwwritten;

		// Check if port is open
		if (!IsOpen()) {

			throw SerialException("Port is not open");
		}

		// Write operation
		if (!WriteFile(hCom, reinterpret_cast<LPVOID>(buffer), static_cast<DWORD>(bytes2write), &dwwritten, NULL)) {

			throw SerialException("Couldn't write to port");
		}
		else {

			return static_cast<uint32_t>(dwwritten);
		}
	}

	bool Serial::IsOpen() const noexcept {

		return isOpen && (hCom != INVALID_HANDLE_VALUE);
	}

	void Serial::WaitComm() noexcept {

		WaitCommEvent(hCom, &dwEventMask, NULL);
	}

	uint32_t Serial::Available() noexcept(false)
	{
		if (!IsOpen()) {

			throw SerialException("Port is not open");
			return 0;
		}

		COMSTAT stat;
		if (!ClearCommError(hCom, NULL, &stat)) {

			throw SerialException("Cannot check status of serial port.");
			return 0;
		}

		return static_cast<uint32_t>(stat.cbInQue);
	}

	inline std::string Serial::prefixPort(const std::string& port) {

		if (strncmp(port.c_str(), "\\\\.\\", 4) == 0) {

			return port;
		}
		else {

			return "\\\\.\\" + port;
		}
	}

	void Serial::setBaudrate(uint32_t baudrate) {

		DWORD baud = 0;

		switch (baudrate) {
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

			throw SerialException("Invalid baudrate");
		}
		}

		DCB dcb = { 0 };

		FillMemory(&dcb, sizeof(dcb), 0);
		if (!GetCommState(hCom, &dcb)) {

			throw SerialException("Couldn't set baudrate");
		}

		// 8N1
		dcb.BaudRate = baud;
		dcb.ByteSize = 8;
		dcb.StopBits = ONESTOPBIT;
		dcb.Parity = NOPARITY;

		if (!SetCommState(hCom, &dcb)) {

			throw SerialException("Couldn't set baudrate");
		}
	}

	void Serial::initReceiveMask() {
		
		dwEventMask = EV_RXCHAR;

		if (!SetCommMask(hCom, dwEventMask)) {

			throw SerialException("Couldn't set Comm mask");
		}
	}

	void Serial::initTimeouts(uint32_t charCount) {

		COMMTIMEOUTS timeouts = { 0 };
		timeouts.ReadIntervalTimeout = 100;
		timeouts.ReadTotalTimeoutConstant = 100;
		timeouts.ReadTotalTimeoutMultiplier = charCount;
		timeouts.WriteTotalTimeoutConstant = 100;
		timeouts.WriteTotalTimeoutMultiplier = charCount;

		if (!SetCommTimeouts(hCom, &timeouts)) {

			throw SerialException("Couldn't set timeouts");
		}
	}

	std::vector<std::string> Serial::GetPortNames()
	{
		std::vector<std::string> ports;
		char target[4096];

		for (int i = 0; i < 255; i++) {

			std::string p = "COM" + std::to_string(i);
			if (QueryDosDeviceA(p.c_str(), target, 4096) != 0) {

				ports.push_back(p);
			}
			if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {

				throw SerialException("Insufficient buffer size (programmer's fault)");
			}
		}

		return ports;
	}
}
