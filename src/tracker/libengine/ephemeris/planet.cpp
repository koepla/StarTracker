#include <utility>

#include "coordinates.hpp"
#include "libengine/math.hpp"
#include "planet.hpp"

namespace ephemeris {

    namespace {

        f64 EccentricAnomaly(f64 meanAnomaly, f64 eccentricity) noexcept {
            const auto eccentricityDegrees = math::Degrees(eccentricity);
            const auto eccentricityRadians = eccentricity;

            auto eccentricAnomaly = meanAnomaly + eccentricityDegrees * math::Sine(meanAnomaly);
            f64 deltaEccentric;
            std::size_t iteration = 0;

            do {
                const auto dm = meanAnomaly - (eccentricAnomaly - eccentricityDegrees * math::Sine(eccentricAnomaly));
                deltaEccentric = dm / (1 - eccentricityRadians * math::Cosine(eccentricAnomaly));
                eccentricAnomaly += deltaEccentric;

            } while (math::Abs(deltaEccentric) > 1.0E-12 && ++iteration < 10);
            return eccentricAnomaly;
        }

        Vector3 PositionOfEarth(f64 julianCenturies) noexcept {
            // The EM-Barycenter kepler elements are hardcoded because they are needed for every computation
            Elements meanEarth{};
            meanEarth.SemiMajorAxis = 1.00000261 + 0.00000562 * julianCenturies;
            meanEarth.Eccentricity = 0.01671022 - 0.00003804 * julianCenturies;
            meanEarth.MeanLongitude = 100.46457166 + 35999.37244981 * julianCenturies;
            meanEarth.LonPerihelion = 102.93768193 + 0.32327364 * julianCenturies;

            const auto a = meanEarth.SemiMajorAxis;
            const auto e = meanEarth.Eccentricity;
            const auto L = meanEarth.MeanLongitude;
            const auto w = meanEarth.LonPerihelion;

            const auto M = math::Mod(L - w, 360.0);
            const auto E = EccentricAnomaly(M, e);

            Vector3 positionInOrbit{};
            positionInOrbit.X = a * (math::Cosine(E) - e);
            positionInOrbit.Y = a * std::sqrt(1 - e * e) * math::Sine(E);
            positionInOrbit.Z = 0;

            return RotationMatrix(RotationAxis::Z, w) * positionInOrbit;
        }
    }// namespace

    Equatorial Planet::GetEquatorialPosition(const DateTime& date) const noexcept {
        const auto t = DateTime::JulianCenturies(date);

        Elements meanKeplerElem{};
        meanKeplerElem.SemiMajorAxis = Orbit.SemiMajorAxis + Rate.SemiMajorAxis * t;
        meanKeplerElem.Eccentricity = Orbit.Eccentricity + Rate.Eccentricity * t;
        meanKeplerElem.Inclination = Orbit.Inclination + Rate.Inclination * t;
        meanKeplerElem.MeanLongitude = Orbit.MeanLongitude + Rate.MeanLongitude * t;
        meanKeplerElem.LonPerihelion = Orbit.LonPerihelion + Rate.LonPerihelion * t;
        meanKeplerElem.LonAscendingNode = Orbit.LonAscendingNode + Rate.LonAscendingNode * t;

        const auto a = meanKeplerElem.SemiMajorAxis;
        const auto e = meanKeplerElem.Eccentricity;
        const auto w = meanKeplerElem.LonPerihelion;
        const auto Om = meanKeplerElem.LonAscendingNode;
        const auto L = meanKeplerElem.MeanLongitude;
        const auto I = meanKeplerElem.Inclination;

        // compute argument of perihelion and mean anomaly
        const auto perihelion = w - Om;
        const auto M = math::Mod(L - w, 360.0);

        // compute eccentric anomaly
        const auto E = EccentricAnomaly(M, e);

        // true anomaly
        Vector3 positionInOrbit{};
        positionInOrbit.X = a * (math::Cosine(E) - e);
        positionInOrbit.Y = a * std::sqrt(1 - e * e) * math::Sine(E);
        positionInOrbit.Z = 0;

        const auto helioEcliptic = RotationMatrix(RotationAxis::Z, Om) * RotationMatrix(RotationAxis::X, I) *
                                   RotationMatrix(RotationAxis::Z, perihelion) * positionInOrbit;

        const auto geoEcliptic = helioEcliptic - PositionOfEarth(t);
        const auto geoEquatorial =
                ReferencePlaneMatrix(ReferencePlane::Ecliptic, ReferencePlane::Equatorial, 0) * geoEcliptic;
        return VectorToEquatorial(PrecessionMatrix(ReferencePlane::Equatorial, 0, t) * geoEquatorial);
    }
}// namespace ephemeris