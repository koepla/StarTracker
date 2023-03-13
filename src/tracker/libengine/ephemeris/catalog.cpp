#include <fmt/format.h>
#include <charconv>
#include <nlohmann/json.hpp>
#include <optional>

#include "../math.hpp"
#include "catalog.hpp"
#include "utility/conversion.hpp"

namespace ephemeris {

    namespace {

        const std::unordered_map<std::string_view, Classification> ClassificationLookUpTable = {
            { " Gx", Classification::Galaxy },
            { " OC", Classification::OpenStarCluster },
            { " Gb", Classification::GlobularStarCluster },
            { " Nb", Classification::ReflectionNebula },
            { " Pl", Classification::PlanetaryNebula },
            { "C+N", Classification::Cluster },
            { "Ast", Classification::Asterism },
            { " Kt", Classification::Knot },
            { "***", Classification::TripleStar },
            { " D*", Classification::DoubleStar },
            { "  *", Classification::SingleStar },
            { "  ?", Classification::Uncertain },
            { "   ", Classification::Unidentified },
            { "  -", Classification::Nonexistent },
            { " PD", Classification::PhotographicPlateDefect }
        };

        const std::unordered_map<std::string_view, std::string_view> CatalogDescriptionExpansionTable = {
            { "ab", "about" },
            { "alm", "almost" },
            { "am", "among" },
            { "annul", "annular or ring nebula" },
            { "att", "attached" },
            { "b", "brighter" },
            { "bet", "between" },
            { "biN", "binuclear" },
            { "bn", "brightest to n side" },
            { "bs", "brightest to s side" },
            { "bp", "brightest to p side" },
            { "bf", "brightest to f side" },
            { "B", "bright" },
            { "c", "considerably" },
            { "chev", "chevelure" },
            { "co", "coarse, coarsely" },
            { "com", "cometic (cometary form)" },
            { "comp", "companion " },
            { "conn", "connected" },
            { "cont", "in contact" },
            { "C", "compressed" },
            { "Cl", "cluster" },
            { "d", "diameter" },
            { "def", "defined" },
            { "dif", "diffused" },
            { "diffic", "difficult" },
            { "dist", "distance, or distant" },
            { "D", "f64" },
            { "e", "extremely, excessively" },
            { "ee", "most extremely" },
            { "er", "easily resolvable" },
            { "exc", "excentric" },
            { "E", "extended" },
            { "f", "following (eastward)" },
            { "F", "faint" },
            { "g", "gradually" },
            { "glob.", "globular" },
            { "gr", "group" },
            { "i", "irregular" },
            { "iF", "irregular figure" },
            { "inv", "involved, involving" },
            { "l", "little or long" },
            { "L", "large" },
            { "m", "much" },
            { "m", "magnitude" },
            { "M", "middle, or in the middle" },
            { "n", "north" },
            { "neb", "nebula" },
            { "nebs", "nebulous" },
            { "neby", "nebulosity" },
            { "nf", "north following" },
            { "np", "north preceding" },
            { "ns", "north-south" },
            { "nr", "near" },
            { "N", "nucleus, or to a nucleus" },
            { "p", "preceding (westward)" },
            { "pf", "preceding-following" },
            { "pF", "pretty faint" },
            { "pB", "pretty bright" },
            { "pL", "pretty large" },
            { "pS", "pretty small in angular size" },
            { "pg", "pretty gradually" },
            { "pm", "pretty much" },
            { "ps", "pretty suddenly" },
            { "plan", "planetary nebula (same as PN)" },
            { "prob", "probably" },
            { "P", "poor (sparse) in stars" },
            { "PN", "planetary nebula" },
            { "r", "resolvable (mottled, not resolved)" },
            { "rr", "partially resolved, some stars seen" },
            { "rrr", "well resolved, clearly consisting of stars" },
            { "R", "round" },
            { "RR", "exactly round" },
            { "Ri", "rich in stars" },
            { "s", "suddenly (abruptly)" },
            { "s", "south" },
            { "sf", "south following" },
            { "sp", "south preceding" },
            { "sc", "scattered" },
            { "sev", "several" },
            { "st", "stars" },
            { "9...", "of 9th magnitude and fainter" },
            { "9..13", "of mag. 9 to 13" },
            { "stell", "stellar, pointlike" },
            { "susp", "suspected" },
            { "S", "small in angular size" },
            { "S*", "small (faint) star" },
            { "trap", "trapezium" },
            { "triangle", "triangle, forms a triangle with" },
            { "triN", "trinuclear" },
            { "v", "very" },
            { "vv", "very" },
            { "var", "variable" },
            { "*", "a single star" },
            { "*10", "a star of 10th magnitude" },
            { "*7-8", "star of mag. 7 or 8" },
            { "**", "f64 star (same as D*)" },
            { "***", "triple star" },
            { "!", "remarkable" },
            { "!!", "very much so" },
            { "!!!", "a magnificent or otherwise interesting object" }
        };

