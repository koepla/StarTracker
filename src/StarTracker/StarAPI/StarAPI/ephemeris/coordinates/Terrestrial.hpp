#ifndef STARAPI_EPHEMERIS_COORDINATES_TERRESTRIAL_H
#define STARAPI_EPHEMERIS_COORDINATES_TERRESTRIAL_H

#include <string>

namespace StarTracker::Ephemeris::Coordinates {

	struct Terrestrial {

		// latitude positive in north, negative in south
		double Latitude;

		// longitude positive in east, negative in west
		double Longitude;
	};

	using Observer = Terrestrial;
}

#endif // STARAPI_EPHEMERIS_COORDINATES_TERRESTRIAL_H