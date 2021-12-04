#include "CelestialBody.hpp"
#include "FixedBody.hpp"
#include "SolarSystemBody.hpp"

namespace StarTracker::Ephemeris {
    
    CelestialBody::CelestialBody(const std::string& name, const std::string& designation) noexcept : name{ name }, designation{ designation } {
    
    }

    const std::string& CelestialBody::GetName() const noexcept {

        return name;
	}

    const std::string& CelestialBody::GetDesignation() const noexcept {

        return designation;
    }

    std::vector<std::shared_ptr<CelestialBody>> CelestialBody::LoadFromFile(const std::filesystem::path& filePath)  noexcept(false) {

        std::ifstream fin(filePath);

        if (!fin.good()) {

            throw std::exception("Couldn't open file!");
        }

        std::string fileContent = std::string(std::istreambuf_iterator<char>(fin), std::istreambuf_iterator<char>());

        if (fileContent.empty()) {

            throw std::exception("File was empty!");
        }

        nlohmann::json jObject = nlohmann::json::parse(fileContent, nullptr, false, true);

        if (!jObject.contains("CelestialBodies") || !jObject["CelestialBodies"].is_array()) {

            throw std::exception("Couldn't find CelestialBodies Array!");
        }

        std::vector<std::shared_ptr<CelestialBody>> celestialBodies{};

        for (auto& element : jObject["CelestialBodies"]) {

            auto type = element["Type"].get<std::string>();

            if (type._Equal("SSB")) {

                celestialBodies.push_back(std::make_shared<SolarSystemBody>(SolarSystemBody{

                    element["Name"].get<std::string>(),
                    KeplerianElements {

                        element["SemiMajorAxis"].get<double>(),
                        element["Eccentricity"].get<double>(),
                        element["Inclination"].get<double>(),
                        element["MeanLongitude"].get<double>(),
                        element["LonPerihelion"].get<double>(),
                        element["LonAscendingNode"].get<double>()
                    },
                    KeplerianElements {

                        element["SemiMajorAxisCentury"].get<double>(),
                        element["EccentricityCentury"].get<double>(),
                        element["InclinationCentury"].get<double>(),
                        element["MeanLongitudeCentury"].get<double>(),
                        element["LonPerihelionCentury"].get<double>(),
                        element["LonAscendingNodeCentury"].get<double>()
                    }
                }));
            }
            else if (type._Equal("CB")) {

                celestialBodies.push_back(std::make_shared<FixedBody>(FixedBody{

                    element["Name"].get<std::string>(),
                    element["Designation"].get<std::string>(),
                    Coordinates::Spherical {

                        element["RightAscension"].get<double>(),
                        element["Declination"].get<double>(),
                        element["Radius"].get<double>()
                    }
                }));
            }
        }

        return celestialBodies;
    }
}