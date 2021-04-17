#include <iostream>
#include "protocol/stt_protocol.h"
#include "protocol/stt_serial.h"

int main() {

	stt_serial serial;
	uint8_t buff[8];

	try {

		serial.open("COM4", 256000);
		
		while (serial.is_open()) {

			serial.write(new uint8_t[8]{ 0, 1, 2, 3, 4, 5, 6, 7 }, 8);
			serial.read(buff, 8);

			for (int i = 0; i < 8; i++) {
				std::cout << (int)buff[i] << " ";
			}

			std::cout << std::endl;
		}

		serial.close();
	}
	catch (const stt_serial_exception& e) {
		
		std::cerr << e.what() << std::endl;
	}

	return 0;
}