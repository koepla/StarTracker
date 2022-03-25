#include "TrackableBodyView.hpp"

namespace StarTracker {

	TrackableBodyView::TrackableBodyView(void* nativeWindowHandle) noexcept : Core::View{ nativeWindowHandle }, observer{}, celestialBodies{}, tracker{}, trackingDuration{} {
		
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

            constexpr auto maxConnectionTries = std::size_t{ 3 };
            for (auto i = std::size_t{ 0 }; i < maxConnectionTries; i++) {

                if (tracker.Connect()) {

                    break;
                }
            }
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

        if (ImGui::BeginMainMenuBar()) {

            if (ImGui::BeginMenu("File")) {

                if (ImGui::MenuItem("Load CelestialBodies", "CTRL+L")) {

                    reloadCelestialBodies();
                }
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

		if (ImGui::Begin("Tracking")) {

            auto& style = ImGui::GetStyle();
            const auto itemSpacing = style.ItemSpacing;
            const auto fontSize = ImGui::GetFontSize();
            const auto trackerInfoCardHeight = 3.0f * fontSize + (2.0f + 2.0f * 0.7f) * itemSpacing.y - 4.0f;
			drawTrackerInfoCard({ ImGui::GetContentRegionAvail().x * 0.5f, trackerInfoCardHeight });

            ImGui::SameLine();

            drawTrackingDurationCard({ ImGui::GetContentRegionAvail().x, trackerInfoCardHeight });

            if (ImGui::BeginTable("Trackable Bodies", 1)) {

                for(const auto& body : celestialBodies) {

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);

                    const auto windowId = std::format("Tracking {} ({})", body->GetName(), body->GetDesignation());
                    const auto celestialBodyCardHeight = 4.0f * fontSize + (2.0f + 3 * 0.7f) * itemSpacing.y - 6.0f;

                    static std::string trackingStatus{ "Not tracking" };

                    if (drawCelestialBodyCard(body,
                                          Core::AssetDataBase::LoadTexture("pillarsOfCreation.jpg"),
                                          { ImGui::GetContentRegionAvail().x, celestialBodyCardHeight })) {

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

                                    trackingStatus = "Failure on tracking";
                                }
                                else {

                                    trackingStatus = "Finished tracking";
                                }
                            };

                            if (tracker.Track(body, { observer.Latitude, observer.Longitude }, trackingDuration, trackerCallback)) {

                                trackingStatus = "Started tracking";
                            }
                            else {

                                trackingStatus = "Failure on tracking";
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

    void TrackableBodyView::drawTrackerInfoCard(const glm::vec2& size) noexcept {

        const auto style = ImGui::GetStyle();
        ImGui::PushStyleColor(ImGuiCol_ChildBg, style.Colors[ImGuiCol_FrameBg]);
        if (ImGui::BeginChild("idChildTrackerInfoCard", { size.x, size.y }, false, ImGuiWindowFlags_NoScrollbar)) {

            const auto dateTimeInfo = DateTime::Now().ToString();
            const auto trackerConnectionInfo = std::format("{}", tracker.IsConnected() ? "Connected" : "Not Connected");
            const auto locationInfo = std::format("Location: {}, {}, {}", observer.City, observer.RegionName, observer.Country);

            const auto drawList = ImGui::GetWindowDrawList();
            const auto cursorPosition = ImGui::GetCursorPos();
            const auto windowPosition = ImGui::GetWindowPos();
            const auto& itemInnerSpacing = style.ItemInnerSpacing;
            const auto& itemSpacing = style.ItemSpacing;
            const auto textColor = ImGui::GetColorU32(style.Colors[ImGuiCol_Text]);
            const auto textLightColor = ImGui::GetColorU32(style.Colors[ImGuiCol_TextDisabled]);
            const auto fontSize = ImGui::GetFontSize();
            auto drawPosition = ImVec2{ windowPosition.x + cursorPosition.x + itemInnerSpacing.x, windowPosition.y + cursorPosition.y };
            const auto regulatedItemSpacing = 0.7f * itemInnerSpacing.x;
            
            // Current Date and Time
            drawList->AddText(Core::UIFont::Medium, fontSize, drawPosition, textColor, dateTimeInfo.c_str());
            drawPosition = { drawPosition.x, drawPosition.y + fontSize + regulatedItemSpacing };

            // Status of the Tracker
            const auto trackerStatusTextWidth = ImGui::CalcTextSize("Tracker: ").x;
            const auto trackerStatusDrawPosition = ImVec2{ drawPosition.x + trackerStatusTextWidth, drawPosition.y };
            const auto trackerStatusTextColor = [&]() -> ImU32 {

                if (tracker.IsConnected()) {

                    return ImGui::GetColorU32(ImVec4{ 0.25f, 0.85f, 0.15f, 1.0f });
                }
                else {

                    return ImGui::GetColorU32(ImVec4{ 1.0f, 0.0f, 0.0f, 1.0f });
                }
            }();
            drawList->AddText(Core::UIFont::Regular, fontSize - 2.0f, drawPosition, textLightColor, "Tracker: ");
            drawList->AddText(Core::UIFont::Regular, fontSize - 2.0f, trackerStatusDrawPosition, trackerStatusTextColor, trackerConnectionInfo.c_str());
            drawPosition = { drawPosition.x, drawPosition.y + fontSize - 2.0f + regulatedItemSpacing };

            // Location of the Observer
            drawList->AddText(Core::UIFont::Regular, fontSize - 2.0f, drawPosition, textLightColor, locationInfo.c_str());

            const auto reconnectButtonWidth = ImGui::CalcTextSize("Reconnect").x * 2.5f;
            ImGui::SetCursorPosX(cursorPosition.x + size.x - reconnectButtonWidth - itemInnerSpacing.x);
            ImGui::SetCursorPosY(cursorPosition.y + itemInnerSpacing.y);

            if (tracker.IsConnected()) {

                const auto reconnectDisabledButtonColor = style.Colors[ImGuiCol_MenuBarBg];
                ImGui::PushStyleColor(ImGuiCol_Button, reconnectDisabledButtonColor);
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, reconnectDisabledButtonColor);
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, reconnectDisabledButtonColor);
            }

            if (ImGui::Button("Reconnect", { reconnectButtonWidth, size.y - 2.0f * itemSpacing.y - 0.25f * itemInnerSpacing.y })) {

                std::thread connectJob{ [&]() -> void {

                    constexpr auto maxConnectionTries = std::size_t{ 10 };
                    for (auto i = std::size_t{ 0 }; i < maxConnectionTries; i++) {

                        if (tracker.Connect()) {

                            return;
                        }
                        else {

                            constexpr auto maxWaitTime = 500.0;
                            Utils::Diagnostics::Stopwatch stopwatch{};
                            stopwatch.Start();
                            while(stopwatch.GetEllapsedMilliseconds() != maxWaitTime);
                            stopwatch.Stop();
                        }
                    }
                }};
                connectJob.detach();
            }

            if (tracker.IsConnected()) {

                ImGui::PopStyleColor(3);
            }
        }
        ImGui::EndChild();
        ImGui::PopStyleColor();
    }

    void TrackableBodyView::drawTrackingDurationCard(const glm::vec2 &size) noexcept {

        const auto& style = ImGui::GetStyle();
        ImGui::PushStyleColor(ImGuiCol_ChildBg, style.Colors[ImGuiCol_FrameBg]);
        if (ImGui::BeginChild("idChildTrackingDurationCard", { size.x, size.y }, false, ImGuiWindowFlags_NoScrollbar)) {

            const auto drawList = ImGui::GetWindowDrawList();
            const auto cursorPosition = ImGui::GetCursorPos();
            const auto windowPosition = ImGui::GetWindowPos();
            const auto& itemInnerSpacing = style.ItemInnerSpacing;
            const auto& itemSpacing = style.ItemSpacing;
            const auto textColor = ImGui::GetColorU32(style.Colors[ImGuiCol_Text]);
            const auto fontSize = ImGui::GetFontSize();
            auto drawPosition = ImVec2{ windowPosition.x + cursorPosition.x + itemInnerSpacing.x, windowPosition.y + cursorPosition.y };
            const auto regulatedItemSpacing = 0.7f * itemInnerSpacing.x;

            drawList->AddText(Core::UIFont::Medium, fontSize, drawPosition, textColor, "Tracking Duration");

            ImGui::SetCursorPosX(cursorPosition.x + itemInnerSpacing.x);
            ImGui::SetCursorPosY(cursorPosition.y + fontSize + regulatedItemSpacing);

            const auto fullHeight = size.y - 2.0f * itemSpacing.y - 0.75f * itemInnerSpacing.y;
            const auto framePadding = ImVec2{ itemInnerSpacing.x, fullHeight - 2.0f * fontSize - regulatedItemSpacing - itemInnerSpacing.y };
            static ImGuiInputTextFlags trackingDurationInputFlags{ ImGuiInputTextFlags_None };
            static int selectedTrackingDurationIndex{ 0 };
            const char* selectedTrackingDurationNames[6] = { "5 Seconds", "10 Seconds", "30 Seconds", "1 Minute", "5 Minutes", "10 Minutes" };
            ImGui::PushItemWidth(size.x - 2.0f * itemInnerSpacing.x);
            ImGui::PushID("idTrackingDurationEnumSlider");
            ImGui::PushStyleColor(ImGuiCol_FrameBg, style.Colors[ImGuiCol_MenuBarBg]);
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, framePadding);
            ImGui::SliderInt("", &selectedTrackingDurationIndex, 0, 5, [&]() -> const char* {

                switch (selectedTrackingDurationIndex) {

                    case 0: trackingDuration = 5; break;
                    case 1: trackingDuration = 10; break;
                    case 2: trackingDuration = 30; break;
                    case 3: trackingDuration = 60; break;
                    case 4: trackingDuration = 300; break;
                    case 5: trackingDuration = 600; break;
                    default: break;
                }

                return selectedTrackingDurationNames[selectedTrackingDurationIndex];
            }());
            ImGui::PopStyleVar();
            ImGui::PopStyleColor();
            ImGui::PopID();
            ImGui::PopItemWidth();
        }
        ImGui::EndChild();
        ImGui::PopStyleColor();

        std::fprintf(stdout, "Duration: %lf\n", trackingDuration);
    }

    bool TrackableBodyView::drawCelestialBodyCard(const std::shared_ptr<Ephemeris::CelestialBody>& body,
                                                  const std::shared_ptr<Core::OpenGL::Texture>& texture,
                                                  const glm::vec2& size) noexcept {

        bool selected{ false };
        const auto& style = ImGui::GetStyle();
        const auto childId = std::format("idChildElement{}{}", body->GetName(), body->GetDesignation());
        ImGui::PushStyleColor(ImGuiCol_ChildBg, style.Colors[ImGuiCol_FrameBg]);
        if (ImGui::BeginChild(childId.c_str(), ImVec2{ size.x, size.y }, false, ImGuiWindowFlags_NoScrollbar)) {

            const auto textureId = static_cast<std::intptr_t>(texture->GetNativeHandle());
            const auto drawList = ImGui::GetWindowDrawList();
            const auto cursorPosition = ImGui::GetCursorPos();
            const auto windowPosition = ImGui::GetWindowPos();
            const auto& itemSpacing = style.ItemSpacing;
            const auto itemInnerSpacing = style.ItemInnerSpacing;
            const auto textColor = ImGui::GetColorU32(style.Colors[ImGuiCol_Text]);
            const auto textLightColor = ImGui::GetColorU32(style.Colors[ImGuiCol_TextDisabled]);
            const auto imageColor = ImGui::GetColorU32(ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f });
            const auto& selectableActiveColor = style.Colors[ImGuiCol_FrameBgActive];
            const auto& selectableHoveredColor = style.Colors[ImGuiCol_FrameBgHovered];
            const auto fontSize = ImGui::GetFontSize();
            auto drawPosition = ImVec2{ windowPosition.x + cursorPosition.x, windowPosition.y + cursorPosition.y };
            const auto regulatedItemSpacing = 0.7f * itemInnerSpacing.x;

            // Basic Selectable Background
            const auto selectableId = std::format("idSelectable{}{}", body->GetName(), body->GetDesignation());
            ImGui::PushID(selectableId.c_str());
            ImGui::PushStyleColor(ImGuiCol_HeaderActive, selectableActiveColor);
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, selectableHoveredColor);
            if (ImGui::Selectable("", false, ImGuiSelectableFlags_None, { size.x, size.y })) {

                selected = true;
            }
            ImGui::PopStyleColor(2);
            ImGui::PopID();

            // Rounded Image for current Celestial Body
            drawList->AddImageRounded(reinterpret_cast<void*>(textureId), drawPosition, { drawPosition.x + size.y, drawPosition.y + size.y }, { 0.0f, 1.0f }, { 1.0f, 0.0f }, imageColor, 4.0f);
            drawPosition = { drawPosition.x + size.y + itemSpacing.x, drawPosition.y };

            // Name of Celestial Body
            drawList->AddText(Core::UIFont::Medium, fontSize, drawPosition, textColor, body->GetName().c_str());
            drawPosition = { drawPosition.x, drawPosition.y + fontSize + regulatedItemSpacing };

            // Designation of Celestial Body
            drawList->AddText(Core::UIFont::Regular, fontSize - 2.0f, drawPosition, textLightColor, body->GetDesignation().c_str());
            drawPosition = { drawPosition.x, drawPosition.y + fontSize - 2.0f + regulatedItemSpacing };

            // Compute the position preview
            const auto now = DateTime::Now();
            const auto positionPreview = Ephemeris::Coordinates::Transform::TerrestrialObserverToHorizontal(
                    body->GetSphericalPosition(now),
                    { observer.Latitude, observer.Longitude },
                    now
            );

            // Azimuth-Angle of the Celestial Body
            const auto azimuthText = std::format("Azimuth: {}", positionPreview.Azimuth);
            drawList->AddText(Core::UIFont::Regular, fontSize - 2.0f, drawPosition, textLightColor, azimuthText.c_str());
            drawPosition = { drawPosition.x, drawPosition.y + fontSize - 2.0f + regulatedItemSpacing };

            // Elevation-Angle of the Celestial Body
            const auto elevationText = std::format("Elevation: {}", positionPreview.Altitude);
            drawList->AddText(Core::UIFont::Regular, fontSize - 2.0f, drawPosition, textLightColor, elevationText.c_str());
        }
        ImGui::EndChild();
        ImGui::PopStyleColor();

        return selected;
    }
}