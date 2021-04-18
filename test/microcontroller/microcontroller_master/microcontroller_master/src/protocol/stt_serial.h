#pragma once
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>

/*
*	For reference, see https://docs.microsoft.com/en-us/previous-versions/ff802693(v=msdn.10)?redirectedfrom=MSDN
*/

class stt_serial_exception : public std::exception {

private:
	std::string message;

public:

	stt_serial_exception(std::string&& message) noexcept : message(message) {
		
	}

	const char* what() const noexcept override {

		return this->message.c_str();
	}
};

class stt_serial {

private:
	HANDLE hcomm;
	DWORD dweventmask;
	bool isopen;

public:

	stt_serial() : hcomm(nullptr), isopen(false) {

		this->dweventmask = EV_RXCHAR;
	}

	~stt_serial() {

		this->close();
	}
	/*
	*	Opens the specified port
	*	Arguments are portname and baudrate
	*/
	void open(const std::string& port, uint32_t baudrate) noexcept(false) {

		std::string prefixed = this->prefix_port(port);
		this->hcomm = CreateFileA(prefixed.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

		if (this->hcomm == INVALID_HANDLE_VALUE) {

			this->isopen = false;
			throw stt_serial_exception("Couldn't open Serial Port " + prefixed);
		}
		else {
			this->isopen = true;
		}

		this->set_baudrate(baudrate);
		this->init_recievemask();
		this->init_commtimeout();
	}

	/*
	*	Closes the currently open Port
	*/
	void close() noexcept(false) {

		if (CloseHandle(this->hcomm) == 0) {

			throw stt_serial_exception("Couldn't close Serial Port");
		}
		this->isopen = false;
	}

	/*
	*	Reads from currently open Port
	*	Takes byte-buffer and bytes to reader
	*/
	uint32_t read(uint8_t* buffer, uint32_t bytes2read) noexcept(false) {

		// Number of bytes read
		DWORD dwread;

		// Check if port is open
		if (!this->is_open()) {
			throw stt_serial_exception("Port is not open");
		}

		this->rx_available();

		// Read operation
		if (!ReadFile(this->hcomm, reinterpret_cast<LPVOID>(buffer), static_cast<DWORD>(bytes2read), &dwread, NULL)) {

			throw stt_serial_exception("Couldn't read from port");			
		}
		else {

			return static_cast<uint32_t>(dwread);
		}		
	}

	/*
	*	Writes to currently open Port
	*	Takes byte-buffer and bytes to write
	*/
	uint32_t write(uint8_t* buffer, uint32_t bytes2write) noexcept(false) {

		// Number of bytes written
		DWORD dwwritten;

		// Check if port is open
		if (!this->is_open()) {
			throw stt_serial_exception("Port is not open");
		}

		// Write operation
		if (!WriteFile(this->hcomm, reinterpret_cast<LPVOID>(buffer), static_cast<DWORD>(bytes2write), &dwwritten, NULL)) {

			throw stt_serial_exception("Couldn't write to port");
		}
		else {
			return static_cast<uint32_t>(dwwritten);
		}
	}

	/*
	*	Returns true if Port is open
	*/
	bool is_open() const noexcept {
		return this->isopen;
	}

	bool rx_available() noexcept {
		
		return WaitCommEvent(this->hcomm, &this->dweventmask, NULL);
	}

	/*
	*	Returns a list of all available ports
	*/
	static std::vector<std::string> get_ports() {

		std::vector<std::string> ports;
		char target[4096];

		for (int i = 0; i < 255; i++) {

			std::string p = "COM" + std::to_string(i);
			if (QueryDosDeviceA(p.c_str(), target, 4096) != 0) {

				ports.push_back(p);
			}
			if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {

				throw stt_serial_exception("Insufficient buffer size (programmer's fault)");
			}
		}

		return ports;
	}

private:

	/*
	*	Add Portname Prefix if needed
	*/
	inline std::string prefix_port(const std::string& port) {

		if (strncmp(port.c_str(), "\\\\.\\", 4) == 0) {

			return port;
		}
		else {
			
			return "\\\\.\\" + port;
		}
	}

	/*
	*	Set Baudrate via CommState
	*/
	void set_baudrate(uint32_t baudrate) {

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

				throw stt_serial_exception("Invalid baudrate");
			}
		}

		DCB dcb = { 0 };

		FillMemory(&dcb, sizeof(dcb), 0);
		if (!GetCommState(this->hcomm, &dcb)) {

			throw stt_serial_exception("Couldn't set baudrate");
		}
		dcb.BaudRate = baud;
		dcb.ByteSize = 8;
		dcb.StopBits = ONESTOPBIT;
		dcb.Parity = NOPARITY;

		if (!SetCommState(this->hcomm, &dcb)) {

			throw stt_serial_exception("Couldn't set baudrate");
		}
	}

	void init_recievemask() {

		this->dweventmask = EV_RXCHAR;

		if (!SetCommMask(this->hcomm, this->dweventmask)) {

			throw stt_serial_exception("Couldn't set comm mask");
		}
	}

	void init_commtimeout() {

		COMMTIMEOUTS timeouts = { 0 };
		timeouts.ReadIntervalTimeout = 1;
		timeouts.ReadTotalTimeoutConstant = 1;
		timeouts.ReadTotalTimeoutMultiplier = 1;
		timeouts.WriteTotalTimeoutConstant = 1;
		timeouts.WriteTotalTimeoutMultiplier = 1;

		if (!SetCommTimeouts(this->hcomm, &timeouts)) {

			throw stt_serial_exception("Couldn't set timeouts");
		}
	}
};