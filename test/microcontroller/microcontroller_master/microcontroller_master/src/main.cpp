#include <iostream>
#include "protocol/stt_protocol.h"
#include "protocol/stt_serial.h"

int main() {

	stt_serial serial;

	try {
		serial.open("COM4");
		serial.close();
	}
	catch (const stt_serial_exception& e) {
		std::cerr << e.what() << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	
	return 0;
}