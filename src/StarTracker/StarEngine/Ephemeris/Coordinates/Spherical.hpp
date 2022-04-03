#ifndef STARENGINE_EPHEMERIS_COORDINATES_SPHERICAL_H
#define STARENGINE_EPHEMERIS_COORDINATES_SPHERICAL_H

#include <string>

namespace StarTracker::Ephemeris::Coordinates {

	struct Spherical {

		double Radius;
		double RightAscension;
		double Declination;
	};
}

#endif // STARENGINE_EPHEMERIS_COORDINATES_SPHERICAL_H