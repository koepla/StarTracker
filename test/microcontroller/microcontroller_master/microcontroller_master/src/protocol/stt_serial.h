#pragma once
#include <windows.h>
#include <iostream>

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
	bool isopen;

public:

	stt_serial() {
		this->hcomm = nullptr;
		this->isopen = false;
	}

	/*
	*	Opens the specified port
	*	Arguments are portname and baudrate
	*/
	void open(const std::string& port, uint32_t baudrate) noexcept(false) {

		std::string prefixed = this->prefix_port(port);
		this->hcomm = CreateFileA(prefixed.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);

		if (this->hcomm == INVALID_HANDLE_VALUE) {

			this->isopen = false;
			throw stt_serial_exception("Couldn't open Serial Port " + prefixed);
		}
		else {
			this->isopen = true;
		}

		this->set_baudrate(baudrate);
	}

	/*
	*	Closes the currently open Port
	*/
	void close() noexcept {

		CloseHandle(this->hcomm);
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

		DCB dcb = { 0 };

		FillMemory(&dcb, sizeof(dcb), 0);
		if (!GetCommState(this->hcomm, &dcb)) {

			throw stt_serial_exception("Couldn't set baudrate");
		}
		dcb.BaudRate = baudrate;

		if (!SetCommState(this->hcomm, &dcb)) {

			throw stt_serial_exception("Couldn't set baudrate");
		}
	}
};