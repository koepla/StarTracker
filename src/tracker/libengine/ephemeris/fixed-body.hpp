#ifndef LIBENGINE_EPHEMERIS_FIXEDBODY_H
#define LIBENGINE_EPHEMERIS_FIXEDBODY_H

#include <string>
#include <unordered_map>

#include "coordinates.hpp"

namespace ephemeris {

    enum class Classification {
        Galaxy,
        OpenStarCluster,
        GlobularStarCluster,
        ReflectionNebula,
        PlanetaryNebula,
        Cluster,
        Asterism,
        Knot,
        TripleStar,
        DoubleStar,
        SingleStar,
        Uncertain,
        Unidentified,
        PhotographicPlateDefect,
        Nonexistent,
        Planet
    };

    struct Constellation {
        std::string Name;
        std::string Abbreviation;
    };

    struct FixedBody {
        std::string Name;
        std::string Designation;
        std::string Description;
        Constellation Const;
        f64 Dimension;
        f64 Magnitude;
        Classification Type;
        Equatorial Position;

        /**
         * Computes the precessed equatorial position of the fixed body with the equinox of date
         * @param dateTime Date for computation
         * @return precessed position
         */
        Equatorial GetEquatorialPosition(const DateTime& dateTime) const noexcept;
    };

    const char* ClassificationToString(Classification classification) noexcept;
}// namespace ephemeris

#endif// LIBENGINE_EPHEMERIS_FIXEDBODY_H