        /**
         * Checks if a planet is properly formatted
         * @param entry potential planet
         * @return true if the entry appears to be valid, false if the entry is malformed
         */
        bool IsPlanetProperlyFormatted(const nlohmann::json& entry) noexcept {
            return entry.contains("Name") && entry.contains("SemiMajorAxis") &&
                   entry.contains("SemiMajorAxisCentury") && entry.contains("Eccentricity") &&
                   entry.contains("EccentricityCentury") && entry.contains("Inclination") &&
                   entry.contains("InclinationCentury") && entry.contains("MeanLongitude") &&
                   entry.contains("MeanLongitudeCentury") && entry.contains("LonPerihelion") &&
                   entry.contains("LonPerihelionCentury") && entry.contains("LonAscendingNode") &&
                   entry.contains("LonAscendingNodeCentury");
        }

        /**
         * Checks if there are any leading whitespaces and removes them
         * @param view Input that shall be trimmed
         * @return trimmed
         */
        std::string_view LeftTrim(std::string_view view) {
            auto trimmed = view;
            for (const auto c : trimmed) {
                if (std::isspace(c)) {
                    trimmed.remove_prefix(1);
                } else {
                    break;
                }
            }
            return trimmed;
        }

        /**
         * Checks if there are any whitespaces at the end and removes them
         * @param view Input that shall be trimmed
         * @return trimmed
         */
        std::string_view RightTrim(std::string_view view) {
            auto trimmed = view;
            for (auto it = view.crbegin(); it != view.crend(); ++it) {
                if (std::isspace(*it)) {
                    trimmed.remove_suffix(1);
                } else {
                    break;
                }
            }
            return trimmed;
        }

        /**
         * Expand a ngc2000 catalog description, as of
         * http://cdsarc.u-strasbg.fr/viz-bin/ReadMe/VII/118?format=html&tex=true
         * @param abbreviated Description in abbreviated form
         * @return expanded description
         */
        std::string ExpandCatalogDescription(std::string_view abbreviated) {
            // Here is what we have to do:
            // First, split by ` `
            // then try to match each word with the expansion table, if there is no match,
            // expand each character
            std::string result;
            std::vector<std::string_view> words{};
            std::vector<std::string_view> parts{};
            utility::Split(parts, abbreviated, ", "sv);
            for (const auto& part : parts) {
                utility::Split(words, part, " "sv);
            }

            for (const auto& word : words) {
                if (CatalogDescriptionExpansionTable.find(word) != CatalogDescriptionExpansionTable.end()) {
                    result += CatalogDescriptionExpansionTable.at(word);
                    result += " ";
                } else {
                    for (const auto c : word) {
                        const auto view = std::string_view{ &c, 1 };
                        if (CatalogDescriptionExpansionTable.find(view) != CatalogDescriptionExpansionTable.end()) {
                            result += CatalogDescriptionExpansionTable.at(view);
                        } else {
                            result += view;
                        }
                        result += " ";
                    }
                }
            }
            return result;
        }

