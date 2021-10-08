#include <StarAPI/StarAPI.hpp>
#include "core/Core.hpp"
#include "core/serial/Serial.hpp"
#include "core/serial/Package.hpp"
#include "utils/HttpRequest.hpp"
#include "utils/GeoLocation.hpp"
#include "utils/Stopwatch.hpp"

int main(int argc, char** argv) {

	try {

		auto elements = Star::Ephemeris::CelestialBody::LoadFromFile("assets/KeplarianElements.json");

		for (auto& e : elements) {

			std::cout << e.GetName();
		}
	}
	catch (const std::exception& e) {

		std::cerr << e.what() << std::endl;
	}

	return 0;

	

	Serial::SerialPort serialPort;
	Utils::LocationService::Location location;
	Star::Coordinates::Observer observer;

	try {

		location = Utils::LocationService::GeoLocation::Get();

		std::cout << "Is this your location (" << location.City << ", " << location.RegionName << ", " << location.Country;
		std::cout << ", Lat: " << location.Latitude << ", Lon: " << location.Longitude << ") [y,n]? ";

		char input;
		std::cin >> input;

		if (input == 'y') {

			observer = { location.Latitude, -location.Longitude };
		}
		else if (input == 'n') {

			std::cout << "Please enter your location <latitude> <longitude>: ";
			double latitude, longitude;
			std::cin >> latitude >> longitude;

			observer = { latitude, longitude };
		}
	}
	catch (const Utils::LocationService::GeoLocationException&) {
	
		std::cout << "Please enter your location <latitude> <longitude>: ";
		double latitude, longitude;
		std::cin >> latitude >> longitude;
		observer = { latitude, longitude };
	}

	try {

		auto portNames = Serial::SerialPort::GetPortNames();

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
	catch (const Serial::SerialException& e) {

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

		auto celestialBodySpherical = Star::Coordinates::Spherical(
			Star::Math::HmsToDegrees(hour, minute, second), 
			Star::Math::DaaToDegrees(degree, arcmin, arcsec));

		auto observedCelestialBody = Star::Coordinates::Transform::TerrestrialObserverToHorizontal(celestialBodySpherical, observer, Star::Date::Now());

		std::cout << "Sending computed data " << observedCelestialBody.ToString() << " to micro controller..." << std::endl;

		try {

			Serial::Pack32 package = Serial::Pack32(Serial::Command::MOVE);
			package.Push<float>(static_cast<float>(observedCelestialBody.Altitude));
			package.Push<float>(static_cast<float>(observedCelestialBody.Azimuth));
			STR_ASSERT(serialPort.Write(reinterpret_cast<uint8_t*>(&package), sizeof(package)) == sizeof(package), "Couldn't send the entire package");
		}
		catch (const Serial::SerialException& e) {

			std::cerr << "Can't send data: " << e.what() << std::endl;
		}
	}

	return 0;
}