#ifndef STARENGINE_EPHEMERIS_FIXEDBODY_H
#define STARENGINE_EPHEMERIS_FIXEDBODY_H

#include <StarEngine/Ephemeris/CelestialBody.hpp>

namespace StarTracker::Ephemeris {

    class FixedBody : public CelestialBody {

    private:
        Coordinates::Spherical position;

    public:
        FixedBody(std::string_view name, std::string_view textureHandle, const Coordinates::Spherical& position) noexcept;
        FixedBody(std::string_view name, std::string_view designation, std::string_view textureHandle, const Coordinates::Spherical& position) noexcept;

        /**
        * Computes the spherical position of the celestial body
        *
        * @param date date and time for the computation
        *
        * @return the computed spherical coordinates
        *
        */
        [[nodiscard]] virtual Coordinates::Spherical GetSphericalPosition(const DateTime& date) const noexcept override;

        /**
        * @brief Formats the FixedBody as JSON-Object
        *
        * @return JSON-formatted FixedBody
        *
        */
        [[nodiscard]] virtual std::string GetSerializable() const noexcept override;
    };
}

#endif // STARENGINE_EPHEMERIS_FIXEDBODY_H