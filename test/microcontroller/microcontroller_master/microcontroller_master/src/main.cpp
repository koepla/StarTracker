#include <iostream>
#include "protocol/stt_protocol.h"
#include "protocol/stt_serial.h"

int main() {

	stt_serial serial;
	stt_protocol<32> prot = stt_protocol<32>(stt_flag::MOVE);
	
	for (int i = 0; i < 8; i++) {

		prot.push<float>(i + 3.14, 1);
	}

	uint8_t* buff = reinterpret_cast<uint8_t*>(&prot);

	try {

		serial.open("COM4", 256000);
		
		while (serial.is_open()) {

			if (GetAsyncKeyState(VK_INSERT) & 1) {

				serial.write(buff, 40);
			}
		}

		serial.close();
	}
	catch (const stt_serial_exception& e) {
		
		std::cerr << e.what() << std::endl;
	}
	
	

	return 0;
}