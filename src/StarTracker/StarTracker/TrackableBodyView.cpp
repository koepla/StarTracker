#include "TrackableBodyView.hpp"

namespace StarTracker {

	TrackableBodyView::TrackableBodyView(void* nativeWindowHandle) noexcept : Core::View{ nativeWindowHandle }, observer{}, tracker{}, bodyLibrary{}, trackingDuration{} {
	
	}

	void TrackableBodyView::OnInit() noexcept {

		bodyLibrary = Core::AssetDataBase::LoadBodyLibrary("CelestialBodies.json");
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

		if (ImGui::Begin("Tracking")) {

			// Info UI
			const auto& style = ImGui::GetStyle();
			const auto itemSpacing = style.ItemSpacing;
			const auto fontSize = ImGui::GetFontSize();
			const auto trackerInfoCardHeight = 3.0f * fontSize + (2.0f + 2.0f * 0.7f) * itemSpacing.y - 4.0f;
			drawTrackerInfoCard({ ImGui::GetContentRegionAvail().x * 0.5f, trackerInfoCardHeight });
			ImGui::SameLine();
			drawTrackingDurationCard({ ImGui::GetContentRegionAvail().x, trackerInfoCardHeight });

			// Filter Search Box
			constexpr auto searchBufferSize = std::size_t{ 128 };
			static std::vector<char> searchBuffer(searchBufferSize);

			{
				UI::ScopedWidth inputTextWidth{ ImGui::GetContentRegionAvail().x };
				ImGui::InputTextWithHint("##idBodySearchEngine", "Search Celestial Body...", searchBuffer.data(), searchBufferSize);
			}

			// Get the filtered library
			const auto filter = std::string{ searchBuffer.data() };
			const auto& filteredLibrary = bodyLibrary->GetFilteredBodies(filter);
		
			// Very messy UI for Bodies
			if (ImGui::BeginChild("idChildTrackableBodiesList", ImGui::GetContentRegionAvail(), false, ImGuiWindowFlags_AlwaysVerticalScrollbar)) {

				if (ImGui::BeginTable("Trackable Bodies", 1)) {

					for(const auto& entry : filteredLibrary) {
		
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);

						const auto& body = entry.Body;
						const auto windowId = std::format("Tracking {} ({})", body->GetName(), body->GetDesignation());
						const auto celestialBodyCardHeight = 4.0f * fontSize + (2.0f + 3 * 0.7f) * itemSpacing.y - 6.0f;

						if (drawCelestialBodyCard(entry, { ImGui::GetContentRegionAvail().x, celestialBodyCardHeight })) {
		
							ImGui::OpenPopup(windowId.c_str());
						}
						drawTrackingMenu(entry, windowId);
					}
					ImGui::EndTable();
				}
			}
			ImGui::EndChild();
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
		
		// Style
		const auto style = ImGui::GetStyle();
		
		UI::ScopedColor childBackground{ ImGuiCol_ChildBg, style.Colors[ImGuiCol_FrameBg] };
		if (ImGui::BeginChild("idChildTrackerInfoCard", { size.x, size.y }, false, ImGuiWindowFlags_NoScrollbar)) {

			// Initial Cursor Position
			const auto initialCursor = UI::DrawCursor::Get();

			// Item Spacings
			const auto itemInnerSpacing = style.ItemInnerSpacing;
			const auto itemSpacing = style.ItemSpacing;
			const auto regulatedItemSpacing = 0.7f * itemInnerSpacing.x;

			// Text Colors
			const auto& baseTextColor = style.Colors[ImGuiCol_Text];
			const auto& baseTextLightColor = ImVec4{ baseTextColor.x, baseTextColor.y, baseTextColor.z, 0.5f * baseTextColor.w };

			// Font Sizes
			const auto fontSize = ImGui::GetFontSize();
			const auto smallFontSize = fontSize - 2.0f;

			// Current Date and Time
			const auto dateTimeInfo = DateTime::Now().ToString();
			UI::DrawCursor::Advance({ itemInnerSpacing.x, 0.0f });
			UI::Text::Draw(dateTimeInfo.c_str(), UI::Font::Medium, fontSize, baseTextColor);

			// Status of the Tracker
			const auto trackerConnectionInfo = std::format("{}", tracker.IsConnected() ? "Connected" : "Not Connected");
			const auto trackerStatusTextWidth = ImGui::CalcTextSize("Tracker:").x;
			const auto trackerStatusTextColor = [&]() -> ImVec4 {

				if (tracker.IsConnected()) {

					return ImVec4{ 0.25f, 0.85f, 0.15f, 1.0f };
				}
				else {

					return ImVec4{ 1.0f, 0.0f, 0.0f, 1.0f };
				}
			}();
			UI::DrawCursor::Advance({ 0.0f, fontSize + regulatedItemSpacing });
			UI::Text::Draw("Tracker:", UI::Font::Regular, smallFontSize, baseTextLightColor);
			UI::DrawCursor::Advance({ trackerStatusTextWidth, 0.0f });
			UI::Text::Draw(trackerConnectionInfo.c_str(), UI::Font::Regular, smallFontSize, trackerStatusTextColor);
			UI::DrawCursor::Advance({ -1.0f * trackerStatusTextWidth, 0.0f });

			// Location of the Observer
			const auto locationInfo = std::format("Location: {}, {}, {}", observer.City, observer.RegionName, observer.Country);
			UI::DrawCursor::Advance({ 0.0f, smallFontSize + regulatedItemSpacing });
			UI::Text::Draw(locationInfo.c_str(), UI::Font::Regular, smallFontSize, baseTextLightColor);

			const auto reconnectButtonWidth = ImGui::CalcTextSize("Reconnect").x * 2.5f;
			UI::DrawCursor::Set(initialCursor);
			UI::DrawCursor::Advance({ size.x - reconnectButtonWidth - itemInnerSpacing.x, itemSpacing.y });

			if (ImGui::Button("Reconnect", { reconnectButtonWidth, size.y - 2.0f * itemSpacing.y })) {

				std::thread connectJob{ [&]() -> void {

					for (auto i = std::size_t{ 0 }; i < 10; i++) {

						if (tracker.Connect()) {

							return;
						}
						else {

							Utils::Diagnostics::Stopwatch stopwatch{};
							stopwatch.Start();
							while (stopwatch.GetEllapsedMilliseconds() != 500.0);
							stopwatch.Stop();
						}
					}
				} };
				connectJob.detach();
			}
		}
		ImGui::EndChild();
	}

