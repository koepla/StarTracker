#include "TrackableBodyView.hpp"

namespace StarTracker {

	TrackableBodyView::TrackableBodyView(void* nativeWindowHandle) noexcept : Core::View{ nativeWindowHandle }, observer{}, celestialBodies{}, tracker{} {	
		
	}

	void TrackableBodyView::OnInit() noexcept {

		celestialBodies = Ephemeris::CelestialBody::LoadFromFile("assets/ephemeris/CelestialBodies.json");
		observer = [&]() -> Utils::LocationService::Location {

			try {

				return Utils::LocationService::GeoLocation::Get();
			}
			catch (const std::exception&) {

				return {};
			}
		}();

		try {

			while (!tracker.Connect());
		}
		catch (const std::exception&) {

			ASSERT(false && "Failed to connect to tracker!");
		}
	}

	void TrackableBodyView::OnUpdate(float deltaTime) noexcept {

		if (ImGui::Begin("Trackable Bodies")) {

			const auto dateTimeInfo = std::format("{}", DateTime::Now().ToString());
			const auto trackerInfo = std::format("Tracker: {}", tracker.IsConnected() ? "Connected" : "Not Connected");
			const auto locationInfo = std::format("Location: {}, {}, {}", observer.City, observer.RegionName, observer.Country);
			const auto frameInfo = std::format("Frametime: {} ms", deltaTime);

			ImGui::PushFont(Core::UIFont::Medium);
			ImGui::Text(std::format("{} - {} - {} - [{}]", dateTimeInfo, trackerInfo, locationInfo, frameInfo).c_str());
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
					{ observer.Latitude, observer.Longitude },
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

						if (tracker.Track(body, { observer.Latitude, observer.Longitude }, trackingDuration, trackerCallback)) {

							std::fprintf(stdout, "Started tracking!\n");
						}
						else {

							std::fprintf(stderr, "Couldn't start tracking!\n");
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