        /**
         * Tries to obtain a FixedBody from an NGC2000.dat entry
         * @param data Entry line
         * @return optional FixedBody
         */
        std::optional<FixedBody> ParseCatalogEntry(std::string_view data) {
            if (data.size() < 46) {
                return {};
            }

            FixedBody body{};

            // When there is a leading I in the designation, it is a IC designation, otherwise NGC
            body.Designation = data.substr(0, 5);
            if (body.Designation[0] == 'I') {
                body.Designation = fmt::format("IC{}", LeftTrim(body.Designation.substr(1)));
            } else {
                body.Designation = fmt::format("NGC{}", LeftTrim(body.Designation));
            }

            // Classification is encoded in three characters
            const auto classification = data.substr(6, 3);
            if (classification.find('?') != std::string_view::npos) {
                body.Type = Classification::Uncertain;
            } else if (ClassificationLookUpTable.find(classification) != ClassificationLookUpTable.end()) {
                body.Type = ClassificationLookUpTable.at(classification);
            } else {
                return {};
            }

            // Right Ascension
            const auto hours = utility::FromString<f64>(LeftTrim(data.substr(10, 2)));
            const auto minutes = utility::FromString<f64>(LeftTrim(data.substr(13, 4)));
            if (hours && minutes) {
                body.Position.RightAscension = math::HmsToDegrees(*hours, *minutes, 0.0);
            } else {
                return {};
            }

            // Declination
            const auto sign = data.at(19);
            const auto degrees = utility::FromString<f64>(LeftTrim(data.substr(20, 2)));
            const auto arcMinutes = utility::FromString<f64>(LeftTrim(data.substr(23, 2)));
            if (degrees && arcMinutes) {
                body.Position.Declination =
                        math::DaaToDegrees(sign == '-' ? -1.0 * *degrees : *degrees, *arcMinutes, 0.0);
            } else {
                return {};
            }

            // Distance is not an entry in the NGC data set
            body.Position.Radius = 1.0;

            // Constellation
            body.Const.Abbreviation = data.substr(29, 3);
            if (ConstellationExpansionTable.find(body.Const.Abbreviation) != ConstellationExpansionTable.end()) {
                body.Const.Name = ConstellationExpansionTable.at(body.Const.Abbreviation);
            }

            // Dimension
            if (const auto dimension = utility::FromString<f64>(LeftTrim(data.substr(33, 5)))) {
                body.Dimension = *dimension;
            }

            // Magnitude
            if (const auto magnitude = utility::FromString<f64>(LeftTrim(data.substr(40, 4)))) {
                body.Magnitude = *magnitude;
            }

            // Description
            body.Description = ExpandCatalogDescription(data.substr(46));

            return body;
        }

        struct CommonNameEntry {
            std::string Name;
            std::string Designation;
        };

        std::optional<CommonNameEntry> ParseCommonNameEntry(std::string_view data) {
            if (data.size() < 36) {
                return {};
            }

            CommonNameEntry entry{};
            entry.Name = RightTrim(data.substr(0, 36));
            if (entry.Name[0] == 'M') {
                entry.Name = fmt::format("Messier {}", LeftTrim(entry.Name.substr(2)));
            }

            entry.Designation = data.substr(36, 5);
            if (entry.Designation[0] == 'I') {
                entry.Designation = fmt::format("IC{}", LeftTrim(entry.Designation.substr(1)));
            } else {
                entry.Designation = fmt::format("NGC{}", LeftTrim(entry.Designation));
            }
            return entry;
        }

        bool ContainsIgnoreCase(std::string_view haystack, std::string_view needle) {
            const auto it = std::search(haystack.begin(), haystack.end(), needle.begin(), needle.end(),
                                        [](char ch1, char ch2) { return std::tolower(ch1) == std::tolower(ch2); });
            return it != haystack.end();
        }
    }// namespace

