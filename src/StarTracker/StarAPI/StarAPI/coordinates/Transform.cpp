#include "Transform.hpp"

namespace Star::Coordinates {

    Rectangular Transform::SphericalToRectangular(const Spherical& coords) {

        Rectangular rectCoords;

        rectCoords.X = coords.Radius * Math::Cosine(coords.RightAscension) * Math::Cosine(coords.Declination);
        rectCoords.Y = coords.Radius * Math::Sine(coords.RightAscension) * Math::Cosine(coords.Declination);
        rectCoords.Z = coords.Radius * Math::Sine(coords.Declination);

        return rectCoords;
    }

    /*
        input:
        &spherical_coord (spherical coordinates (reference)),
        x,y,z horizontal coordinates

        routine:
        transforms rectangular coordinates to spherical ones
    */
    Spherical Transform::RectangularToSpherical(const Rectangular& coords) {

        Spherical sphericalCoords;

        double x = coords.X;
        double y = coords.Y;
        double z = coords.Z;

        //celestial north and south pole
        if (x == 0 && y == 0) {

            sphericalCoords.RightAscension = 0;
        }

        sphericalCoords.Radius = sqrt(x * x + y * y + z * z);
        sphericalCoords.RightAscension = Math::ArcTangent2(y, x);
        sphericalCoords.Declination = Math::ArcTangent2(z, sqrt(x * x + y * y));

        return sphericalCoords;
    }

	void Transform::RotateY(Rectangular& rectCoords, double angle) {

		double xn = rectCoords.X * Math::Sine(angle) - rectCoords.Z * Math::Cosine(angle);
		double yn = rectCoords.Y;
		double zn = rectCoords.X * Math::Cosine(angle) + rectCoords.Z * Math::Sine(angle);

		rectCoords.X = xn;
		rectCoords.Y = yn;
		rectCoords.Z = zn;
	}

	Horizontal Transform::EquatorialToHorizontal(double declination, double hourAngle, double latitude) {

		Horizontal horizontalCoords;

		Rectangular re = SphericalToRectangular(Spherical(hourAngle, declination));

		RotateY(re, latitude);

		// add 180 to get the angle from north to east to south and so on
		horizontalCoords.Azimuth = Math::ArcTangent2(re.Y, re.X) + 180;
		horizontalCoords.Altitude = Math::ArcSine(re.Z);

		return horizontalCoords;
	}

	Horizontal Transform::TerrestrialObserverToHorizontal(const Spherical& sphericalCoords, const Terrestrial& observer, const DateTime& date) {

		double hourAngle = DateTime::Gmst(date) + observer.Longitude - sphericalCoords.RightAscension;
		return EquatorialToHorizontal(sphericalCoords.Declination, hourAngle, observer.Latitude);
	}
}