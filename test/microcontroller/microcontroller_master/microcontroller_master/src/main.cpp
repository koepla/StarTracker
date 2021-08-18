#include <iostream>
#include "protocol/Package.hpp"
#include "protocol/Serial.hpp"
#include "util/Hardware.hpp"

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

std::string autoDetectPort(Protocol::Serial& port);

int main(int argc, char** argv) {

	Protocol::Serial serialPort;
	Protocol::Pack64 package;

	auto portName = autoDetectPort(serialPort);

	try {

		if (!portName.empty()) {

			serialPort.Open(portName, 115200);
		}
		else {

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

std::string autoDetectPort(Protocol::Serial& port) {

	// Get the Hardware ID
	auto hwid = Hardware::GetUID();

	// Create new Package and Push the Hardware ID into it
	auto package = Protocol::Pack64(Protocol::Command::ACK);
	package.PushRange<char>(hwid.c_str(), hwid.size());

	auto portNames = Protocol::Serial::GetPortNames();

	for (auto& p : portNames) {

		try {

			std::cout << "Trying port " << p << "..." << std::endl;

			port.Open(p, 115200);
			
			if (port.IsOpen()) {

				port.Write(reinterpret_cast<uint8_t*>(&package), sizeof(package));
				package.Clear();
				port.Read(reinterpret_cast<uint8_t*>(&package), sizeof(package));

				if (package.GetFlag() == Protocol::Command::ACK) {

					std::cout << "Acknowledge on port " << p << ". Checking Hardware ID..." << std::endl;
					
					if (strcmp(hwid.c_str(), std::string(package.ReadRange<char>(0)).c_str()) == 0) {

						std::cout << "Hardware ID matches!" << std::endl;
					}
					else {

						std::cerr << "Invalid Hardware ID!" << std::endl;

						port.Close();
						continue;
					}

					port.Close();
					return p;
				}
				else {

					std::cerr << "Invalid response on port " << p << "." << std::endl;

					port.Close();
					continue;
				}
			}
			else {

				std::cerr << "Failed to open port " << p << std::endl;
			}
		}
		catch (const Protocol::SerialException& e) {

			std::cerr << "Failed to open port " << p << std::endl;
		}
	}

	return std::string();
}