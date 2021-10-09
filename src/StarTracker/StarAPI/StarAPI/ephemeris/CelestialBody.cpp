#include "CelestialBody.hpp"

namespace StarTracker::Ephemeris {

    CelestialBody::CelestialBody(const KeplerianElements& keplerElements, const KeplerianElements& keplerElementsCentury) : CelestialBody("Unnamed Celestial Body", keplerElements, keplerElementsCentury)
    {

    }

    CelestialBody::CelestialBody(const std::string& name, const KeplerianElements& keplerElements, const KeplerianElements& keplerElementsCentury) : name(name), keplerElements(keplerElements), keplerElementsCentury(keplerElementsCentury)
    {

    }

    const std::string& CelestialBody::GetName() const noexcept
    {
        return name;
    }

    StarTracker::Ephemeris::Coordinates::Spherical CelestialBody::GetSphericalPosition(const DateTime& date, double eps) const noexcept
    {
        double t = DateTime::JulianCenturies(date);

        KeplerianElements meanKeplerElem = {

            keplerElements.SemiMajorAxis + keplerElementsCentury.SemiMajorAxis * t,
            keplerElements.Eccentricity + keplerElementsCentury.Eccentricity * t,
            keplerElements.Inclination + keplerElementsCentury.Inclination * t,
            keplerElements.MeanLongitude + keplerElementsCentury.MeanLongitude * t,
            keplerElements.LonPerihelion + keplerElementsCentury.LonPerihelion * t,
            keplerElements.LonAscendingNode + keplerElementsCentury.LonAscendingNode * t
        };

        const double& w = meanKeplerElem.LonPerihelion;
        const double& Om = meanKeplerElem.LonAscendingNode;
        const double& I = meanKeplerElem.Inclination;

        // compute argument of perihelion and mean anomaly

        double perihelion = w - Om;
        double meanAnomaly = Math::Mod(meanKeplerElem.MeanLongitude - w, 360.0);

        // compute eccentric anomaly

        double eccentricAnomaly = computeEccentricAnomaly(meanAnomaly, meanKeplerElem.Eccentricity, eps);


        // compute true Anomaly and distance of the Planet

        auto [trueAnomaly, distance] = computeTrueAnomalyAndDistance(meanKeplerElem.SemiMajorAxis, eccentricAnomaly, meanKeplerElem.Eccentricity);

        // compute Planets heliocentric position in 3D-space

        double xh = distance * (Math::Cosine(Om) * Math::Cosine(trueAnomaly + w - Om) - Math::Sine(Om) * Math::Sine(trueAnomaly + w - Om) * Math::Cosine(I));
        double yh = distance * (Math::Sine(Om) * Math::Cosine(trueAnomaly + w - Om) + Math::Cosine(Om) * Math::Sine(trueAnomaly + w - Om) * Math::Cosine(I));
        double zh = distance * (Math::Sine(trueAnomaly + w - Om) * Math::Sine(I));
        
        // compute Planets rectangular ecliptic geocentric position in 3D-space

        Coordinates::Rectangular earthCoords = computeEarthPos(t);

        double xg = xh - earthCoords.X;
        double yg = yh - earthCoords.Y;
        double zg = zh - earthCoords.Z;

        // transform to rectangular equatorial coordinates

        double ecl = computeEcliptic(t);
    
        double xe = xg;
        double ye = yg * Math::Cosine(ecl) - zg * Math::Sine(ecl);
        double ze = yg * Math::Sine(ecl) + zg * Math::Cosine(ecl);

        Coordinates::Spherical sphericalCoords{};
        sphericalCoords.RightAscension = Math::ArcTangent2(ye, xe);
        sphericalCoords.Declination = Math::ArcTangent2(ze, std::sqrt(xe * xe + ye * ye));
        sphericalCoords.Radius = std::sqrt(xe * xe + ye * ye + ze * ze);

        return sphericalCoords;
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
            });
        }

        return celestialBodies;
    }

    // Correct for drifting ecliptic due to planets pull on the Earth

    double CelestialBody::computeEccentricAnomaly(double meanAnomaly, double eccentricity, double eps) const noexcept
    {
        double edeg = Math::Degrees(eccentricity);
        double erad = eccentricity;

        double eccentricAnomaly = meanAnomaly + edeg * Math::Sine(meanAnomaly);
        double de;

        do {

            double dm = meanAnomaly - (eccentricAnomaly - edeg * Math::Sine(eccentricAnomaly));
            de = dm / (1 - erad * Math::Cosine(eccentricAnomaly));
            eccentricAnomaly += de;

        } while (Math::Abs(de) > eps);

        return eccentricAnomaly;
    }

    std::pair<double, double> CelestialBody::computeTrueAnomalyAndDistance(double semiMajorAxis, double eccentricAnomaly, double eccentricity) const noexcept
    {
        double xv = semiMajorAxis * (Math::Cosine(eccentricAnomaly) - eccentricity);
        double yv = semiMajorAxis * std::sqrt(1 - std::pow(eccentricity, 2)) * Math::Sine(eccentricAnomaly);

        double trueAnomaly = Math::ArcTangent2(yv, xv);
        double distance = std::sqrt(xv * xv + yv * yv);

        return std::make_pair(Math::Mod(trueAnomaly, 360.0), distance);
    }

    double CelestialBody::computeEcliptic(double julianCenturies) const noexcept
    {
        return (23.43929111 - (46.8150 + (0.00059 - 0.001813 * julianCenturies) * julianCenturies) * julianCenturies / 3600.0);
    }

    // Returns radius and longitude of the Sun, needed for heliocentric to geocentric transform

    Coordinates::Rectangular CelestialBody::computeEarthPos(double julianCenturies) const noexcept
    {
        // The EM-Bary keplarian elements are hardcoded because they are needed for every computation

        KeplerianElements meanEarth{};
        meanEarth.SemiMajorAxis = 1.00000261 + 0.00000562 * julianCenturies;
        meanEarth.Eccentricity = 0.01671022 - 0.00003804 * julianCenturies;
        meanEarth.Inclination = 0.00005 - 0.01294668 * julianCenturies;
        meanEarth.MeanLongitude = 100.46457166 + 35999.37244981 * julianCenturies;
        meanEarth.LonPerihelion = 102.93768193 + 0.32327364 * julianCenturies;
        meanEarth.LonAscendingNode = -11.26064 - (18228.25 / 3600.0) * julianCenturies;

        const double& w = meanEarth.LonPerihelion;
        const double& Om = meanEarth.LonAscendingNode;
        const double& I = meanEarth.Inclination;

        double perihelion = w - Om;
        double meanAnomaly = Math::Mod(meanEarth.MeanLongitude - w, 360.0);

        // compute eccentric anomaly

        double eccentricAnomaly = computeEccentricAnomaly(meanAnomaly, meanEarth.Eccentricity);

        // compute true Anomaly and distance

        auto [trueAnomaly, distance] = computeTrueAnomalyAndDistance(meanEarth.SemiMajorAxis, eccentricAnomaly, meanEarth.Eccentricity);

        double lonEarth = trueAnomaly + w;

        Coordinates::Rectangular earthCoords{};
        earthCoords.X = distance * Math::Cosine(lonEarth);
        earthCoords.Y = distance * Math::Sine(lonEarth);
        earthCoords.Z = 0;

        return earthCoords;
    }
}