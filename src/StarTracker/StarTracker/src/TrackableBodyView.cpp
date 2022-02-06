#include "TrackableBodyView.hpp"

namespace StarTracker {

	TrackableBodyView::TrackableBodyView(void* nativeWindowHandle) noexcept : Core::View{ nativeWindowHandle }, observer{}, celestialBodies{}, tracker{} {	
		
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

		try {

			while(!tracker.Connect());
		}
		catch (const std::exception&) {

			ASSERT(false && "Failed to connect to tracker!");
		}
	}

	void TrackableBodyView::OnUpdate(float deltaTime) noexcept {

		if (ImGui::Begin("Trackable Bodies")) {

			ImGui::PushFont(Core::UIFont::Medium);
			ImGui::Text("%s [%f fps, Tracker: %s]", DateTime::Now().ToString().c_str(), 1.0f / deltaTime, tracker.IsConnected() ? "Connected" : "Not Connected");
			ImGui::PopFont();
			ImGui::Separator();
			
			static int trackingDuration{ 5000 };
			const auto contentRegion = ImGui::GetContentRegionAvail();
			ImGui::PushItemWidth(contentRegion.x / 2);
			ImGui::InputInt("Tracking duration [ms]", &trackingDuration);
			ImGui::PopItemWidth();
			ImGui::Separator();
			
			for (const auto& body : celestialBodies) {

				const auto sphericalPosition = body->GetSphericalPosition(DateTime::Now());
				const auto positionPreview = Ephemeris::Coordinates::Transform::TerrestrialObserverToHorizontal(
					sphericalPosition,
					observer,
					DateTime::Now()
				);
				
				const auto bodyData = std::format("Name: {}, Designation: {}, Azimuth: {}, Elevation: {}", 
					body->GetName(),
					body->GetDesignation(),
					positionPreview.Azimuth,
					positionPreview.Altitude
				);

				const auto windowId = std::format("Tracking {} ({})", body->GetName(), body->GetDesignation());

				bool selected = false;
				if (ImGui::Selectable(bodyData.c_str(), &selected)) {

					ImGui::OpenPopup(windowId.c_str());					
				}

				bool popUpOpen = true;
				if (ImGui::BeginPopupModal(windowId.c_str(), &popUpOpen, ImGuiWindowFlags_AlwaysAutoResize)) {

					const auto size = ImGui::CalcTextSize(windowId.c_str());

					ImGui::Text(windowId.c_str());
					if (ImGui::Button("Track", ImVec2{ size.x, 1.4f * size.y })) {

						const auto trackerCallback = [&](Core::TrackerCallbackStatus status) -> void {

							if (status == Core::TrackerCallbackStatus::FAILURE) {

								std::fprintf(stderr, "Failure on tracking!\n");
							}
							else {

								std::fprintf(stdout, "Tracking finished! (%d ms)\n", trackingDuration);
							}
						};

						if (tracker.Track(sphericalPosition, observer, trackingDuration, trackerCallback)) {

							std::fprintf(stdout, "Started tracking!\n");
						}
					}
					ImGui::EndPopup();
				}
				ImGui::Separator();
			}
		}
		
		ImGui::End();
	}

	void TrackableBodyView::OnDestroy() noexcept {

		try {

			ASSERT(tracker.Disconnect() && "Failed to disconnect from tracker!");
		}
		catch (const std::exception&) {

			ASSERT(false && "Failed to disconnect from tracker!");
		}
	}
}