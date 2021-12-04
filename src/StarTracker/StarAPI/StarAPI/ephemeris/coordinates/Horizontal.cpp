#include "Horizontal.hpp"

namespace StarTracker::Ephemeris::Coordinates {

	Horizontal::Horizontal() : Azimuth{ 0 }, Altitude{ 0 } {

	}

	Horizontal::Horizontal(double azimuth, double altitude) : Azimuth{ azimuth }, Altitude{ altitude } {

	}

	std::string Horizontal::ToString() const noexcept {

		return "[azimuth = " + std::to_string(Azimuth) + " | altitude = " + std::to_string(Altitude) + "]";
	}
}