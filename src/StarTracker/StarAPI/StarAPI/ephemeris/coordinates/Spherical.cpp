#include "Spherical.hpp"

namespace StarTracker::Ephemeris::Coordinates {

	Spherical::Spherical() : RightAscension{ 0 }, Declination{ 0 }, Radius{ 1 } {

	}

	Spherical::Spherical(double rightAscension, double declination, double radius) : RightAscension{ rightAscension }, Declination{ declination }, Radius{ radius } {

	}

	std::string Spherical::ToString() const noexcept {

		return "[right_ascension = " + std::to_string(RightAscension) + " | declination = " + std::to_string(Declination) + " | r = " + std::to_string(Radius) + "]";
	}
}