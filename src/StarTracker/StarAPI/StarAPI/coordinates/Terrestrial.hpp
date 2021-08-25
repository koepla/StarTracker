#ifndef _STARAPI_TERRESTIAL_H_
#define _STARAPI_TERRESTIAL_H_

#include <string>

namespace Star::Coordinates {

	struct Terrestrial {

		// latitude positive in north, negative in south
		double Latitude;

		// longitude positive in east, negative in west
		double Longitude;

		Terrestrial();
		Terrestrial(double latitude, double longitude);

		std::string ToString() const;
	};

	typedef Terrestrial Observer;
}

#endif // _STARAPI_TERRESTIAL_H_