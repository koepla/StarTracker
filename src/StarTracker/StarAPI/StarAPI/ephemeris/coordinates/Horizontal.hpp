#ifndef STARAPI_EPHEMERIS_COORDINATES_HORIZONTAL_H
#define STARAPI_EPHEMERIS_COORDINATES_HORIZONTAL_H

#include <string>

namespace StarTracker::Ephemeris::Coordinates {

	struct Horizontal {

		double Azimuth;
		double Altitude;

		Horizontal();
		Horizontal(double azimuth, double altitude);

		/**
		* Formats member variables to a std::string
		*
		* @return the member variables `Azimuth` and `Altitude`
		*     formatted as std::string
		*
		*/
		[[nodiscard]] std::string ToString() const noexcept;
	};
}

#endif // STARAPI_EPHEMERIS_COORDINATES_HORIZONTAL_H