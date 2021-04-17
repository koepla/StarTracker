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

public:

	stt_serial() {
		this->hcomm = nullptr;
	}

	void open(const std::string& port) noexcept(false) {

		std::string prefixed = this->prefix_port(port);
		this->hcomm = CreateFileA(prefixed.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);

		if (this->hcomm == INVALID_HANDLE_VALUE) {
			throw stt_serial_exception("Couldn't open Serial Port " + prefixed);
		}
	}

	void close() noexcept {

		CloseHandle(this->hcomm);
	}

private:

	inline std::string prefix_port(const std::string& port) {

		if (strncmp(port.c_str(), "\\\\.\\", 4) == 0) {
			return port;
		}
		else {
			
			return "\\\\.\\" + port;
		}
	}
};