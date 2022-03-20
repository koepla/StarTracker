#include "TrackableBodyView.hpp"

namespace StarTracker {

	TrackableBodyView::TrackableBodyView(void* nativeWindowHandle) noexcept : Core::View{ nativeWindowHandle }, observer{}, celestialBodies{}, tracker{} {	
		
	}

	void TrackableBodyView::OnInit() noexcept {

		celestialBodies = Ephemeris::CelestialBody::LoadFromFile("Assets/Ephemeris/CelestialBodies.json");
		observer = [&]() -> Utils::LocationService::Location {

			try {

				return Utils::LocationService::GeoLocation::Get();
			}
			catch (const std::exception&) {

				return {};
			}
		}();

		try {

			//while (!tracker.Connect());
		}
		catch (const std::exception&) {

			ASSERT(false && "Failed to connect to tracker!");
		}
	}

	void TrackableBodyView::OnUpdate(float deltaTime) noexcept {

        const auto application = Core::Application::GetInstance();
        const auto reloadCelestialBodies = [&]() -> void {

            const auto filePath = Utils::File::OpenFileDialog("Select CelestialBodes File", false);

            if (!filePath.empty()) {

                celestialBodies = Ephemeris::CelestialBody::LoadFromFile(filePath.at(0));
            }
        };

        if (Core::Input::IsKeyPressed(Core::KeyCode::LeftControl) && Core::Input::IsKeyPressed(Core::KeyCode::L)) {

            reloadCelestialBodies();
        }

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Load CelestialBodies", "CTRL+L")) {

                    reloadCelestialBodies();
                }
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

		if (ImGui::Begin("Tracking")) {

			const auto dateTimeInfo = std::format("{}", DateTime::Now().ToString());
			const auto trackerInfo = std::format("Tracker: {}", tracker.IsConnected() ? "Connected" : "Not Connected");
			const auto locationInfo = std::format("Location: {}, {}, {}", observer.City, observer.RegionName, observer.Country);
			const auto frameInfo = std::format("FrameTime: {} ms", deltaTime);

			ImGui::PushFont(Core::UIFont::Medium);
			ImGui::Text("%s", std::format("{} - {} - {} - [{}]", dateTimeInfo, trackerInfo, locationInfo, frameInfo).c_str());
			ImGui::PopFont();
			ImGui::Separator();
			
			static double trackingDuration{ 5000 };
			const auto contentRegion = ImGui::GetContentRegionAvail();
			ImGui::PushItemWidth(contentRegion.x / 2);
			ImGui::InputDouble("Tracking duration [ms]", &trackingDuration);
			ImGui::PopItemWidth();
			ImGui::Separator();
			
			for (const auto& body : celestialBodies) {

				const auto sphericalPosition = body->GetSphericalPosition(DateTime::Now());
				const auto positionPreview = Ephemeris::Coordinates::Transform::TerrestrialObserverToHorizontal(
					sphericalPosition,
					{ observer.Latitude, observer.Longitude },
					DateTime::Now()
				);

				const auto windowId = std::format("Tracking {} ({})", body->GetName(), body->GetDesignation());
                const auto availableSize = ImGui::GetContentRegionAvail().x;
                const auto usedSize = availableSize / 4.0f;

				bool selected = false;
				if (ImGui::Selectable(std::format("Name: {}", body->GetName()).c_str(), &selected)) {

					ImGui::OpenPopup(windowId.c_str());					
				}
                ImGui::SameLine(1.0f * usedSize);
                ImGui::Text("Designation: %s", body->GetDesignation().c_str());
                ImGui::SameLine(2.0f * usedSize);
                ImGui::Text("Azimuth: %6.2lf", positionPreview.Azimuth);
                ImGui::SameLine(3.0f * usedSize);
                ImGui::Text("Elevation: %6.2lf", positionPreview.Altitude);


				bool popUpOpen = true;
				if (ImGui::BeginPopupModal(windowId.c_str(), &popUpOpen, ImGuiWindowFlags_AlwaysAutoResize)) {

					const auto size = ImGui::CalcTextSize(windowId.c_str());

					ImGui::Text("%s", windowId.c_str());
					if (ImGui::Button("Track", { size.x, 1.4f * size.y })) {

						const auto trackerCallback = [&](Core::TrackerCallbackStatus status) -> void {

							if (status == Core::TrackerCallbackStatus::FAILURE) {

								std::fprintf(stderr, "Failure on tracking!\n");
							}
							else {

								std::fprintf(stdout, "Tracking finished! (%lf ms)\n", trackingDuration);
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