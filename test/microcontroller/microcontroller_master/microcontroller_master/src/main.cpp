#include <iostream>
#include "protocol/stt_protocol.h"
#include "protocol/stt_serial.h"

int main() {

	stt_serial serial;
	uint8_t rbuff[10];

	try {
		serial.open("COM4", 9600);

		int i = 0;

		while (true) {

			if (serial.rx_available()) {
				
				serial.read(rbuff, 10);

				for (int i = 0; i < 10; i++) {
					std::cout << (int)rbuff[i] << " ";
				}
				std::cout << std::endl;
			}
			else {
				std::cout << "not" << std::endl;
			}
		}

		serial.close();
	}
	catch (const stt_serial_exception& e) {

		std::cerr << e.what() << "Error Code: " << GetLastError() <<  std::endl;
		exit(-1);
	}
	catch (const std::exception& e) {

		std::cerr << e.what() << "Error Code: " << GetLastError() << std::endl;
		exit(-1);
	}
	
	return 0;
}