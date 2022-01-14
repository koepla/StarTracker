#include <StarAPI/StarAPI.hpp>

#include "core/Core.hpp"
#include "core/Assert.hpp"
#include "core/Application.hpp"
#include "utils/serial/Package.hpp"
#include "utils/serial/Serial.hpp"

#include "utils/GeoLocation.hpp"
#include "utils/HttpRequest.hpp"
#include "utils/Stopwatch.hpp"

#include <iostream>
#include <filesystem>

class TrackerView : public StarTracker::Core::View {

private:
	std::vector<std::shared_ptr<StarTracker::Ephemeris::CelestialBody>> celestialBodies;
	StarTracker::Ephemeris::Coordinates::Observer observer;

public:
	explicit TrackerView(void* nativeWindowHandle) noexcept : View{ nativeWindowHandle } {

		celestialBodies = {};
		observer = {};
	}

	virtual void OnInit() noexcept override {

		celestialBodies = StarTracker::Ephemeris::CelestialBody::LoadFromFile("assets/CelestialBodies.json");
		auto geoLocation = StarTracker::Utils::LocationService::GeoLocation::Get();
		observer = { geoLocation.Latitude, geoLocation.Longitude };
	}

	virtual void OnUpdate(float deltaTime) noexcept override {

		ImGui::Begin("Celestial Bodies");

		for (auto& body : celestialBodies) {

			auto position = StarTracker::Ephemeris::Coordinates::Transform::TerrestrialObserverToHorizontal(
				body->GetSphericalPosition(StarTracker::DateTime::Now()),
				observer,
				StarTracker::DateTime::Now()
			);

			ImGui::Text("Name: {%s} Designation: {%s} Azimuth: {%lf} Elevation: {%lf}",
				body->GetName().c_str(),
				body->GetDesignation().c_str(),
				position.Azimuth,
				position.Altitude
			);
		}

		ImGui::End();
	}

	virtual void OnDestroy() noexcept override {


	}
};

auto main(int, const char**) -> int {

	StarTracker::Core::ApplicationData applicationData{};
	applicationData.Title = "StarTracker";
	applicationData.Width = 1280;
	applicationData.Height = 720;
	applicationData.EnableDockspace = true;
	applicationData.Fullscreen = false;

	StarTracker::Core::Application application{ applicationData };

	TrackerView view{ application.GetWindow().GetNativeHandle() };

	application.AddToViewList(&view);
	application.Run();

	return 0;
}