#ifndef _ASTRO_TERRESTIAL_H_
#define _ASTRO_TERRESTIAL_H_

#include <string>

namespace Astro::Coordinates {

	struct Terrestial {

		// latitude positive in north, negative in south
		double latitude;

		// longitude positive in east, negative in west
		double longitude;

		Terrestial() {

			this->latitude = 0;
			this->longitude = 0;
		}

		Terrestial(double latitude, double longitude) {

			this->latitude = latitude;
			this->longitude = longitude;
		}

		std::string ToString() const {

			return "[latitude = " + std::to_string(latitude) + " | longitude = " + std::to_string(longitude) + "]";
		}
	};
}

#endif // _ASTRO_TERRESTIAL_H_