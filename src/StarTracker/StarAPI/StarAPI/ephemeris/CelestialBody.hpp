#ifndef _STAR_CELESTIALBODY_H_
#define _STAR_CELESTIALBODY_H_

#include "KeplarianElements.hpp"
#include "../coordinates/Spherical.hpp"
#include "../coordinates/Rectangular.hpp"
#include "../DateTime.hpp"
#include <vector>
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>

namespace Star::Ephemeris {

	class CelestialBody
	{
	private:
		std::string name;
		KeplarianElements keplerElements;
		KeplarianElements keplerElementsCentury;

	public:
		CelestialBody(const KeplarianElements& keplerElements, const KeplarianElements& keplerElementsCentury);
		CelestialBody(const std::string& name, const KeplarianElements& keplerElements, const KeplarianElements& keplerElementsCentury);
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

#endif // _STAR_CELESTIALBODY_H_