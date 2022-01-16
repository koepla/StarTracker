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

class ComputationView : public StarTracker::Core::View {

private:
	std::vector<std::shared_ptr<StarTracker::Ephemeris::CelestialBody>> celestialBodies;
	StarTracker::Ephemeris::Coordinates::Observer observer;

public:
	explicit ComputationView(void* nativeWindowHandle) noexcept : View{ nativeWindowHandle }, celestialBodies{}, observer{} {

	}

	virtual void OnInit() noexcept override {

		const auto geoLocation = StarTracker::Utils::LocationService::GeoLocation::Get();
		observer = { geoLocation.Latitude, geoLocation.Longitude };
		celestialBodies = StarTracker::Ephemeris::CelestialBody::LoadFromFile("assets/ephemeris/CelestialBodies.json");
	}

	virtual void OnUpdate(float deltaTime) noexcept override {

		ImGui::Begin("Celestial Bodies");

		auto mediumFont = ImGui::GetIO().Fonts->Fonts[StarTracker::Core::UIFont::Medium];

		ImGui::PushFont(mediumFont);
		ImGui::Text("%s", StarTracker::DateTime::Now().ToString().c_str());
		ImGui::PopFont();

		ImGui::Separator();

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
			ImGui::Separator();
		}
		ImGui::End();
	}

	virtual void OnDestroy() noexcept override {


	}
};

auto main(int, char**) -> int {

	StarTracker::Core::ApplicationData applicationData{};
	applicationData.Title = "StarTracker";
	applicationData.Width = 1280;
	applicationData.Height = 720;
	applicationData.EnableDockspace = true;
	applicationData.Fullscreen = false;
	applicationData.VerticalSync = false;

	try {
		StarTracker::Core::Application application{ applicationData };

		const auto view = std::make_unique<ComputationView>(application.GetWindow().GetNativeHandle());

		application.AddToViewList(view.get());
		application.Run();
	}
	catch (const std::exception& e) {

		ASSERT(false && "Fatal exception!");
	}

	return 0;
}