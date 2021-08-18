#include <iostream>
#include "protocol/Package.hpp"
#include "protocol/Serial.hpp"
#include "util/Hardware.hpp"

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