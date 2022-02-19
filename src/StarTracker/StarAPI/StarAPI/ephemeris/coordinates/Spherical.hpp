#ifndef STARAPI_EPHEMERIS_COORDINATES_SPHERICAL_H
#define STARAPI_EPHEMERIS_COORDINATES_SPHERICAL_H

#include <string>

namespace StarTracker::Ephemeris::Coordinates {

    struct Spherical {

        double Radius;
        double RightAscension;
        double Declination;
    };
}

#endif // STARAPI_EPHEMERIS_COORDINATES_SPHERICAL_H