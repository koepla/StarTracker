#ifndef _ASTRO_COORDINATES_H_
#define _ASTRO_COORDINATES_H_

#include "../Math.hpp"
#include "Horizontal.hpp"
#include "Spherical.hpp"
#include "Rectangular.hpp"
#include "Terrestial.hpp"
#include "../Date.hpp"

/*
*   Calculations are based on the pdf doc\references\astronomy_with_personal_computer.pdf
*   For further reference, also see http://www.stjarnhimlen.se/comp/tutorial.html 
*/

namespace Astro::Coordinates {

    class Transform {

    public:

        /*
            input:
            &rect_cord  (rectangular coordinates (reference)),
            right_asc   (Right Ascension)
            decl        (Declination)
            r           (Distance, set to 1 by default)

            routine:
            transforms spherical coordinates to rectangular ones
        */
        static Rectangular SphericalToRectangular(const Spherical& coords) {

            Rectangular rectCoords;

            rectCoords.x = coords.radius * Math::Cosine(coords.rightAscension) * Math::Cosine(coords.declination);
            rectCoords.y = coords.radius * Math::Sine(coords.rightAscension) * Math::Cosine(coords.declination);
            rectCoords.z = coords.radius * Math::Sine(coords.declination);

            return rectCoords;
        }

        /*
            input:
            &spherical_coord (spherical coordinates (reference)),
            x,y,z horizontal coordinates

            routine:
            transforms rectangular coordinates to spherical ones
        */
        static Spherical RectangularToSpherical(const Rectangular& coords) {

            Spherical sphericalCoords;

            double x = coords.x;
            double y = coords.y;
            double z = coords.z;

            //celestial north and south pole
            if (x == 0 && y == 0) {

                sphericalCoords.rightAscension = 0;
            }

            sphericalCoords.radius = sqrt(x * x + y * y + z * z);
            sphericalCoords.rightAscension = Math::ArcTangent2(y, x);
            sphericalCoords.declination = Math::ArcTangent2(z , sqrt(x*x + y*y));

            return sphericalCoords;
        }

        /*
        *   input: 
        *   &rect_coord (rectangular coordinates (reference)),
        *   angle       (angle of rotation)
        * 
        *   routine:
        *   rotates the given rectangular coordinates around the y axis [angle] degrees
        */
        static void RotateY(Rectangular& rectCoords, double angle) {

            double xn = rectCoords.x * Math::Sine(angle) - rectCoords.z * Math::Cosine(angle);
            double yn = rectCoords.y;
            double zn = rectCoords.x * Math::Cosine(angle) + rectCoords.z * Math::Sine(angle);

            rectCoords.x = xn;
            rectCoords.y = yn;
            rectCoords.z = zn;
        }

        /*
        *   input:
        *   declination     (declination in the celestial system)
        *   hour_angle      (hour angle (can be calculated by mean sidereal time and right ascension)
        *   latitude        (latitude of the observer (is needed for the coordinate rotation)
        */
        static Horizontal EquatorialToHorizontal(double declination, double hourAngle, double latitude) {

            Horizontal horizontalCoords;

            Rectangular re = SphericalToRectangular(Spherical(hourAngle, declination));

            RotateY(re, latitude);

            // add 180 to get the angle from north to east to south and so on
            horizontalCoords.azimuth = Math::ArcTangent2(re.y, re.x) + 180;
            horizontalCoords.altitude = Math::ArcSine(re.z);

            return horizontalCoords;
        }

        /*
        *   input:
        *   &coords     (spherical coords of an in object in the sky)
        *   &observer   (terrestial coordinates of an observer)
        *   &d          (date for the calculation)
        * 
        *   routine:
        *   calculates the horizontal coordinates for given spherical coordinates and an observer at a certain date
        */
        static Horizontal TerrestialObserverToHorizontal(const Spherical& sphericalCoords, const Terrestial& observer, const Date& d) {
    
            double hourAngle = Date::Gmst(d) + observer.longitude - sphericalCoords.rightAscension;
            return EquatorialToHorizontal(sphericalCoords.declination, hourAngle, observer.latitude);
        }
    };
}

#endif // _ASTRO_COORDINATES_H_