	void TrackableBodyView::drawTrackingDurationCard(const glm::vec2 &size) noexcept {

		const auto& style = ImGui::GetStyle();

		UI::ScopedColor childBackground{ ImGuiCol_ChildBg, style.Colors[ImGuiCol_FrameBg] };
		if (ImGui::BeginChild("idChildTrackingDurationCard", { size.x, size.y }, false, ImGuiWindowFlags_NoScrollbar)) {

			// Item Spacings
			const auto itemInnerSpacing = style.ItemInnerSpacing;
			const auto itemSpacing = style.ItemSpacing;
			const auto regulatedItemSpacing = 0.7f * itemInnerSpacing.x;

			// Text Colors
			const auto& baseTextColor = style.Colors[ImGuiCol_Text];

			// Font Size
			const auto fontSize = ImGui::GetFontSize();

			UI::DrawCursor::Advance({ itemInnerSpacing.x, 0.0f });
			UI::Text::Draw("Tracking Duration", UI::Font::Medium, fontSize, baseTextColor);

			{
				const auto fullHeight = size.y - 2.0f * itemSpacing.y - 0.75f * itemInnerSpacing.y;
				const auto framePadding = ImVec2{ itemInnerSpacing.x, fullHeight - 2.0f * fontSize - regulatedItemSpacing - itemInnerSpacing.y };

				UI::ScopedID sliderId{ "idTrackingDurationEnumSlider" };
				UI::ScopedColor frameBackground{ ImGuiCol_FrameBg, style.Colors[ImGuiCol_MenuBarBg] };
				UI::ScopedWidth width{ size.x - 2.0f * itemInnerSpacing.x };
				UI::ScopedStyleVar framePaddingVar{ ImGuiStyleVar_FramePadding , framePadding };

				static int selectedTrackingDurationIndex{ 0 };
				const char* selectedTrackingDurationNames[6] = { "5 Seconds", "10 Seconds", "30 Seconds", "1 Minute", "5 Minutes", "10 Minutes" };

				UI::DrawCursor::Advance({ 0.0f, fontSize + regulatedItemSpacing });
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
			}
		}
		ImGui::EndChild();
	}

