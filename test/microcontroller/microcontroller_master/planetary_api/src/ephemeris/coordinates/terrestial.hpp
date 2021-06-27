#pragma once

#include <string>

namespace coordinates {

	struct terrestial {

		// latitude positive in north, negative in south
		double latitude;

		// longitude positive in east, negative in west
		double longitude;

		terrestial() {

			this->latitude = 0;
			this->longitude = 0;
		}

		terrestial(double latitude, double longitude) {

			this->latitude = latitude;
			this->longitude = longitude;
		}

		std::string to_string() const {

			return "[latitude = " + std::to_string(latitude) + " | longitude = " + std::to_string(longitude) + "]";
		}
	};
}