    bool Catalog::ImportFixed(std::string_view catalog, std::string_view names) noexcept {
        std::vector<std::string_view> data{};
        utility::Split(data, catalog, "\n"sv);
        for (const auto& entry : data) {
            if (auto body = ParseCatalogEntry(entry)) {
                bodies.emplace_back(std::make_shared<FixedBody>(std::move(*body)));
            }
        }

        std::vector<std::string_view> nameEntries{};
        utility::Split(nameEntries, names, "\n"sv);
        for (const auto& entry : nameEntries) {
            if (auto name = ParseCommonNameEntry(entry)) {
                if (const auto body = FindFixedByDesignation(name->Designation)) {
                    body->Name = std::move(name->Name);
                }
            }
        }

        // Sort them by dimension
        std::sort(bodies.begin(), bodies.end(),
                  [](const std::shared_ptr<FixedBody>& a, const std::shared_ptr<FixedBody>& b) {
                      return a->Dimension > b->Dimension;
                  });

        return true;
    }

    bool Catalog::ImportPlanets(std::string_view planetaryData) noexcept {
        const auto objects = nlohmann::json::parse(planetaryData, nullptr, false, true);
        if (!objects.contains("Data") || !objects["Data"].is_array()) {
            return false;
        }

        for (const auto& element : objects["Data"]) {
            if (!IsPlanetProperlyFormatted(element)) {
                continue;
            }

            Elements elements{};
            elements.SemiMajorAxis = element["SemiMajorAxis"].get<f64>();
            elements.Eccentricity = element["Eccentricity"].get<f64>();
            elements.Inclination = element["Inclination"].get<f64>();
            elements.MeanLongitude = element["MeanLongitude"].get<f64>();
            elements.LonPerihelion = element["LonPerihelion"].get<f64>();
            elements.LonAscendingNode = element["LonAscendingNode"].get<f64>();

            Elements rate{};
            rate.SemiMajorAxis = element["SemiMajorAxisCentury"].get<f64>();
            rate.Eccentricity = element["EccentricityCentury"].get<f64>();
            rate.Inclination = element["InclinationCentury"].get<f64>();
            rate.MeanLongitude = element["MeanLongitudeCentury"].get<f64>();
            rate.LonPerihelion = element["LonPerihelionCentury"].get<f64>();
            rate.LonAscendingNode = element["LonAscendingNodeCentury"].get<f64>();

            Planet planet{ element["Name"].get<std::string>(), elements, rate };
            planets.emplace_back(std::make_shared<Planet>(planet));
        }
        return true;
    }

    std::shared_ptr<FixedBody> Catalog::FindFixedByDesignation(std::string_view designation) const noexcept {
        for (const auto& fixed : bodies) {
            if (fixed->Designation == designation) {
                return fixed;
            }
        }
        return nullptr;
    }

    std::shared_ptr<FixedBody> Catalog::FindFixedByName(std::string_view name) const noexcept {
        for (const auto& fixed : bodies) {
            if (fixed->Name == name) {
                return fixed;
            }
        }
        return nullptr;
    }

    std::shared_ptr<Planet> Catalog::FindPlanetByName(std::string_view name) const noexcept {
        for (const auto& planet : planets) {
            if (planet->Name == name) {
                return planet;
            }
        }
        return nullptr;
    }

