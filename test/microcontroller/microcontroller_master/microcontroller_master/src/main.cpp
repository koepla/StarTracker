#include <iostream>
#include "protocol/stt_protocol.h"
#include "protocol/stt_serial.h"

/*
*	TODO: Figure out a solution for the following problem
*	
*	The Arduino only has a Serial Buffer of 64 bytes, thus the maximum byte
*	buffer size of our protocol would be 52 or 56, because we have to acount for the
*	8 byte header. Now we have to decide, whether the byte buffer size should be 
*	constant (52 / 56), or still modifyable, because packages like stop and origin 
*	have zero payload other than the header. We also have to acount for the handling 
*	on the microcontroller side, because if we have data larger than 52 / 56 bytes, 
*	we have to send it in a series of packages, for which we would have to
*	introduce a new flag, which would be dependent of the previous flag. One might argue that
*	this new flag could only occur when sending data for the MOVE flag, which is true, but
*	you never know. We also have to implement functions on the Arduino side for dealing with 
*	incoming data. We could use a global data buffer, in which all of the payload is loaded.
*	Such a design would not be good, because in theory, when the move flag is set, the arduino
*	should start turning the stepper motors.
*/

int main() {

	stt_serial serial;
	stt_protocol<32> prot = stt_protocol<32>(stt_flag::MOVE);
	
	
	for (int i = 0; i < 8; i++) {

		prot.push<float>(i + 3.14);
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