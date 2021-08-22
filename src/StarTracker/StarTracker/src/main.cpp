#include <StarAPI/StarAPI.hpp>
#include "core/serial/Serial.hpp"
#include "core/serial/Package.hpp"

int main(int argc, char** argv) {

	Protocol::Serial serialPort;

	auto observer = Star::Coordinates::Terrestial(48.30694, -14.28583);

	try {

		auto portNames = Protocol::Serial::GetPortNames();

		if (portNames.size() == 0) {

			std::cerr << "No ports available" << std::endl;
			return -1;
		}

		std::cout << "Available ports: " << std::endl;

		for (int i = 0; i < portNames.size(); i++) {

			std::cout << std::to_string(i + 1) << ". " << portNames[i] << std::endl;
		}

		std::cout << "Enter Port: ";

		std::string portChoice;
		std::cin >> portChoice;

		serialPort.Open(portChoice, 115200);
	}
	catch (const Protocol::SerialException& e) {

		std::cerr << e.what() << std::endl;
		return -1;
	}

	while (true) {

		double hour, minute, second;
		std::cout << "Enter Right ascension (Format: <hour> <minute> <second>): ";
		std::cin >> hour >> minute >> second;

		double degree, arcmin, arcsec;
		std::cout << "Enter Declination (Format: <degrees> <arcminutes> <arcseconds>): ";
		std::cin >> degree >> arcmin >> arcsec;

		auto mars = Star::Coordinates::Spherical(
			Star::Math::HourToDegrees(hour, minute, second), 
			Star::Math::RealDegrees(degree, arcmin, arcsec));

		auto marsPos = Star::Coordinates::Transform::TerrestialObserverToHorizontal(mars, observer, Star::Date::Now());

		std::cout << "Sending computed data " << marsPos.ToString() << " to microcontroller..." << std::endl;

		try {

			Protocol::Pack32 package = Protocol::Pack32(Protocol::Command::MOVE);
			package.Push<float>(static_cast<float>(marsPos.altitude));
			package.Push<float>(static_cast<float>(marsPos.azimuth));

			serialPort.Write(reinterpret_cast<uint8_t*>(&package), sizeof(package));
		}
		catch (const Protocol::SerialException& e) {

			std::cerr << "Can't send data: " << e.what() << std::endl;
		}
	}

	return 0;
}