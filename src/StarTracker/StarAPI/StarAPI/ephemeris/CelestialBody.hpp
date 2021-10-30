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
		
		/**
		* Retrieves the name
		*
		* @return the name of the celestial body
		*
		*/
		[[nodiscard]] const std::string& GetName() const noexcept;
		
		/**
		* Computes the spherical position of the celestial body
		*
		* @param date date and time for the computation
		*
		* @param eps tolerance for kepler's equation
		*
		* @return the computed spherical coordinates
		*
		*/
		[[nodiscard]] Coordinates::Spherical GetSphericalPosition(const DateTime& date, double eps = 1e-12) const noexcept;

	public:
		/**
		* Retrieves a list of celestial bodies from a .json file
		*
		* @param filePath path to .json file
		*
		* @return list of the retrieved celestial bodies as std::vector
		*
		* @throws std::exception if the file doesn't exist or if the content is invalid formatted
		* 
		*/
		[[nodiscard]] static std::vector<CelestialBody> LoadFromFile(const std::filesystem::path& filePath) noexcept(false);

	private:
		[[nodiscard]] double computeEccentricAnomaly(double meanAnomaly, double eccentricity, double eps = 1e-12) const noexcept;
		[[nodiscard]] std::pair<double, double> computeTrueAnomalyAndDistance(double semiMajorAxis, double eccentricAnomaly, double eccentricity) const noexcept;
		[[nodiscard]] double computeEcliptic(double julianCenturies) const noexcept;
		[[nodiscard]] Coordinates::Rectangular computeEarthPos(double julianCenturies) const noexcept;
	};
}

#endif // STARAPI_EPHEMERIS_CELESTIALBODY_H