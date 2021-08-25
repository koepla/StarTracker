#ifndef _STARAPI_HORIZONTAL_H_
#define _STARAPI_HORIZONTAL_H_

#include <string>

namespace Star::Coordinates {

	struct Horizontal {

		double Azimuth;
		double Altitude;

		Horizontal();
		Horizontal(double azimuth, double altitude);

		std::string ToString() const;
	};
}

#endif // _STARAPI_HORIZONTAL_H_