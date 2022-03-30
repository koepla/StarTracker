#ifndef STARENGINE_EPHEMERIS_CELESTIALBODY_H
#define STARENGINE_EPHEMERIS_CELESTIALBODY_H

#include <StarEngine/DateTime.hpp>
#include <StarEngine/Ephemeris/Coordinates/Spherical.hpp>
#include <StarEngine/Ephemeris/Coordinates/Rectangular.hpp>

#include <vector>
#include <filesystem>
#include <fstream>
#include <stdexcept>

#pragma warning(disable:26819)
#include <nlohmann/json.hpp>
#pragma warning(default:26819)

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
		[[nodiscard]] std::string GetName() const noexcept;

		/**
		* Retrieves the designation
		*
		* @return the name of the celestial body
		*
		*/
		[[nodiscard]] std::string GetDesignation() const noexcept;

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
		[[nodiscard]] static std::vector<std::shared_ptr<CelestialBody>> LoadFromFile(const std::filesystem::path& filePath) noexcept(false);
	
	private:
		/**
		* Retrieves a list of celestial bodies from a .json file
		*
		* @param entry potential body entry
		*
		* @return true if the entry appears to be valid, false if the entry is malformed
		*
		*/
		[[nodiscard]] static bool isValidCelestialBody(const nlohmann::json& entry) noexcept;
	};
}


#endif // STARENGINE_EPHEMERIS_CELESTIALBODY_H

