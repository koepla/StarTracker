#ifndef STARAPI_EPHEMERIS_CELESTIALBODY_H
#define STARAPI_EPHEMERIS_CELESTIALBODY_H

#include "KeplerianElements.hpp"
#include "coordinates/Spherical.hpp"
#include "coordinates/Rectangular.hpp"
#include "../DateTime.hpp"
#include <vector>
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>

namespace StarTracker::Ephemeris {

	class CelestialBody
	{
	private:
		std::string name;
		KeplerianElements keplerElements;
		KeplerianElements keplerElementsCentury;

	public:
		CelestialBody(const KeplerianElements& keplerElements, const KeplerianElements& keplerElementsCentury) noexcept;
		CelestialBody(const std::string& name, const KeplerianElements& keplerElements, const KeplerianElements& keplerElementsCentury) noexcept;
		[[nodiscard]] const std::string& GetName() const noexcept;
		[[nodiscard]] Coordinates::Spherical GetSphericalPosition(const DateTime& date, double eps = 1e-12) const noexcept;

	public:
		[[nodiscard]] static std::vector<CelestialBody> LoadFromFile(const std::filesystem::path& filePath) noexcept(false);

	private:
		[[nodiscard]] double computeEccentricAnomaly(double meanAnomaly, double eccentricity, double eps = 1e-12) const noexcept;
		[[nodiscard]] std::pair<double, double> computeTrueAnomalyAndDistance(double semiMajorAxis, double eccentricAnomaly, double eccentricity) const noexcept;
		[[nodiscard]] double computeEcliptic(double julianCenturies) const noexcept;
		[[nodiscard]] Coordinates::Rectangular computeEarthPos(double julianCenturies) const noexcept;
	};
}

#endif // STARAPI_EPHEMERIS_CELESTIALBODY_H