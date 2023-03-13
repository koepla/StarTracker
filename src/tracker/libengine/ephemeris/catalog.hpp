#ifndef LIBENGINE_EPHEMERIS_CATALOG_H
#define LIBENGINE_EPHEMERIS_CATALOG_H

#include <cfloat>
#include <filesystem>
#include <optional>
#include <unordered_set>
#include <vector>

#include "../math.hpp"
#include "coordinates.hpp"
#include "fixed-body.hpp"
#include "planet.hpp"

namespace ephemeris {

    class Catalog {
    private:
        std::vector<std::shared_ptr<Planet>> planets{};
        std::vector<std::shared_ptr<FixedBody>> bodies{};

    public:
        Catalog() noexcept = default;

        /**
         * Import the NGC2000 catalog and the corresponding common names
         * @param catalog Data of the ngc2000 catalog
         * @param names Data of the names
         * @return boolean value that indicates success
         */
        bool ImportFixed(std::string_view catalog, std::string_view names) noexcept;

        /**
         * Import the planetary catalog from `planets.json`
         * @param planetaryData Data of the planets
         * @return boolean value that indicates success
         */
        bool ImportPlanets(std::string_view planetaryData) noexcept;

        /**
         * Find a FixedBody by its designation
         * @param designation Designation for the search
         * @return reference or null
         */
        std::shared_ptr<FixedBody> FindFixedByDesignation(std::string_view designation) const noexcept;

        /**
         * Find a FixedBody by its name
         * @param name Name for the search
         * @return reference or null
         */
        std::shared_ptr<FixedBody> FindFixedByName(std::string_view name) const noexcept;

        /**
         * Find a Planet by its name
         * @param name Name for the search
         * @return reference or null
         */
        std::shared_ptr<Planet> FindPlanetByName(std::string_view name) const noexcept;

        struct VisibilityFilter {
            f64 AltitudeThreshold;
            Geographic Observer;
        };

        struct Filter {
            std::string_view Identifier;
            std::unordered_set<Classification> Classifications;
            std::unordered_set<std::string_view> Constellations;
            std::optional<VisibilityFilter> Visibility;
        };

        /**
         * Filter
         * @param filter Filter
         * @return matching FixedBodies
         */
        std::vector<std::shared_ptr<FixedBody>> FilterFixed(const Filter& filter) const noexcept;

        /*
         * Filter
         * @param filter Filter
         * @return matching Planets
         */
        std::vector<std::shared_ptr<Planet>> FilterPlanets(std::string_view filter) const noexcept;

        /**
         * Retrieves the planets
         * @return planets
         */
        std::vector<std::shared_ptr<Planet>>& GetPlanets() noexcept;

        /**
         * Retrieves the bodies
         * @return bodies
         */
        std::vector<std::shared_ptr<FixedBody>>& GetBodies() noexcept;
    };

    inline bool operator==(const Catalog::Filter& a, const Catalog::Filter& b) noexcept {
        const auto trivialChecks = a.Identifier == b.Identifier && a.Classifications == b.Classifications &&
                                   a.Constellations == b.Constellations;
        if (!trivialChecks) {
            return false;
        }

        if (a.Visibility) {
            if (b.Visibility) {
                const auto f64Equal = [](f64 a, f64 b) { return fabs(a - b) < DBL_EPSILON * 10.0; };
                return f64Equal(a.Visibility->AltitudeThreshold, b.Visibility->AltitudeThreshold) &&
                       f64Equal(a.Visibility->Observer.Latitude, b.Visibility->Observer.Latitude) &&
                       f64Equal(a.Visibility->Observer.Longitude, b.Visibility->Observer.Longitude);
            }
            return false;
        }
        if (b.Visibility) {
            return false;
        }
        return true;
    }

    inline bool operator!=(const Catalog::Filter& a, const Catalog::Filter& b) noexcept {
        return !(a == b);
    }

    struct ComputeResult {
        std::vector<f64> Altitudes;
        std::vector<f64> Azimuths;
    };

