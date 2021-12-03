#ifndef STARAPI_EPHEMERIS_CELESTIALBODY_H
#define STARAPI_EPHEMERIS_CELESTIALBODY_H

#include "coordinates/Spherical.hpp"
#include "coordinates/Rectangular.hpp"
#include "../DateTime.hpp"

#include <vector>
#include <filesystem>
#include <fstream>

#include <nlohmann/json.hpp>

namespace StarTracker::Ephemeris {

	class CelestialBody {

	protected:
		std::string name;
		std::string designation;

		CelestialBody(const std::string& name, const std::string& designation) noexcept;

	public:
		/**
		* Computes the spherical position of the celestial body
		*
		* @param date date and time for the computation
		*
		* @return the computed spherical coordinates
		*
		*/
		[[nodiscard]] virtual Coordinates::Spherical GetSphericalPosition(const DateTime& date) const noexcept = 0;

		/**
		* Retrieves the name
		*
		* @return the name of the celestial body
		*
		*/
		[[nodiscard]] const std::string& GetName() const noexcept;

		/**
		* Retrieves the designation
		*
		* @return the name of the celestial body
		*
		*/
		[[nodiscard]] const std::string& GetDesignation() const noexcept;

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
		[[nodiscard]] static std::vector<std::unique_ptr<CelestialBody>> LoadFromFile(const std::filesystem::path& filePath) noexcept(false);
	};
}


#endif // STARAPI_EPHEMERIS_CELESTIALBODY_H

