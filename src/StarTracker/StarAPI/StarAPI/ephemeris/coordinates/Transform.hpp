#ifndef STARAPI_EPHEMERIS_COORDINATES_TRANSFORM_H
#define STARAPI_EPHEMERIS_COORDINATES_TRANSFORM_H

#include "../../Math.hpp"
#include "Horizontal.hpp"
#include "Spherical.hpp"
#include "Rectangular.hpp"
#include "Terrestrial.hpp"
#include "../../DateTime.hpp"

/*
*   Calculations are based on the pdf doc\references\astronomy_with_personal_computer.pdf
*   For further reference, also see http://www.stjarnhimlen.se/comp/tutorial.html 
*/

namespace StarTracker::Ephemeris::Coordinates {

    class Transform {

    public:

        /**
        * Transform spherical coordinates to rectangular ones
        *
        * @param coords the spherical coordinates as const reference
        *
        * @return the transformed rectangular coordinates
        *
        */
        [[nodiscard]] static Rectangular SphericalToRectangular(const Spherical& coords) noexcept;

        /**
        * Transform rectangular coordinates to spherical ones
        *
        * @param coords the rectangular coordinates as const reference
        *
        * @return the transformed spherical coordinates
        *
        */
        [[nodiscard]] static Spherical RectangularToSpherical(const Rectangular& coords) noexcept;

        /**
        * Rotates rectangular coordinates around the Y-axis by `angle`
        *
        * @param rectCoords the rectangular coordinates as const reference
        * 
        * @param angle the to be rotated angle
        *
        * @return the rotated rectangular coordinates
        *
        */
        [[nodiscard]] static Rectangular RotateRectangular(const Rectangular& rectCoords, double angle) noexcept;

        /**
        * Transforms Equatorial coordinates to Horizontal ones
        *
        * @param declination declination in degrees
        *
        * @param hourAngle hourAngle (by GMST) in degrees
        * 
        * @param latitude latitude in degrees
        *
        * @return the transformed horizontal coordinates
        *
        */
        [[nodiscard]] static Horizontal EquatorialToHorizontal(double declination, double hourAngle, double latitude) noexcept;

        /**
        * Computes the Horizontal position of an object with spherical coordinates
        *
        * @param sphericalCoords the spherical coordinates of the object
        *
        * @param observer the terrestrial coordinates of the observer
        *
        * @param date the date and time for the computation
        *
        * @return the computed horizontal coordinates
        *
        */
        [[nodiscard]] static Horizontal TerrestrialObserverToHorizontal(const Spherical& sphericalCoords, const Terrestrial& observer, const DateTime& date) noexcept;
    };
}

#endif // STARAPI_EPHEMERIS_COORDINATES_TRANSFORM_H