    struct ComputeInfo {
        DateTime Date;
        Geographic Observer;
        std::size_t Steps;
        std::size_t StepSize;
        DateTime::Unit Unit;

        ComputeInfo() noexcept;
    };

    ComputeResult ComputeGeographic(const std::shared_ptr<Planet>& planet, ComputeInfo info) noexcept;
    ComputeResult ComputeGeographic(const std::shared_ptr<FixedBody>& body, ComputeInfo info) noexcept;

    static inline const std::unordered_map<std::string_view, std::string_view> ConstellationExpansionTable = {
        { "And", "Andromeda" },
        { "Ant", "Antlia" },
        { "Aps", "Apus" },
        { "Aqr", "Aquarius" },
        { "Aql", "Aquila" },
        { "Ara", "Ara" },
        { "Ari", "Aries" },
        { "Aur", "Auriga" },
        { "Boo", "Bo�tes" },
        { "Cae", "Caelum" },
        { "Cam", "Camelopardalis" },
        { "Cnc", "Cancer" },
        { "CVn", "Canes Venatici" },
        { "CMa", "Canis Major" },
        { "CMi", "Canis Minor" },
        { "Cap", "Capricornus" },
        { "Car", "Carina" },
        { "Cas", "Cassiopeia" },
        { "Cen", "Centaurus" },
        { "Cep", "Cepheus" },
        { "Cet", "Cetus" },
        { "Cha", "Chamaeleon" },
        { "Cir", "Circinus" },
        { "Col", "Columba" },
        { "Com", "Coma Berenices" },
        { "CrA", "Corona Australis" },
        { "CrB", "Corona Borealis" },
        { "Crv", "Corvus" },
        { "Crt", "Crater" },
        { "Cru", "Crux" },
        { "Cyg", "Cygnus" },
        { "Del", "Delphinus" },
        { "Dor", "Dorado" },
        { "Dra", "Draco" },
        { "Equ", "Equuleus" },
        { "Eri", "Eridanus" },
        { "For", "Fornax" },
        { "Gem", "Gemini" },
        { "Gru", "Grus" },
        { "Her", "Hercules" },
        { "Hor", "Horologium" },
        { "Hya", "Hydra" },
        { "Hyi", "Hydrus" },
        { "Ind", "Indus" },
        { "Lac", "Lacerta" },
        { "Leo", "Leo" },
        { "Lib", "Libra" },
        { "LMi", "Leo Minor" },
        { "Lep", "Lepus" },
        { "Lup", "Lupus" },
        { "Lyn", "Lynx" },
        { "Lyr", "Lyra" },
        { "Men", "Mensa" },
        { "Mic", "Microscopium" },
        { "Mon", "Monoceros" },
        { "Mus", "Musca" },
        { "Nor", "Norma" },
        { "Oct", "Octans" },
        { "Oph", "Ophiuchus" },
        { "Ori", "Orion" },
        { "Pav", "Pavo" },
        { "Peg", "Pegasus" },
        { "Per", "Perseus" },
        { "Phe", "Phoenix" },
        { "Pic", "Pictor" },
        { "Psc", "Pisces" },
        { "PsA", "Piscis Austrinus" },
        { "Pup", "Puppis" },
        { "Pyx", "Pyxis" },
        { "Ret", "Reticulum" },
        { "Sge", "Sagitta" },
        { "Sgr", "Sagittarius" },
        { "Sco", "Scorpius" },
        { "Scl", "Sculptor" },
        { "Sct", "Scutum" },
        { "Ser", "Serpens" },
        { "Sex", "Sextans" },
        { "Tau", "Taurus" },
        { "Tel", "Telescopium" },
        { "Tri", "Triangulum" },
        { "TrA", "Triangulum Australe" },
        { "Tuc", "Tucana" },
        { "UMa", "Ursa Major" },
        { "UMi", "Ursa Minor" },
        { "Vel", "Vela" },
        { "Vir", "Virgo" },
        { "Vol", "Volans" },
        { "Vul", "Vulpecula" },
    };
}// namespace ephemeris

#endif// LIBENGINE_EPHEMERIS_CATALOG_H
