#include "TrackableBodyView.hpp"

namespace StarTracker {

	TrackableBodyView::TrackableBodyView(void* nativeWindowHandle) noexcept : Core::View{ nativeWindowHandle } {
	
		observer = {};
		celestialBodies = {};
	}

	void TrackableBodyView::OnInit() noexcept {

		celestialBodies = Ephemeris::CelestialBody::LoadFromFile("assets/ephemeris/CelestialBodies.json");
		observer = [&]() -> Ephemeris::Coordinates::Observer {

			try {

				const auto geoLocation = Utils::LocationService::GeoLocation::Get();

				Ephemeris::Coordinates::Observer observer{};
				observer.Latitude = geoLocation.Latitude;
				observer.Longitude = geoLocation.Longitude;
				return observer;
			}
			catch (const std::exception&) {

				return {};
			}
		}();
	}

	void TrackableBodyView::OnUpdate(float deltaTime) noexcept {

		if (ImGui::Begin("Trackable Bodies")) {

			ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[Core::UIFont::Medium]);
			ImGui::Text("%s", DateTime::Now().ToString().c_str());
			ImGui::PopFont();
			ImGui::Separator();
			
			for (const auto& body : celestialBodies) {

				const auto position = Ephemeris::Coordinates::Transform::TerrestrialObserverToHorizontal(
					body->GetSphericalPosition(DateTime::Now()),
					observer,
					DateTime::Now()
				);
				
				const auto data = std::format("Name: {}, Designation: {}, Azimuth: {}, Elevation: {}", 
					body->GetName(),
					body->GetDesignation(),
					position.Azimuth,
					position.Altitude
				);

				const auto windowId = std::format("Begin Tracking {} ({})", body->GetName(), body->GetDesignation());

				bool selected = false;
				if (ImGui::Selectable(data.c_str(), &selected)) {

					ImGui::OpenPopup(windowId.c_str());					
				}

				bool popUpOpen = true;
				if (ImGui::BeginPopupModal(windowId.c_str(), &popUpOpen)) {

					ImGui::Text("Tracking %s", body->GetName().c_str());
					ImGui::EndPopup();
				}
				ImGui::Separator();
			}
		}
		
		ImGui::End();
	}

	void TrackableBodyView::OnDestroy() noexcept {


	}
}