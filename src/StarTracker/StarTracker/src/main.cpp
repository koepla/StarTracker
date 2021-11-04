#include <StarAPI/StarAPI.hpp>

#include "core/Core.hpp"
#include "core/serial/Package.hpp"
#include "core/serial/Serial.hpp"

#include "utils/GeoLocation.hpp"
#include "utils/HttpRequest.hpp"
#include "utils/Stopwatch.hpp"

#include <iostream>

int main(int argc, const char** argv) {

	try {

		StarTracker::Serial::SerialPort serialPort{};
		StarTracker::Serial::Pack32 package{};

		auto portNames = serialPort.GetPortNames();

		if (portNames.size() == 0) {

			std::cerr << "No Serial Ports available" << std::endl;
			return -1;
		}

		for (auto&& port : portNames) {

			std::cout << port << std::endl;
		}

		std::string selectedPort;
		std::cout << "Select a port: ";
		std::cin >> selectedPort;

		serialPort.Open(selectedPort, 115200);

		StarTracker::Ephemeris::Coordinates::Observer observer{};
		std::cout << "Enter Location <latitude> <longitude>: ";
		std::cin >> observer.Latitude >> observer.Longitude;

		while (!GetAsyncKeyState(VK_ESCAPE) && serialPort.IsOpen()) {

			auto celestialBodies = StarTracker::Ephemeris::CelestialBody::LoadFromFile("assets/CelestialBodies.json");
	
			for (auto&& body : celestialBodies) {

				std::cout << body.GetName() << std::endl;
			}

			std::string bodyName;
			std::cout << "Select a body: ";
			std::cin >> bodyName;

			for (auto&& body : celestialBodies) {

				if (body.GetName()._Equal(bodyName)) {

					package.Clear();
					package.SetFlag(StarTracker::Serial::Command::MOVE);
					
					auto position = StarTracker::Ephemeris::Coordinates::Transform::TerrestrialObserverToHorizontal(
						body.GetSphericalPosition(StarTracker::DateTime::Now()),
						{ observer.Latitude, -observer.Longitude },
						StarTracker::DateTime::Now()
					);

					std::cout << "Computed position: " << position.ToString() << std::endl;

					package.Push<float>(static_cast<float>(position.Altitude));
					package.Push<float>(static_cast<float>(position.Azimuth));

					if (serialPort.Write(reinterpret_cast<uint8_t*>(&package), sizeof(package)) == sizeof(package)) {

						std::cout << "Sent position to Tracker!" << std::endl;
					}
					else {

						std::cerr << "Couldn't send position to Tracker!" << std::endl;
					}
				}
			}
		}

		std::cout << "Shutting down..." << std::endl;

		serialPort.Close();
	}
	catch (const std::exception& e) {

		std::cerr << e.what() << std::endl;
	}
	
	return 0;
}
