#include <StarAPI/StarAPI.hpp>

#include "core/Core.hpp"
#include "core/Assert.hpp"
#include "utils/serial/Package.hpp"
#include "utils/serial/Serial.hpp"

#include "utils/GeoLocation.hpp"
#include "utils/HttpRequest.hpp"
#include "utils/Stopwatch.hpp"

#include <iostream>
#include <filesystem>

bool sendToTracker(StarTracker::Utils::Serial::SerialPort& port, StarTracker::Utils::Serial::Pack32& package, const StarTracker::Ephemeris::Coordinates::Horizontal& position) {

	std::cout << "\tTracking Object " << position.ToString() << std::endl << std::endl;

	package.Clear();
	package.SetFlag(StarTracker::Utils::Serial::Command::MOVE);
	package.Push<float>(static_cast<float>(position.Altitude));
	package.Push<float>(static_cast<float>(position.Azimuth));

	return (port.Write(reinterpret_cast<uint8_t*>(&package), sizeof(package)) == sizeof(package));
}

int main(int argc, const char** argv) {

	/* Serial Debug - ECHO Test */
	try {

		StarTracker::Utils::Serial::SerialPort serialPort{};
		StarTracker::Utils::Serial::Pack32 txPackage{ StarTracker::Utils::Serial::Command::ACK };
		StarTracker::Utils::Serial::Pack32 rxPackage{ StarTracker::Utils::Serial::Command::NONE };

		serialPort.Open("COM3", 115200);

		while (serialPort.IsOpen()) {

			rxPackage.Clear();

			std::fprintf(stdout, "SENDING - %s\n", StarTracker::DateTime::Now().ToString().c_str());

			ASSERT(serialPort.Write(reinterpret_cast<uint8_t*>(&txPackage), sizeof txPackage) == sizeof txPackage);

			while (serialPort.Available() != sizeof rxPackage) {

				std::fprintf(stdout, "WAITING - %s\n", StarTracker::DateTime::Now().ToString().c_str());
				Sleep(50);
			}

			ASSERT(serialPort.Read(reinterpret_cast<uint8_t*>(&rxPackage), sizeof rxPackage) == sizeof rxPackage, false);

			if (rxPackage.GetFlag() == StarTracker::Utils::Serial::Command::ACK) {

				std::fprintf(stdout, "ECHO - %s\n", StarTracker::DateTime::Now().ToString().c_str());
			}
			else {

				std::fprintf(stderr, "FAILURE - %s\n", StarTracker::DateTime::Now().ToString().c_str());
				__debugbreak();
			}

			Sleep(500);
		}
	}
	catch (const std::exception& e) {

		std::fprintf(stderr, "Fatal Exception: %s\n", e.what());
	}

	exit(0);

	try {	

		std::cout << "------ StarTracker Commandline Interface ------" << std::endl << std::endl;

		StarTracker::Utils::Serial::SerialPort serialPort{};
		StarTracker::Utils::Serial::Pack32 package{};

		std::cout << "#Port Selection " << std::endl;

		auto portNames = serialPort.GetPortNames();

		if (portNames.size() == 0) {

			std::cerr << "\tNo Serial Ports available" << std::endl;
			return -1;
		}

		for (auto&& port : portNames) {

			std::cout << "\t" << port << std::endl;
		}

		std::string selectedPort;
		std::cout << "\tSelect a port: ";
		std::cin >> selectedPort;

		serialPort.Open(selectedPort, 115200);

		std::cout << std::endl << "#Tracker Adjustment " << std::endl;

		char dec;
		do {

			float pitch, yaw;
			std::cout << "\tAdjust tracker origin <pitch> <yaw>: ";
			std::cin >> pitch >> yaw;

			package.Clear();
			package.SetFlag(StarTracker::Utils::Serial::Command::MOVE);
			package.Push(static_cast<float>(pitch));
			package.Push(static_cast<float>(yaw));
			ASSERT(serialPort.Write(reinterpret_cast<uint8_t*>(&package), sizeof(package)) == sizeof package);

			package.Clear();
			package.SetFlag(StarTracker::Utils::Serial::Command::CONF);
			package.Push(0.0f);
			package.Push(0.0f);
			ASSERT(serialPort.Write(reinterpret_cast<uint8_t*>(&package), sizeof(package)) == sizeof package);

			std::cout << "\tAdjust further? [y|n] ";
			std::cin >> dec;
		} 
		while (dec == 'y');

		std::cout << std::endl << "#Celestial Body Computation" << std::endl;

		StarTracker::Ephemeris::Coordinates::Observer observer{};
		std::cout << "\tEnter Location <latitude> <longitude>: ";
		std::cin >> observer.Latitude >> observer.Longitude;

		while (!GetAsyncKeyState(VK_ESCAPE) && serialPort.IsOpen()) {

			auto celestialBodies = StarTracker::Ephemeris::CelestialBody::LoadFromFile("assets/CelestialBodies.json");
	
			for (auto&& body : celestialBodies) {

				std::cout << "\t" << body->GetName() << " " << body->GetSphericalPosition(StarTracker::DateTime::Now()).ToString() << std::endl;
			}

			std::cout << "\tManual [no precomputed data]" << std::endl;

			std::string bodyName;
			std::cout << "\tSelect a body: ";
			std::getline(std::cin, bodyName);

			if (bodyName._Equal("Manual")) {

				double raHour, raMin, raSec;
				std::cout << "\t\tEnter Right Ascension <hour> <minute> <second>: ";
				std::cin >> raHour >> raMin >> raSec;

				double declDeg, declArcmin, declArcsec;
				std::cout << "\t\tEnter Declination <degree> <arcmin> <arcsec>: ";
				std::cin >> declDeg >> declArcmin >> declArcsec;

				StarTracker::Ephemeris::Coordinates::Spherical manualObject{};
				manualObject.RightAscension = StarTracker::Math::HmsToDegrees(raHour, raMin, raSec);
				manualObject.Declination = StarTracker::Math::DaaToDegrees(declDeg, declArcmin, declArcsec);
				manualObject.Radius = 1.0;

				auto observedPosition = StarTracker::Ephemeris::Coordinates::Transform::TerrestrialObserverToHorizontal(
					manualObject,
					{ observer.Latitude, observer.Longitude },
					StarTracker::DateTime::Now()
				);

				std::cout << std::endl << "\tComputed position: " << manualObject.ToString() << " " << observedPosition.ToString() << std::endl;

				sendToTracker(serialPort, package, observedPosition);
			}
			else {

				for (auto&& body : celestialBodies) {

					if (body->GetName()._Equal(bodyName)) {

						auto sphericalPosition = body->GetSphericalPosition(StarTracker::DateTime::Now());
						auto observedPosition = StarTracker::Ephemeris::Coordinates::Transform::TerrestrialObserverToHorizontal(
							sphericalPosition,
							{ observer.Latitude, observer.Longitude },
							StarTracker::DateTime::Now()
						);

						std::cout << std::endl << "\tComputed position: " << sphericalPosition.ToString() << " " << observedPosition.ToString() << std::endl;
						
						sendToTracker(serialPort, package, observedPosition);
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