    std::vector<std::shared_ptr<FixedBody>> Catalog::FilterFixed(const Filter& filter) const noexcept {
        const auto ignoreIdentifier = filter.Identifier.empty();
        const auto ignoreClassification = filter.Classifications.empty();
        const auto ignoreConstellation = filter.Constellations.empty();
        const auto ignoreVisibility = !filter.Visibility.has_value();

        if (ignoreIdentifier && ignoreClassification && ignoreConstellation && ignoreVisibility) {
            return bodies;
        }

        std::vector<std::shared_ptr<FixedBody>> result{};
        std::vector<std::string_view> filters{};

        if (!ignoreIdentifier) {
            utility::Split(filters, filter.Identifier, ";"sv);
        }

        for (const auto& body : bodies) {
            const auto classificationSatisfied =
                    ignoreClassification || filter.Classifications.find(body->Type) != filter.Classifications.end();
            const auto identifierSatisfied = std::invoke([&] {
                if (!ignoreIdentifier) {
                    for (const auto& currentFilter : filters) {
                        if (ContainsIgnoreCase(body->Name, currentFilter)) {
                            return true;
                        }
                        if (ContainsIgnoreCase(body->Designation, currentFilter)) {
                            return true;
                        }
                    }
                    return false;
                }
                return true;
            });
            const auto constellationSatisfied =
                    ignoreConstellation ||
                    filter.Constellations.find(body->Const.Abbreviation) != filter.Constellations.end();

            const auto visibilitySatisfied = std::invoke([&] {
                if (!ignoreVisibility) {
                    const auto now = DateTime::Now();
                    const auto preview =
                            ObserveGeographic(body->GetEquatorialPosition(now), filter.Visibility->Observer, now);
                    return preview.Altitude >= filter.Visibility->AltitudeThreshold;
                }
                return true;
            });


            if (classificationSatisfied && identifierSatisfied && constellationSatisfied && visibilitySatisfied) {
                result.emplace_back(body);
            }
        }

        return result;
    }

    std::vector<std::shared_ptr<Planet>> Catalog::FilterPlanets(std::string_view filter) const noexcept {
        if (filter.empty()) {
            return planets;
        }

        std::vector<std::shared_ptr<Planet>> result{};
        std::vector<std::string_view> filters{};
        utility::Split(filters, filter, ";"sv);

        for (const auto& planet : planets) {
            for (const auto& currentFilter : filters) {
                if (ContainsIgnoreCase(planet->Name, currentFilter)) {
                    result.emplace_back(planet);
                    break;
                }
            }
        }

        return result;
    }

    std::vector<std::shared_ptr<Planet>>& Catalog::GetPlanets() noexcept {
        return planets;
    }

    std::vector<std::shared_ptr<FixedBody>>& Catalog::GetBodies() noexcept {
        return bodies;
    }

    ComputeInfo::ComputeInfo() noexcept
        : Date(DateTime::Now()),
          Observer({ 0.0, 0.0 }),
          Steps(1440),
          StepSize(1),
          Unit(DateTime::Unit::Minutes) { }

    ComputeResult ComputeGeographic(const std::shared_ptr<Planet>& planet, ComputeInfo info) noexcept {
        ComputeResult result{};
        result.Altitudes.resize(info.Steps);
        result.Azimuths.resize(info.Steps);
        for (std::size_t step = 0; step < info.Steps; ++step) {
            const auto position = ObserveGeographic(planet->GetEquatorialPosition(info.Date), info.Observer, info.Date);
            result.Altitudes[step] = position.Altitude;
            result.Azimuths[step] = position.Azimuth;
            info.Date.Add(static_cast<std::int64_t>(info.StepSize), info.Unit);
        }
        return result;
    }

    ComputeResult ComputeGeographic(const std::shared_ptr<FixedBody>& body, ComputeInfo info) noexcept {
        ComputeResult result{};
        result.Altitudes.resize(info.Steps);
        result.Azimuths.resize(info.Steps);
        for (std::size_t step = 0; step < info.Steps; ++step) {
            const auto position = ObserveGeographic(body->GetEquatorialPosition(info.Date), info.Observer, info.Date);
            result.Altitudes[step] = position.Altitude;
            result.Azimuths[step] = position.Azimuth;
            info.Date.Add(static_cast<std::int64_t>(info.StepSize), info.Unit);
        }
        return result;
    }
}// namespace ephemeris