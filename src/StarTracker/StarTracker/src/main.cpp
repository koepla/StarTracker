#include <StarAPI/StarAPI.hpp>

#include "core/Core.hpp"
#include "core/serial/Package.hpp"
#include "core/serial/Serial.hpp"

#include "utils/GeoLocation.hpp"
#include "utils/HttpRequest.hpp"
#include "utils/Stopwatch.hpp"

#include <iostream>

int main(int argc, char** argv) {

	try {

		auto&& location = Utils::LocationService::GeoLocation::Get();
		auto&& planets = StarTracker::Ephemeris::CelestialBody::LoadFromFile("assets/CelestialBodies.json");

		for (auto&& planet : planets) {

			auto&& position = planet.GetSphericalPosition(StarTracker::DateTime::Now());
			auto&& observedPosition =
				StarTracker::Ephemeris::Coordinates::Transform::TerrestrialObserverToHorizontal(
					position,
					{ location.Latitude, -location.Longitude },
					StarTracker::DateTime::Now());

			std::cout << observedPosition.ToString() << std::endl;
		}
	}
	catch (const std::exception& e) {

		std::cerr << e.what() << std::endl;
	}
	
	return 0;
}