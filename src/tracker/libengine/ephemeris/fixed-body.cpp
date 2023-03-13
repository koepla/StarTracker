#include "fixed-body.hpp"
#include "math.hpp"

namespace ephemeris {

    Equatorial FixedBody::GetEquatorialPosition(const DateTime& dateTime) const noexcept {
        const auto cartesian = EquatorialToVector(Position);

        constexpr auto epochB2000 = -0.000012775;
        const auto epochOfDate = DateTime::JulianCenturies(dateTime);
        const auto precessed = PrecessionMatrix(ReferencePlane::Equatorial, epochB2000, epochOfDate) * cartesian;
        return VectorToEquatorial(precessed);
    }

    const char* ClassificationToString(Classification classification) noexcept {
        switch (classification) {
            case Classification::Galaxy:
                return "Galaxy";
            case Classification::OpenStarCluster:
                return "Open Star Cluster";
            case Classification::GlobularStarCluster:
                return "Globular Star Cluster";
            case Classification::ReflectionNebula:
                return "Reflection Nebula";
            case Classification::PlanetaryNebula:
                return "Planetary Nebula";
            case Classification::Cluster:
                return "Cluster";
            case Classification::Asterism:
                return "Asterism";
            case Classification::Knot:
                return "Knot";
            case Classification::TripleStar:
                return "Triple Star";
            case Classification::DoubleStar:
                return "Double Star";
            case Classification::SingleStar:
                return "Single Star";
            case Classification::Uncertain:
                return "Uncertain";
            case Classification::Unidentified:
                return "Unidentified";
            case Classification::Nonexistent:
                return "Nonexistent";
            case Classification::PhotographicPlateDefect:
                return "Photographic plate defect";
            case Classification::Planet:
                return "Planet";
        }
        return "";
    }
}// namespace ephemeris