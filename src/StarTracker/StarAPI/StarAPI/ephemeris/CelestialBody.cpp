#include "CelestialBody.hpp"

namespace Star::Ephemeris {

    CelestialBody::CelestialBody(const KeplarianElements& keplerElements, const KeplarianElements& keplerElementsCentury) : CelestialBody("Unnamed Celestial Body", keplerElements, keplerElementsCentury)
    {

    }

    CelestialBody::CelestialBody(const std::string& name, const KeplarianElements& keplerElements, const KeplarianElements& keplerElementsCentury) : name(name), keplerElements(keplerElements), keplerElementsCentury(keplerElementsCentury)
    {

    }

    const std::string& CelestialBody::GetName() const noexcept
    {
        return name;
    }

    Star::Coordinates::Spherical CelestialBody::GetSphericalPosition(const Date& date) const noexcept
    {
        double t = Date::JulianCenturies(date);

        KeplarianElements currKeplerElem = {

            keplerElements.SemiMajorAxis + keplerElementsCentury.SemiMajorAxis * t,
            keplerElements.Eccentricity + keplerElementsCentury.Eccentricity * t,
            keplerElements.Inclination + keplerElementsCentury.Inclination * t,
            keplerElements.MeanLongitude + keplerElementsCentury.MeanLongitude * t,
            keplerElements.LonPerihelion + keplerElementsCentury.LonPerihelion * t,
            keplerElements.LonAscendingNode + keplerElementsCentury.LonAscendingNode * t
        };

        double perihelion = currKeplerElem.LonPerihelion - currKeplerElem.LonAscendingNode;
        double meanAnomaly = currKeplerElem.MeanLongitude - currKeplerElem.LonPerihelion;


        return Star::Coordinates::Spherical();
    }

    std::vector<CelestialBody> CelestialBody::LoadFromFile(const std::filesystem::path& filePath)  noexcept(false)
    {
        std::ifstream fin(filePath);

        if (!fin.good()) {

            throw std::exception("Couldn't open file!");
        }
        
        std::string fileContent = std::string(std::istreambuf_iterator<char>(fin), std::istreambuf_iterator<char>());

        if (fileContent.empty()) {

            throw std::exception("File was empty!");
        }

        nlohmann::json jObject = nlohmann::json::parse(fileContent);

        if (!jObject.contains("CelestialBodies") || !jObject["CelestialBodies"].is_array()) {

            throw std::exception("Couldn't find CelestialBodies Array!");
        }

        std::vector<CelestialBody> celestialBodies;

        for (auto& element : jObject["CelestialBodies"]) {

            celestialBodies.push_back(CelestialBody{

                element["Name"].get<std::string>(),
                KeplarianElements {

                    element["SemiMajorAxis"].get<double>(),
                    element["Eccentricity"].get<double>(),
                    element["Inclination"].get<double>(),
                    element["MeanLongitude"].get<double>(),
                    element["LonPerihelion"].get<double>(),
                    element["LonAscendingNode"].get<double>()
                },
                KeplarianElements {

                    element["SemiMajorAxisCentury"].get<double>(),
                    element["EccentricityCentury"].get<double>(),
                    element["InclinationCentury"].get<double>(),
                    element["MeanLongitudeCentury"].get<double>(),
                    element["LonPerihelionCentury"].get<double>(),
                    element["LonAscendingNodeCentury"].get<double>()
                }
            });
        }

        return celestialBodies;
    }
}


