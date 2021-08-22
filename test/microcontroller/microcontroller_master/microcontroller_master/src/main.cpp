#include <iostream>
#include <chrono>
#include "protocol/Package.hpp"
#include "protocol/Serial.hpp"
#include "util/Hardware.hpp"

int main(int argc, char** argv) {

	Protocol::Serial serialPort;                  
	Protocol::Pack32 package;

	try {
		
		auto portNames = Protocol::Serial::GetPortNames();

		if (portNames.size() == 0) {

			std::cerr << "No Ports available!" << std::endl;
			return -1;
		}

		std::cout << "Available Ports ----------" << std::endl;

		for (int i = 0; i < portNames.size(); i++) {

			std::cout << std::to_string(i + 1) << ". " << portNames[i] << std::endl;
		}

		std::cout << std::endl << "Select Port: ";

		std::string port;
		std::cin >> port;

		serialPort.Open(port, 115200);
	}
	catch (const Protocol::SerialException& e) {

		std::cerr << e.what() << std::endl;
		return -1;
	}

	while (true) {

		float pitch = 0;
		float yaw = 0;

		std::cout << "Enter Pitch: ";
		std::cin >> pitch;

		std::cout << "Enter Yaw: ";
		std::cin >> yaw;

		package.Clear();
		package.SetFlag(Protocol::Command::MOVE);
		package.Push<float>(pitch);
		package.Push<float>(yaw);

		try {

			serialPort.Write(reinterpret_cast<uint8_t*>(&package), sizeof(package));
		}
		catch (const Protocol::SerialException& e) {

			std::cerr << e.what() << std::endl;
		}
	}
}