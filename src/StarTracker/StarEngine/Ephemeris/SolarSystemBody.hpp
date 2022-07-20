#ifndef STARENGINE_EPHEMERIS_SOLARSYSTEMBODY_H
#define STARENGINE_EPHEMERIS_SOLARSYSTEMBODY_H

#include <StarEngine/Ephemeris/CelestialBody.hpp>
#include <StarEngine/Ephemeris/KeplerianElements.hpp>

namespace StarTracker::Ephemeris {

    class SolarSystemBody : public CelestialBody
    {
    private:
        KeplerianElements keplerElements;
        KeplerianElements keplerElementsCentury;

    public:
        SolarSystemBody(std::string_view name, std::string_view textureHandle, const KeplerianElements& keplerElements, const KeplerianElements& keplerElementsCentury) noexcept;

        /**
        * Computes the spherical position of the solar system body
        *
        * @param date date and time for the computation
        *
        * @return the computed spherical coordinates
        *
        */
        [[nodiscard]] virtual Coordinates::Spherical GetSphericalPosition(const DateTime& date) const noexcept override;

        /**
        * @brief Formats the SolarSystemBody as JSON-Object
        *
        * @return JSON-formatted SolarSystemBody
        *
        */
        [[nodiscard]] virtual std::string GetSerializable() const noexcept override;

    private:
        [[nodiscard]] double computeEccentricAnomaly(double meanAnomaly, double eccentricity, double eps = 1e-12) const noexcept;
        [[nodiscard]] std::pair<double, double> computeTrueAnomalyAndDistance(double semiMajorAxis, double eccentricAnomaly, double eccentricity) const noexcept;
        [[nodiscard]] double computeEcliptic(double julianCenturies) const noexcept;
        [[nodiscard]] Coordinates::Rectangular computeEarthPos(double julianCenturies) const noexcept;
    };
}

#endif // STARENGINE_EPHEMERIS_SOLARSYSTEMBODY_H