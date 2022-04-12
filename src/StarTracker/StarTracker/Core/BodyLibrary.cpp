#include "BodyLibrary.hpp"

#include <StarTracker/Core/AssetDataBase.hpp>

namespace StarTracker::Core {

	BodyLibrary::BodyLibrary() noexcept : library{} {

	}

	void BodyLibrary::LoadFromFile(const std::filesystem::path& path) noexcept {

		// Load the basic Celestial Bodies
		const auto baseLibrary = Ephemeris::CelestialBody::LoadFromFile(path);

		for (const auto& body : baseLibrary) {

			// Grab the texture, this probably needs better error handling
			const auto textureHandle = body->GetTextureHandle();
			const auto texture = AssetDataBase::LoadTexture(textureHandle);

			BodyLibraryEntry entry{};
			entry.Body = body;
			entry.Texture = texture;

			library.emplace_back(entry);
		}
	}

	std::vector<BodyLibraryEntry> BodyLibrary::GetBodies() const noexcept {

		return library;
	}

	[[nodiscard]] static std::vector<std::string_view> split(std::string_view string, std::string_view delimiter) noexcept {

		std::size_t posStart = 0;
		std::size_t posEnd = 0;
		std::size_t delimiterLength = delimiter.length();

		std::string_view token{};
		std::vector<std::string_view> result{};

		while ((posEnd = string.find(delimiter, posStart)) != std::string_view::npos) {

			token = string.substr(posStart, posEnd - posStart);
			posStart = posEnd + delimiterLength;
			result.emplace_back(token);
		}

		result.emplace_back(string.substr(posStart));

		return result;
	}

	std::vector<BodyLibraryEntry> BodyLibrary::GetFilteredBodies(std::string filter) const noexcept {

		// If no filter is specified, return the whole library
		if (filter.empty()) {

			return library;
		}

		// Lambda for toLower transform
		const auto lower = [&](std::int32_t c) -> std::int32_t {

			return std::tolower(c);
		};

		// Check if there are multiple filters
		const auto filters = [&]() -> std::vector<std::string_view> {

			// Transform the filter to be lowercase
			std::transform(filter.begin(), filter.end(), filter.begin(), lower);

			// We accept both ',' and ';' as separation characters
			if (filter.contains(",")) {

				return split(filter, ",");
			}
			else if (filter.contains(";")) {

				return split(filter, ";");
			}
			else {

				return { filter };
			}
		}();

		// It is okay to make a copy, since we are only copying references anyway
		std::vector<BodyLibraryEntry> filteredLibrary{};

		for (const auto& entry : library) {

			std::string name{ entry.Body->GetName() };
			std::string designation{ entry.Body->GetDesignation() };

			// Transform name and designation to lower
			std::transform(name.begin(), name.end(), name.begin(), lower);
			std::transform(designation.begin(), designation.end(), designation.begin(), lower);

			for (const auto& currentFilter : filters) {

				// Skip empty filters
				if (currentFilter.empty()) {

					continue;
				}

				// As we don't want duplicates, the break statements are necessary
				if (name.contains(currentFilter)) {

					filteredLibrary.emplace_back(entry);
					break;
				}
				else if (designation.contains(currentFilter)) {

					filteredLibrary.emplace_back(entry);
					break;
				}
			}
		}

		return filteredLibrary;
	}

	std::string BodyLibrary::GetSerializable() const noexcept {

		nlohmann::json jObject{};
		nlohmann::json jBodyArray{};

		for (const auto& entry : library) {

			const auto serializableEntry = entry.Body->GetSerializable();
			nlohmann::json jEntry = nlohmann::json::parse(serializableEntry, nullptr, false);
			jBodyArray.emplace_back(jEntry);
		}

		jObject["CelestialBodies"] = jBodyArray;

		return jObject.dump(1, '\t');
	}

	void BodyLibrary::AddEntry(BodyLibraryEntry entry) noexcept {

		if (entry.Body != nullptr) {

			// Load default texture
			if (entry.Texture == nullptr) {

				entry.Texture = Core::AssetDataBase::LoadTexture("Default.png");
			}

			library.emplace_back(entry);
		}
	}
}