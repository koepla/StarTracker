#include "CelestialBody.hpp"
#include "FixedBody.hpp"
#include "SolarSystemBody.hpp"

namespace StarTracker::Ephemeris {

	CelestialBody::CelestialBody(const std::string& name, const std::string& designation) noexcept : name{ name }, designation{ designation } {

	}

	std::string CelestialBody::GetName() const noexcept {

		return name;
	}

	std::string CelestialBody::GetDesignation() const noexcept {

		return designation.empty() ? std::string{ "No Designation" } : designation;
	}

	std::vector<std::shared_ptr<CelestialBody>> CelestialBody::LoadFromFile(const std::filesystem::path& filePath)  noexcept(false) {

		std::ifstream fin{ filePath };

		if (!fin.is_open()) {

			throw std::exception{ "Couldn't open file!" };
		}

		const auto fileContent = std::string{ std::istreambuf_iterator<char>{ fin }, std::istreambuf_iterator<char>{ } };

		if (fileContent.empty()) {

			throw std::exception{ "File was empty!" };
		}

		const auto jObject = nlohmann::json::parse(fileContent, nullptr, false, true);

		if (!jObject.contains("CelestialBodies") || !jObject["CelestialBodies"].is_array()) {

			throw std::exception{ "Couldn't find CelestialBodies Array!" };
		}

		std::vector<std::shared_ptr<CelestialBody>> celestialBodies{};

		for (const auto& element : jObject["CelestialBodies"]) {

			const auto type = element["Type"].get<std::string>();

			if (type._Equal("SSB")) {

				KeplerianElements orbitalElements{};
				orbitalElements.SemiMajorAxis = element["SemiMajorAxis"].get<double>();
				orbitalElements.Eccentricity = element["Eccentricity"].get<double>();
				orbitalElements.Inclination = element["Inclination"].get<double>();
				orbitalElements.MeanLongitude = element["MeanLongitude"].get<double>();
				orbitalElements.LonPerihelion = element["LonPerihelion"].get<double>();
				orbitalElements.LonAscendingNode = element["LonAscendingNode"].get<double>();

				KeplerianElements orbitalRateElements{};
				orbitalRateElements.SemiMajorAxis = element["SemiMajorAxisCentury"].get<double>();
				orbitalRateElements.Eccentricity = element["EccentricityCentury"].get<double>();
				orbitalRateElements.Inclination = element["InclinationCentury"].get<double>();
				orbitalRateElements.MeanLongitude = element["MeanLongitudeCentury"].get<double>();
				orbitalRateElements.LonPerihelion = element["LonPerihelionCentury"].get<double>();
				orbitalRateElements.LonAscendingNode = element["LonAscendingNodeCentury"].get<double>();

				celestialBodies.emplace_back(std::make_shared<SolarSystemBody>(element["Name"].get<std::string>(), orbitalElements, orbitalRateElements));
			}
			else if (type._Equal("FB")) {

				Coordinates::Spherical sphericalCoordinates{};
				sphericalCoordinates.RightAscension = element["RightAscension"].get<double>();
				sphericalCoordinates.Declination = element["Declination"].get<double>();
				sphericalCoordinates.Radius = element["Radius"].get<double>();

				celestialBodies.emplace_back(std::make_shared<FixedBody>(element["Name"].get<std::string>(), element["Designation"].get<std::string>(), sphericalCoordinates));
			}
		}

		return celestialBodies;
	}
}