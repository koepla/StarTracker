#pragma once

#include "../math.hpp"
#include "horizontal.hpp"
#include "spherical.hpp"
#include "rectangular.hpp"
#include "terrestial.hpp"
#include "../date.hpp"

namespace coordinates {

    class transform {

    public:

        /*
            input:
            &rect_cord  (rectangular coordinates (reference)),
            right_asc   (Right Ascension)
            decl        (Declination)
            r           (Distance, set to 1 by default)
        */
        static rectangular spherical2rect(const spherical& coords) {

            rectangular rect_coord;

            rect_coord.x = coords.r * cos_deg(coords.right_asc) * cos_deg(coords.decl);
            rect_coord.y = coords.r * sin_deg(coords.right_asc) * cos_deg(coords.decl);
            rect_coord.z = coords.r * sin_deg(coords.decl);

            return rect_coord;
        }

        /*
            input:
            &spherical_coord (spherical coordinates (reference)),
            x,y,z horizontal coordinates
        */
        static spherical rect2spherical(const rectangular& coords) {

            spherical spherical_coord;

            double x = coords.x;
            double y = coords.y;
            double z = coords.z;

            //celestial north and south pole
            if (x == 0 && y == 0) {

                spherical_coord.right_asc = 0;
            }

            spherical_coord.r = sqrt(x * x + y * y + z * z);
            spherical_coord.right_asc = atan2_deg(y, x);
            spherical_coord.decl = atan2_deg(z, sqrt(x * x + y * y));

            return spherical_coord;
        }

        static void rotate_rect(rectangular& rect_coord, double latitude) {

            double xn = rect_coord.x * sin_deg(latitude) - rect_coord.z * cos_deg(latitude);
            double yn = rect_coord.y;
            double zn = rect_coord.x * cos_deg(latitude) + rect_coord.z * sin_deg(latitude);

            rect_coord.x = xn;
            rect_coord.y = yn;
            rect_coord.z = zn;
        }

        static horizontal equ2horizontal(double declination, double hour_angle, double latitude) {

            horizontal hor_coord;

            rectangular re = spherical2rect(spherical(hour_angle, declination));

            rotate_rect(re, latitude);

            hor_coord.azimuth = atan2_deg(re.y, re.x) + 180;
            hor_coord.altitude = asin_deg(re.z);

            return hor_coord;
        }

        /*
        *   all inputs and outputs in degrees
        */
        static horizontal horizontal_position(const spherical& coords, const terrestial& observer, const const date& d) {

            double hour_angle = date::gmst(d) + observer.longitude - coords.right_asc;
            return equ2horizontal(coords.decl, hour_angle, observer.latitude);
        }
    };
}