	bool TrackableBodyView::drawCelestialBodyCard(Core::BodyLibraryEntry entry, const glm::vec2& size) noexcept {

		const auto body = entry.Body;
		const auto texture = entry.Texture;
		std::string name{ body->GetName() };
		std::string designation{ body->GetDesignation() };

		bool selected{ false };
		const auto style = ImGui::GetStyle();
		
		{
			UI::ScopedColor childBackground{ ImGuiCol_ChildBg, style.Colors[ImGuiCol_FrameBg] };
			if (ImGui::BeginChild(std::format("idChildElement{}{}", name, designation).c_str(), ImVec2{ size.x, size.y }, false, ImGuiWindowFlags_NoScrollbar)) {

				// Item Spacings
				const auto itemSpacing = style.ItemSpacing;
				const auto itemInnerSpacing = style.ItemInnerSpacing;
				const auto regulatedItemSpacing = 0.7f * itemInnerSpacing.x;

				// Text Colors
				const auto baseTextColor = style.Colors[ImGuiCol_Text];
				const auto baseTextLightColor = ImVec4{ baseTextColor.x, baseTextColor.y, baseTextColor.z, 0.5f * baseTextColor.w };

				// Font Sizes
				const auto fontSize = ImGui::GetFontSize();
				const auto smallFontSize = fontSize - 2.0f;

				const auto cursor = UI::DrawCursor::Get();
				{
					UI::ScopedID selectable{ std::format("idSelectable{}{}", name, designation) };
					UI::ScopedColor headerActive{ ImGuiCol_HeaderActive, style.Colors[ImGuiCol_FrameBgActive] };
					UI::ScopedColor headerHovered{ ImGuiCol_HeaderHovered, style.Colors[ImGuiCol_FrameBgHovered] };
					if (ImGui::Selectable("", false, ImGuiSelectableFlags_None, { size.x, size.y })) {

						selected = true;
					}
				}
				UI::DrawCursor::Set(cursor);

				// Rounded Image for current Celestial Body
				UI::Image::DrawRounded(texture->GetNativeHandle(), { size.y, size.y });

				// Name of Celestial Body
				UI::DrawCursor::Advance({ size.y + itemSpacing.x, 0.0f });
				UI::Text::Draw(name, UI::Font::Medium, fontSize, baseTextColor);

				// Designation of Celestial Body
				UI::DrawCursor::Advance({ 0.0f, fontSize + regulatedItemSpacing });
				UI::Text::Draw(designation, UI::Font::Regular, smallFontSize, baseTextLightColor);

				// Compute the position preview
				const auto now = DateTime::Now();
				const auto positionPreview = Ephemeris::Coordinates::Transform::TerrestrialObserverToHorizontal(
					body->GetSphericalPosition(now),
					{ observer.Latitude, observer.Longitude },
					now
				);

				// Azimuth-Angle of the Celestial Body
				const auto azimuthText = std::format("Azimuth: {}", positionPreview.Azimuth);
				UI::DrawCursor::Advance({ 0.0f, smallFontSize + regulatedItemSpacing });
				UI::Text::Draw(azimuthText, UI::Font::Regular, smallFontSize, baseTextLightColor);

				// Elevation-Angle of the Celestial Body
				const auto elevationText = std::format("Elevation: {}", positionPreview.Altitude);
				UI::DrawCursor::Advance({ 0.0f, smallFontSize + regulatedItemSpacing });
				UI::Text::Draw(elevationText, UI::Font::Regular, smallFontSize, baseTextLightColor);
			}
			ImGui::EndChild();
		}

		return selected;
	}

	void TrackableBodyView::drawTrackingMenu(Core::BodyLibraryEntry entry, std::string_view title) noexcept {

		const auto& body = entry.Body;
		const auto& name = body->GetName();
		const auto& designation = body->GetDesignation();
		const auto& texture = entry.Texture;
		bool open{ true };

		if (ImGui::BeginPopupModal(title.data(), &open, ImGuiWindowFlags_Modal)) {

			{
				UI::ScopedFont mediumFont{ UI::Font::Medium };

				const auto headerText = std::format("Tracking {} ({} s)", name, trackingDuration);
				ImGui::Text("%s", headerText.c_str());
				ImGui::Separator();
			}

			const auto now = DateTime::Now();
			const auto positionPreview = Ephemeris::Coordinates::Transform::TerrestrialObserverToHorizontal(
				body->GetSphericalPosition(now),
				{ observer.Latitude, observer.Longitude },
				now
			);

			const auto azimuthText = std::format("Azimuth: {}", positionPreview.Azimuth);
			ImGui::Text("%s", azimuthText.c_str());

			const auto elevationText = std::format("Elevation: {}", positionPreview.Altitude);
			ImGui::Text("%s", elevationText.c_str());

			static std::string trackingStatus{ "Not Tracking" };
			ImGui::Text("Status: %s", trackingStatus.c_str());

			if (ImGui::Button("Start", { ImGui::GetContentRegionAvail().x, 0.0f })) {

				if (tracker.Track(body, { observer.Latitude, observer.Longitude }, trackingDuration * 1000, 
					
					[&](Core::TrackerCallbackStatus status) {

						switch (status) {

						case Core::TrackerCallbackStatus::FAILURE: trackingStatus = "Failed"; break;
						case Core::TrackerCallbackStatus::SUCCESS: trackingStatus = "Finished"; break;
						default: ASSERT(false && "Invalid Tracking Status"); break;
						}
					}
				)) {

					trackingStatus = "Started";
				}
				else {

					trackingStatus = "Failed";
				}
			}
			ImGui::EndPopup();
		}
	}
}