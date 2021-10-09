#ifndef STARAPI_EPHEMERIS_COORDINATES_SPHERICAL_H
#define STARAPI_EPHEMERIS_COORDINATES_SPHERICAL_H

#include <string>

namespace StarTracker::Ephemeris::Coordinates {

    struct Spherical {

        double Radius;
        double RightAscension;
        double Declination;

        Spherical();
        Spherical(double rightAscension, double declination, double radius = 1);

        std::string ToString() const;
    };
}

#endif // STARAPI_EPHEMERIS_COORDINATES_SPHERICAL_H