#ifndef _ASTRO_HORIZONTAL_H_
#define _ASTRO_HORIZONTAL_H_

#include <string>

namespace Astro::Coordinates {

	struct Horizontal {

		double azimuth;
		double altitude;

		Horizontal() {

			this->azimuth = 0;
			this->altitude = 0;
		}

		Horizontal(double azimuth, double altitude) {

			this->azimuth = azimuth;
			this->altitude = altitude;
		}

		std::string ToString() const {

			return "[azimuth = " + std::to_string(this->azimuth) + " | altitude = " + std::to_string(this->altitude) + "]";
		}
	};
}

#endif // _ASTRO_HORIZONTAL_H_