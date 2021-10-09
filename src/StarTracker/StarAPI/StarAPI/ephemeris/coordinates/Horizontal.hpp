#ifndef STARAPI_EPHEMERIS_COORDINATES_HORIZONTAL_H
#define STARAPI_EPHEMERIS_COORDINATES_HORIZONTAL_H

#include <string>

namespace StarTracker::Ephemeris::Coordinates {

	struct Horizontal {

		double Azimuth;
		double Altitude;

		Horizontal();
		Horizontal(double azimuth, double altitude);

		std::string ToString() const;
	};
}

#endif // STARAPI_EPHEMERIS_COORDINATES_HORIZONTAL_H