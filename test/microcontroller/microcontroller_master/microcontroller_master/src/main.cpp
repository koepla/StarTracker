#include <iostream>
#include <conio.h>
#include "protocol/stt_package.hpp"
#include "protocol/stt_serial.hpp"
#include "app.hpp"

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
*	introduce a new flag, which would be dependent on the previous flag. One might argue that
*	this new flag could only occur when sending data for the MOVE flag, which is true, but
*	you never know. We also have to implement functions on the Arduino side for dealing with 
*	incoming data. We could use a global data buffer, in which all of the payload is loaded.
*	Such a design would not be good, because in theory, when the move flag is set, the arduino
*	should start turning the stepper motors.
*/

/*
*	Inheriting class of app
*/
class serial_app : public app {

private:
	protocol::stt_serial serial;
	protocol::stt_package<32> pack;
	uint8_t* buff;

public:

	serial_app() {
		this->buff = nullptr;
	}

	virtual void init() override {

		try {
			serial.open("COM5", 115200);
		}
		catch (const protocol::stt_serial_exception& e) {

			std::cerr << e.what() << std::endl;
			running = false;
		}

		this->buff = reinterpret_cast<uint8_t*>(&pack);
	}

	virtual void update() override {

		float angle;

		std::cout << "Enter angle: ";
		std::cin >> angle;

		pack.clear();
		pack.push<float>(angle);

		try {
			serial.write(buff, 40);
			std::cout << "sent 40 bytes of data" << std::endl;
		}
		catch (const protocol::stt_serial_exception& e) {

			std::cerr << e.what() << std::endl;
		}
	}

	virtual void onevent(const events::event& e) override {


	}
};

extern app* create() {
	return new serial_app();
}