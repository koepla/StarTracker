#ifndef STARTRACKER_CORE_CELESTIALBODYLIBRARY_H
#define STARTRACKER_CORE_CELESTIALBODYLIBRARY_H

#include <StarEngine/Ephemeris/CelestialBody.hpp>
#include <StarTracker/Core/Core.hpp>
#include <StarTracker/Core/AssetDataBase.hpp>
#include <StarTracker/Core/OpenGL/Texture.hpp>

#include <memory>

namespace StarTracker::Core {

	struct LibraryEntry {

		std::shared_ptr<Ephemeris::CelestialBody> Body;
		std::shared_ptr<OpenGL::Texture> Texture;
	};

	class CelestialBodyLibrary {

	private:
		std::vector<LibraryEntry> library;

	public:
		CelestialBodyLibrary() noexcept;
		void LoadFromFile(const std::filesystem::path& path) noexcept;

		/**
		* @brief Returns the complete list of all stored Celestial Bodies
		* 
		* @return Complete list of all stored Celestial Bodies
		*/
		[[nodiscard]] std::vector<LibraryEntry> GetBodies() const noexcept;

		/**
		* @brief Returns a filtered list of all stored Celestial Bodies
		* 
		* @param filter search-value that follows some filter conventions
		* 
		* @return Filtered list of Celestial Bodies
		*/
		[[nodiscard]] std::vector<LibraryEntry> GetFilteredBodies(std::string filter) const noexcept;
	};
}

#endif // STARTRACKER_CORE_CELESTIALBODYLIBRARY_H