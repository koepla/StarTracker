#ifndef STARTRACKER_CORE_CELESTIALBODYLIBRARY_H
#define STARTRACKER_CORE_CELESTIALBODYLIBRARY_H

#include <StarEngine/Ephemeris/CelestialBody.hpp>
#include <StarTracker/Core/Core.hpp>
#include <StarTracker/Core/OpenGL/Texture.hpp>

#include <memory>

namespace StarTracker::Core {

	struct BodyLibraryEntry {

		std::shared_ptr<Ephemeris::CelestialBody> Body;
		std::shared_ptr<OpenGL::Texture> Texture;
	};

	class BodyLibrary {

	private:
		std::vector<BodyLibraryEntry> library;

	public:
		BodyLibrary() noexcept;
		void LoadFromFile(const std::filesystem::path& path) noexcept;

		/**
		* @brief Returns the complete list of all stored Celestial Bodies
		* 
		* @return Complete list of all stored Celestial Bodies
		*/
		[[nodiscard]] std::vector<BodyLibraryEntry> GetBodies() const noexcept;

		/**
		* @brief Returns a filtered list of all stored Celestial Bodies
		* 
		* @param filter search-value that follows some filter conventions
		* 
		* @return Filtered list of Celestial Bodies
		*/
		[[nodiscard]] std::vector<BodyLibraryEntry> GetFilteredBodies(std::string filter) const noexcept;

		/**
		* @brief Returns a JSON-formatted string of all stored Celestial Bodies
		* 
		* @return JSON-formatted BodyLibrary
		* 
		*/
		[[nodiscard]] std::string GetSerializable() const noexcept;

		/**
		* @brief Adds a new Entry to the library
		* 
		* @param entry Instance of BodyLibraryEntry, if the texture is not set a default-texture is used
		* 
		*/
		void AddEntry(BodyLibraryEntry entry) noexcept;
	};
}

#endif // STARTRACKER_CORE_CELESTIALBODYLIBRARY_H