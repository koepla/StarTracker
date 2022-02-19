#include "Transform.hpp"

namespace StarTracker::Ephemeris::Coordinates {

    Rectangular Transform::SphericalToRectangular(const Spherical& coords) noexcept {

        Rectangular rectCoords{};
        rectCoords.X = coords.Radius * Math::Cosine(coords.RightAscension) * Math::Cosine(coords.Declination);
        rectCoords.Y = coords.Radius * Math::Sine(coords.RightAscension) * Math::Cosine(coords.Declination);
        rectCoords.Z = coords.Radius * Math::Sine(coords.Declination);
        
        return rectCoords;
    }

    Spherical Transform::RectangularToSpherical(const Rectangular& coords) noexcept {

        const auto x = coords.X;
        const auto y = coords.Y;
        const auto z = coords.Z;
        
        Spherical sphericalCoords{};
        sphericalCoords.Radius = sqrt(x * x + y * y + z * z);
        sphericalCoords.RightAscension = (x == 0 && y == 0) ? 0 : Math::ArcTangent2(y, x);
        sphericalCoords.Declination = Math::ArcTangent2(z, sqrt(x * x + y * y));
        
        return sphericalCoords;
    }

	Rectangular Transform::RotateRectangular(const Rectangular& rectCoords, double angle) noexcept {

        const auto x = rectCoords.X * Math::Sine(angle) - rectCoords.Z * Math::Cosine(angle);
        const auto y = rectCoords.Y;
        const auto z = rectCoords.X * Math::Cosine(angle) + rectCoords.Z * Math::Sine(angle);

        Rectangular rectangularCoords{};
        rectangularCoords.X = x;
        rectangularCoords.Y = y;
        rectangularCoords.Z = z;

        return rectangularCoords;
	}

	Horizontal Transform::EquatorialToHorizontal(double declination, double hourAngle, double latitude) noexcept {
        
        Spherical sphericalPosition{};
        sphericalPosition.RightAscension = hourAngle;
        sphericalPosition.Declination = declination;
        sphericalPosition.Radius = 1.0;
        
        const auto rectangularPosition = SphericalToRectangular(sphericalPosition);
        const auto rotatedRectangularPosition = RotateRectangular(rectangularPosition, latitude);
        
        // add 180 to get the angle from north to east to south and so on
        Horizontal horizontalCoords{};
        horizontalCoords.Azimuth = Math::ArcTangent2(rotatedRectangularPosition.Y, rotatedRectangularPosition.X) + 180.0;
        horizontalCoords.Altitude = Math::ArcSine(rotatedRectangularPosition.Z);

		return horizontalCoords;
	}

	Horizontal Transform::TerrestrialObserverToHorizontal(const Spherical& sphericalCoords, const Terrestrial& observer, const DateTime& date) noexcept {
        
        DateTime relativeUtcTime = date;
        relativeUtcTime.AddHours(-DateTime::UtcDiff());

        const auto localMeanSiderealTime = DateTime::Gmst(relativeUtcTime) + observer.Longitude;
        const auto hourAngle = localMeanSiderealTime - sphericalCoords.RightAscension;
		return EquatorialToHorizontal(sphericalCoords.Declination, hourAngle, observer.Latitude);
	}
}