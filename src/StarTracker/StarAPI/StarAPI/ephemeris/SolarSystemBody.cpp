#include "SolarSystemBody.hpp"

#include <nlohmann/json.hpp>

namespace StarTracker::Ephemeris {

    SolarSystemBody::SolarSystemBody(
        const KeplerianElements& keplerElements, 
        const KeplerianElements& keplerElementsCentury) noexcept : 
        SolarSystemBody{ "Unnamed Celestial Body", keplerElements, keplerElementsCentury } {

    }

    SolarSystemBody::SolarSystemBody(
        const std::string& name, const KeplerianElements& keplerElements, 
        const KeplerianElements& keplerElementsCentury) noexcept : 
        CelestialBody{ name, {} },
        keplerElements{ keplerElements }, keplerElementsCentury{ keplerElementsCentury } {

    }

    StarTracker::Ephemeris::Coordinates::Spherical SolarSystemBody::GetSphericalPosition(const DateTime& date) const noexcept {

        const auto t = DateTime::JulianCenturies(date);

        KeplerianElements meanKeplerElem = {

            keplerElements.SemiMajorAxis + keplerElementsCentury.SemiMajorAxis * t,
            keplerElements.Eccentricity + keplerElementsCentury.Eccentricity * t,
            keplerElements.Inclination + keplerElementsCentury.Inclination * t,
            keplerElements.MeanLongitude + keplerElementsCentury.MeanLongitude * t,
            keplerElements.LonPerihelion + keplerElementsCentury.LonPerihelion * t,
            keplerElements.LonAscendingNode + keplerElementsCentury.LonAscendingNode * t
        };

        const auto w = meanKeplerElem.LonPerihelion;
        const auto Om = meanKeplerElem.LonAscendingNode;
        const auto I = meanKeplerElem.Inclination;

        // compute argument of perihelion and mean anomaly
        const auto perihelion = w - Om;
        const auto meanAnomaly = Math::Mod(meanKeplerElem.MeanLongitude - w, 360.0);

        // compute eccentric anomaly
        const auto eccentricAnomaly = computeEccentricAnomaly(meanAnomaly, meanKeplerElem.Eccentricity);


        // compute true Anomaly and distance of the Planet
        const auto [trueAnomaly, distance] = computeTrueAnomalyAndDistance(meanKeplerElem.SemiMajorAxis, eccentricAnomaly, meanKeplerElem.Eccentricity);

        // compute Planets heliocentric position in 3D-space
        const auto xh = distance * (Math::Cosine(Om) * Math::Cosine(trueAnomaly + w - Om) - Math::Sine(Om) * Math::Sine(trueAnomaly + w - Om) * Math::Cosine(I));
        const auto yh = distance * (Math::Sine(Om) * Math::Cosine(trueAnomaly + w - Om) + Math::Cosine(Om) * Math::Sine(trueAnomaly + w - Om) * Math::Cosine(I));
        const auto zh = distance * (Math::Sine(trueAnomaly + w - Om) * Math::Sine(I));
        
        // compute Planets rectangular ecliptic geocentric position in 3D-space
        Coordinates::Rectangular earthCoords = computeEarthPos(t);

        const auto xg = xh - earthCoords.X;
        const auto yg = yh - earthCoords.Y;
        const auto zg = zh - earthCoords.Z;

        // transform to rectangular equatorial coordinates
        const auto ecl = computeEcliptic(t);
    
        const auto xe = xg;
        const auto ye = yg * Math::Cosine(ecl) - zg * Math::Sine(ecl);
        const auto ze = yg * Math::Sine(ecl) + zg * Math::Cosine(ecl);

        Coordinates::Spherical sphericalCoords{};
        sphericalCoords.RightAscension = Math::ArcTangent2(ye, xe);
        sphericalCoords.Declination = Math::ArcTangent2(ze, std::sqrt(xe * xe + ye * ye));
        sphericalCoords.Radius = std::sqrt(xe * xe + ye * ye + ze * ze);

        return sphericalCoords;
    }

    double SolarSystemBody::computeEccentricAnomaly(double meanAnomaly, double eccentricity, double eps) const noexcept {

        const auto eccentricityDegrees = Math::Degrees(eccentricity);
        const auto eccentricityRadians = eccentricity;

        auto eccentricAnomaly = meanAnomaly + eccentricityDegrees * Math::Sine(meanAnomaly);
        auto deltaEccentric = double{ 0.0 };
        auto iteration = std::size_t{ 0 };

        do {

            const auto dm = meanAnomaly - (eccentricAnomaly - eccentricityDegrees * Math::Sine(eccentricAnomaly));
            deltaEccentric = dm / (1 - eccentricityRadians * Math::Cosine(eccentricAnomaly));
            eccentricAnomaly += deltaEccentric;

        } while (Math::Abs(deltaEccentric) > eps && ++iteration < 10);

        return eccentricAnomaly;
    }

    std::pair<double, double> SolarSystemBody::computeTrueAnomalyAndDistance(double semiMajorAxis, double eccentricAnomaly, double eccentricity) const noexcept {

        const auto xv = semiMajorAxis * (Math::Cosine(eccentricAnomaly) - eccentricity);
        const auto yv = semiMajorAxis * std::sqrt(1 - std::pow(eccentricity, 2)) * Math::Sine(eccentricAnomaly);

        const auto trueAnomaly = Math::ArcTangent2(yv, xv);
        const auto distance = std::sqrt(xv * xv + yv * yv);

        return std::make_pair(Math::Mod(trueAnomaly, 360.0), distance);
    }

    // Correct for drifting ecliptic due to planets pull on the Earth
    double SolarSystemBody::computeEcliptic(double julianCenturies) const noexcept {

        return (23.43929111 - (46.8150 + (0.00059 - 0.001813 * julianCenturies) * julianCenturies) * julianCenturies / 3600.0);
    }

    Coordinates::Rectangular SolarSystemBody::computeEarthPos(double julianCenturies) const noexcept {

        // The EM-Bary keplarian elements are hardcoded because they are needed for every computation

        KeplerianElements meanEarth{};
        meanEarth.SemiMajorAxis = 1.00000261 + 0.00000562 * julianCenturies;
        meanEarth.Eccentricity = 0.01671022 - 0.00003804 * julianCenturies;
        meanEarth.Inclination = 0.00005 - 0.01294668 * julianCenturies;
        meanEarth.MeanLongitude = 100.46457166 + 35999.37244981 * julianCenturies;
        meanEarth.LonPerihelion = 102.93768193 + 0.32327364 * julianCenturies;
        meanEarth.LonAscendingNode = -11.26064 - (18228.25 / 3600.0) * julianCenturies;

        const auto w = meanEarth.LonPerihelion;
        const auto Om = meanEarth.LonAscendingNode;
        const auto I = meanEarth.Inclination;

        const auto perihelion = w - Om;
        const auto meanAnomaly = Math::Mod(meanEarth.MeanLongitude - w, 360.0);

        // compute eccentric anomaly
        const auto eccentricAnomaly = computeEccentricAnomaly(meanAnomaly, meanEarth.Eccentricity);

        // compute true Anomaly and distance
        const auto [trueAnomaly, distance] = computeTrueAnomalyAndDistance(meanEarth.SemiMajorAxis, eccentricAnomaly, meanEarth.Eccentricity);
        const auto longitudeEarth = trueAnomaly + w;

        Coordinates::Rectangular earthCoords{};
        earthCoords.X = distance * Math::Cosine(longitudeEarth);
        earthCoords.Y = distance * Math::Sine(longitudeEarth);
        earthCoords.Z = 0;

        return earthCoords;
    }
}