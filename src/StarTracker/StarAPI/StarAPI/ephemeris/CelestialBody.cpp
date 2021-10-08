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

    Star::Coordinates::Spherical CelestialBody::GetSphericalPosition(const Date& date, double eps) const noexcept
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

        double w = currKeplerElem.LonPerihelion;
        double Om = currKeplerElem.LonAscendingNode;
        double I = currKeplerElem.Inclination;

        // compute argument of perihelion and mean anomaly

        double perihelion = w - Om;
        double meanAnomaly = Math::Mod(currKeplerElem.MeanLongitude - w, 360.0);

        // compute eccentric anomaly

        double edeg = Math::Degrees(currKeplerElem.Eccentricity);
        double erad = currKeplerElem.Eccentricity;

        double eccentricAnomaly = meanAnomaly + edeg * Math::Sine(meanAnomaly);
        double de;

        do {

            double dm = meanAnomaly - (eccentricAnomaly - edeg * Math::Sine(eccentricAnomaly));
            de = dm / (1 - erad * Math::Cosine(eccentricAnomaly));
            eccentricAnomaly += de;

        } while (Math::Abs(de) > eps);

        // The above part should work and checks with online sources, the code below doesn't work!

        // compute planets heliocentric coordinates in orbital plane (x axis aligned from the focus to perihelion)

        double x = currKeplerElem.SemiMajorAxis * (Math::Cosine(eccentricAnomaly) - currKeplerElem.Eccentricity);
        double y = currKeplerElem.SemiMajorAxis * std::sqrt(1 - std::pow(currKeplerElem.Eccentricity, 2)) * Math::Sine(eccentricAnomaly);
        double z = 0;

        double xecl = (Math::Cosine(w) * Math::Cosine(Om) - Math::Sine(w) * Math::Sine(Om) * Math::Cosine(I)) * x + (-Math::Sine(w) * Math::Cosine(Om) - Math::Cosine(w) * Math::Sine(Om) * Math::Cosine(I)) * y;
        double yecl = (Math::Cosine(w) * Math::Sine(Om) + Math::Sine(w) * Math::Cosine(Om) * Math::Cosine(I)) * x + (-Math::Sine(w) * Math::Sine(Om) + Math::Cosine(w) * Math::Cosine(Om) * Math::Cosine(I)) * y;
        double zecl = (Math::Sine(w) * Math::Sine(I)) * x + (Math::Cosine(w) * Math::Cosine(I)) * y;

        // compute equatorial coordinates in the ICRF frame

        double xeq = xecl;
        double yeq = Math::Cosine(23.43928) * yecl - Math::Sine(23.43928) * zecl;
        double zeq = Math::Sine(23.43928) * yecl + Math::Cosine(23.43928) * zecl;

        return Coordinates::Spherical{

            std::sqrt(xeq * xeq + yeq * yeq + zeq * zeq), // radius
            Math::ArcTangent2(yeq, xeq),// RA
            Math::ArcTangent(zeq / std::sqrt(xeq * xeq + yeq * yeq)) // DECL
        };
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


