#include "Terrestrial.hpp"

namespace Star::Coordinates {

	Terrestrial::Terrestrial() : Latitude(0), Longitude(0) {

	}

	Terrestrial::Terrestrial(double latitude, double longitude) : Latitude(latitude), Longitude(longitude) {

	}

	std::string Terrestrial::ToString() const {

		return "[latitude = " + std::to_string(Latitude) + " | longitude = " + std::to_string(Longitude) + "]";
	}
}