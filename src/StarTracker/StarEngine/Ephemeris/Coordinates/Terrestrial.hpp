#ifndef STARENGINE_EPHEMERIS_COORDINATES_TERRESTRIAL_H
#define STARENGINE_EPHEMERIS_COORDINATES_TERRESTRIAL_H

namespace StarTracker::Ephemeris::Coordinates {

	struct Terrestrial {

		// latitude positive in north, negative in south
		double Latitude;

		// longitude positive in east, negative in west
		double Longitude;
	};

	using Observer = Terrestrial;
}

#endif // STARENGINE_EPHEMERIS_COORDINATES_TERRESTRIAL_H