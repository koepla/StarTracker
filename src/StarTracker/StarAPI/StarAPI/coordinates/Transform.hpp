#ifndef _STARAPI_COORDINATES_H_
#define _STARAPI_COORDINATES_H_

#include "../Math.hpp"
#include "Horizontal.hpp"
#include "Spherical.hpp"
#include "Rectangular.hpp"
#include "Terrestrial.hpp"
#include "../DateTime.hpp"

/*
*   Calculations are based on the pdf doc\references\astronomy_with_personal_computer.pdf
*   For further reference, also see http://www.stjarnhimlen.se/comp/tutorial.html 
*/

namespace Star::Coordinates {

    class Transform {

    public:

        /*
            input:
            coords (by const reference)

            routine:
            transforms spherical coordinates to rectangular ones
        */
        static Rectangular SphericalToRectangular(const Spherical& coords);

        /*
            input:
            coords (by const reference)

            routine:
            transforms rectangular coordinates to spherical ones
        */
        static Spherical RectangularToSpherical(const Rectangular& coords);

        /*
        *   input: 
        *   rectCoords (by reference),
        *   angle      (angle of rotation)
        * 
        *   routine:
        *   rotates the given rectangular coordinates around the y axis [angle] degrees
        *   
        *   note:
        *   could technically use a 4x4 rotation matrix, but due to the fact that we only need to rotate around the y axis,
        *   this 'hard coded' rotation is faster
        */
        static void RotateY(Rectangular& rectCoords, double angle);

        /*
        *   input:
        *   declination (declination in the celestial system)
        *   hourAngle   (hour angle (can be calculated by mean sidereal time and right ascension)
        *   latitude    (latitude of the observer (is needed for the coordinate rotation)
        */
        static Horizontal EquatorialToHorizontal(double declination, double hourAngle, double latitude);

        /*
        *   input:
        *   sphericalCoords (spherical coords of a celestial body, by const reference)
        *   observer        (terrestrial coordinates of an observer, by const reference)
        *   date            (date for the calculation, by const reference)
        * 
        *   routine:
        *   calculates the horizontal coordinates for given spherical coordinates and an observer at a certain date
        */
        static Horizontal TerrestrialObserverToHorizontal(const Spherical& sphericalCoords, const Terrestrial& observer, const DateTime& date);
    };
}

#endif // _STARAPI_COORDINATES_H_