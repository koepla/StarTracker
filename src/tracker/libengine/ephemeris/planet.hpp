#ifndef LIBENGINE_EPHEMERIS_PLANET_H
#define LIBENGINE_EPHEMERIS_PLANET_H

#include "coordinates.hpp"

namespace ephemeris {

    struct Elements {
        f64 SemiMajorAxis;
        f64 Eccentricity;
        f64 Inclination;
        f64 MeanLongitude;
        f64 LonPerihelion;
        f64 LonAscendingNode;
    };

    struct Planet {
        std::string Name;
        Elements Orbit;
        Elements Rate;

        /**
         * @brief Computes the equatorial position of the planet
         * @param date date and time for the computation
         * @return the computed equatorial coordinates
         */
        Equatorial GetEquatorialPosition(const DateTime& date) const noexcept;
    };
}// namespace ephemeris

#endif// LIBENGINE_EPHEMERIS_PLANET_H