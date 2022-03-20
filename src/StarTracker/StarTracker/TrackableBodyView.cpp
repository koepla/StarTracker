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

			while (!tracker.Connect());
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

            if (ImGui::BeginTable("Trackable Bodies", 1)) {

                for(const auto& body : celestialBodies) {

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);

                    auto& style = ImGui::GetStyle();
                    const auto itemSpacing = style.ItemSpacing;
                    const auto fontSize = ImGui::GetFontSize();
                    const auto windowId = std::format("Tracking {} ({})", body->GetName(), body->GetDesignation());
                    const auto cardHeight = 4.0f * fontSize + 3.8f * itemSpacing.y - 6.0f;

                    static std::string trackingStatus{ "Not tracking" };

                    if (drawCelestialBodyCard(body,
                                          { observer.Latitude, observer.Longitude },
                                          Core::AssetDataBase::LoadTexture("pillarsOfCreation.jpg"),
                                          { ImGui::GetContentRegionAvail().x, cardHeight })) {

                        trackingStatus = "Not tracking";
                        ImGui::OpenPopup(windowId.c_str());
                    }

                    bool open = true;
                    if (ImGui::BeginPopupModal(windowId.c_str(), &open, ImGuiWindowFlags_AlwaysAutoResize)) {

                        const auto size = ImGui::CalcTextSize(windowId.c_str());

                        ImGui::Text("%s", windowId.c_str());
                        if (ImGui::Button("Track", { size.x * 2.0f, 1.4f * size.y })) {

                            const auto trackerCallback = [&](Core::TrackerCallbackStatus status) -> void {

                                if (status == Core::TrackerCallbackStatus::FAILURE) {

                                    trackingStatus = "Failure";
                                }
                                else {

                                    trackingStatus = "Finished tracking";
                                }
                            };

                            if (tracker.Track(body, { observer.Latitude, observer.Longitude }, trackingDuration, trackerCallback)) {

                                trackingStatus = "Started tracking";
                            }
                            else {

                                trackingStatus = "Failure";
                            }
                        }
                        ImGui::Text("Status: %s", trackingStatus.c_str());

                        ImGui::EndPopup();
                    }
                }
                ImGui::EndTable();
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

    bool TrackableBodyView::drawCelestialBodyCard(const std::shared_ptr<Ephemeris::CelestialBody>& body,
                                                  const Ephemeris::Coordinates::Observer& observer,
                                                  const std::shared_ptr<Core::OpenGL::Texture>& texture,
                                                  const glm::vec2& size) noexcept {

        bool selected = false;

        const auto drawList = ImGui::GetWindowDrawList();
        const auto fontSize = ImGui::GetFontSize();
        auto& style = ImGui::GetStyle();

        const auto itemInnerSpacing = style.ItemInnerSpacing;
        const auto itemSpacing = style.ItemSpacing;
        const auto textColor = ImGui::GetColorU32(style.Colors[ImGuiCol_Text]);
        const auto textLightColor = ImGui::GetColorU32(style.Colors[ImGuiCol_TextDisabled]);
        const auto imageColor = ImGui::GetColorU32(ImVec4{ 255.0f, 255.0f, 255.0f, 255.0f });
        const auto childId = std::format("idChildElement{}{}", body->GetName(), body->GetDesignation());
        const auto selectableActiveColor = style.Colors[ImGuiCol_FrameBgActive];
        const auto selectableHoveredColor = style.Colors[ImGuiCol_FrameBgHovered];

        ImGui::PushStyleColor(ImGuiCol_ChildBg, style.Colors[ImGuiCol_FrameBg]);
        if (ImGui::BeginChild(childId.c_str(), ImVec2{ size.x, size.y }, false, ImGuiWindowFlags_NoScrollbar)) {

            const auto textureId = static_cast<std::intptr_t>(texture->GetNativeHandle());
            const auto cursorPosition = ImGui::GetCursorPos();
            const auto windowPosition = ImGui::GetWindowPos();
            auto drawPosition = ImVec2{ windowPosition.x + cursorPosition.x, windowPosition.y + cursorPosition.y };

            const auto selectableId = std::format("idSelectable{}{}", body->GetName(), body->GetDesignation());
            ImGui::PushID(selectableId.c_str());
            ImGui::PushStyleColor(ImGuiCol_HeaderActive, selectableActiveColor);
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, selectableHoveredColor);
            if (ImGui::Selectable("", false, ImGuiSelectableFlags_None, { size.x, size.y })) {

                selected = true;
            }
            ImGui::PopStyleColor(2);
            ImGui::PopID();

            drawList->AddImageRounded(reinterpret_cast<void*>(textureId), drawPosition, { drawPosition.x + size.y, drawPosition.y + size.y }, { 0.0f, 1.0f }, { 1.0f, 0.0f }, imageColor, 4.0f);
            drawPosition = { drawPosition.x + size.y + itemSpacing.x, drawPosition.y };

            drawList->AddText(Core::UIFont::Medium, fontSize, drawPosition, textColor, body->GetName().c_str());
            drawPosition = { drawPosition.x, drawPosition.y + fontSize + itemSpacing.y * 0.6f };

            drawList->AddText(Core::UIFont::Regular, fontSize - 2.0f, drawPosition, textLightColor, body->GetDesignation().c_str());
            drawPosition = { drawPosition.x, drawPosition.y + fontSize - 2.0f + itemSpacing.y * 0.6f };

            const auto now = DateTime::Now();
            const auto positionPreview = Ephemeris::Coordinates::Transform::TerrestrialObserverToHorizontal(
                    body->GetSphericalPosition(now),
                    observer,
                    now
            );

            const auto azimuthText = std::format("Azimuth: {}", positionPreview.Azimuth);
            drawList->AddText(Core::UIFont::Regular, fontSize - 2.0f, drawPosition, textLightColor, azimuthText.c_str());
            drawPosition = { drawPosition.x, drawPosition.y + fontSize - 2.0f + itemSpacing.y * 0.6f };

            const auto elevationText = std::format("Elevation: {}", positionPreview.Altitude);
            drawList->AddText(Core::UIFont::Regular, fontSize - 2.0f, drawPosition, textLightColor, elevationText.c_str());
        }
        ImGui::EndChild();
        ImGui::PopStyleColor();

        return selected;
    }
}