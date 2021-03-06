#include "CelestialBody.hpp"
#include "FixedBody.hpp"
#include "SolarSystemBody.hpp"

namespace StarTracker::Ephemeris {

    CelestialBody::CelestialBody(std::string_view name, std::string_view designation, std::string_view textureHandle) noexcept : name{ name }, designation{ designation }, textureHandle{ textureHandle } {

    }

    CelestialBody::~CelestialBody() noexcept {

    }

    std::string_view CelestialBody::GetName() const noexcept {

        return name;
    }

    std::string_view CelestialBody::GetDesignation() const noexcept {

        return designation;
    }

    std::string_view CelestialBody::GetTextureHandle() const noexcept {

        return textureHandle;
    }

    std::vector<std::shared_ptr<CelestialBody>> CelestialBody::LoadFromFile(const std::filesystem::path& filePath)  noexcept {

        std::ifstream fin{ filePath };

        if (!fin.is_open()) {

            return {};
        }

        const auto fileContent = std::string{ std::istreambuf_iterator<char>{ fin }, std::istreambuf_iterator<char>{ } };

        if (fileContent.empty()) {

            return {};
        }

        const auto jObject = nlohmann::json::parse(fileContent, nullptr, false, true);

        if (!jObject.contains("CelestialBodies") || !jObject["CelestialBodies"].is_array()) {

            return {};
        }

        std::vector<std::shared_ptr<CelestialBody>> celestialBodies{};

        for (const auto& element : jObject["CelestialBodies"]) {

            if (!isValidCelestialBody(element)) {

                continue;
            }

            const auto type = element["Type"].get<std::string>();
            const auto textureHandle = element["TextureHandle"].get<std::string>();

            if (type.compare("SSB") == 0) {

                KeplerianElements orbitalElements{};
                orbitalElements.SemiMajorAxis = element["SemiMajorAxis"].get<double>();
                orbitalElements.Eccentricity = element["Eccentricity"].get<double>();
                orbitalElements.Inclination = element["Inclination"].get<double>();
                orbitalElements.MeanLongitude = element["MeanLongitude"].get<double>();
                orbitalElements.LonPerihelion = element["LonPerihelion"].get<double>();
                orbitalElements.LonAscendingNode = element["LonAscendingNode"].get<double>();

                KeplerianElements orbitalRateElements{};
                orbitalRateElements.SemiMajorAxis = element["SemiMajorAxisCentury"].get<double>();
                orbitalRateElements.Eccentricity = element["EccentricityCentury"].get<double>();
                orbitalRateElements.Inclination = element["InclinationCentury"].get<double>();
                orbitalRateElements.MeanLongitude = element["MeanLongitudeCentury"].get<double>();
                orbitalRateElements.LonPerihelion = element["LonPerihelionCentury"].get<double>();
                orbitalRateElements.LonAscendingNode = element["LonAscendingNodeCentury"].get<double>();

                auto solarSystemBody = std::make_shared<SolarSystemBody>(element["Name"].get<std::string>(), textureHandle, orbitalElements, orbitalRateElements);
                celestialBodies.emplace_back(solarSystemBody);
            }
            else if (type.compare("FB") == 0) {

                Coordinates::Spherical sphericalCoordinates{};
                sphericalCoordinates.RightAscension = element["RightAscension"].get<double>();
                sphericalCoordinates.Declination = element["Declination"].get<double>();
                sphericalCoordinates.Radius = element["Radius"].get<double>();

                auto fixedBody = std::make_shared<FixedBody>(element["Name"].get<std::string>(), element["Designation"].get<std::string>(), textureHandle, sphericalCoordinates);
                celestialBodies.emplace_back(fixedBody);
            }
        }

        return celestialBodies;
    }

    bool CelestialBody::isValidCelestialBody(const nlohmann::json& entry) noexcept {

        if (!entry.contains("Name") || !entry.contains("Type") || !entry.contains("TextureHandle")) {

            return false;
        }

        const auto type = entry["Type"].get<std::string>();

        // SolarSystemBody
        if (type.compare("SSB") == 0) {

            return
                entry.contains("SemiMajorAxis") &&
                entry.contains("SemiMajorAxisCentury") &&
                entry.contains("Eccentricity") &&
                entry.contains("EccentricityCentury") &&
                entry.contains("Inclination") &&
                entry.contains("InclinationCentury") &&
                entry.contains("MeanLongitude") &&
                entry.contains("MeanLongitudeCentury") &&
                entry.contains("LonPerihelion") &&
                entry.contains("LonPerihelionCentury") &&
                entry.contains("LonAscendingNode") &&
                entry.contains("LonAscendingNodeCentury");
        }
        // FixedBody
        else if (type.compare("FB") == 0) {

            return
                entry.contains("Designation") &&
                entry.contains("RightAscension") &&
                entry.contains("Declination") &&
                entry.contains("Radius");
        }
        else {

            return false;
        }
    }
}