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

        reconnectToTracker();
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
            const auto entryButtonWidth = ImGui::CalcTextSize("Add Entry").x * 1.4f;
            constexpr std::size_t searchBufferSize = 128;
            static std::vector<char> searchBuffer(searchBufferSize);
            {
                UI::ScopedWidth inputTextWidth{ ImGui::GetContentRegionAvail().x - entryButtonWidth - itemSpacing.x };
                ImGui::InputTextWithHint("##idBodySearchEngine", "Search Celestial Body...", searchBuffer.data(), searchBufferSize);
            }

            ImGui::SameLine();

            constexpr const auto addEntryPopupId = "Add Library Entry";
            if (ImGui::Button("Add Entry", { ImGui::GetContentRegionAvail().x, 0.0f })) {

                ImGui::OpenPopup(addEntryPopupId);
            }
            drawAddEntryMenu(addEntryPopupId);

            Utils::Diagnostics::Stopwatch stopwatch{};
            stopwatch.Start();

            // Get the filtered library
            const auto filter = std::string{ searchBuffer.data() };
            const auto& filteredLibrary = bodyLibrary->GetFilteredBodies(filter);

            // Very messy UI for Bodies
            if (ImGui::BeginChild("idChildTrackableBodiesList", ImGui::GetContentRegionAvail(), false, ImGuiWindowFlags_AlwaysVerticalScrollbar)) {

                if (ImGui::BeginTable("Trackable Bodies", 1)) {

                    for (const auto& entry : filteredLibrary) {

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);

                        const auto& body = entry.Body;
                        const auto windowId = fmt::format("Tracking {} ({})", body->GetName(), body->GetDesignation());
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

    void TrackableBodyView::reconnectToTracker() noexcept {

        try {

            constexpr std::size_t maxConnectionTries = 3;
            for (std::size_t i = 0; i < maxConnectionTries; i++) {

                if (tracker.Connect()) {

                    if (tracker.IsConnected()) {

                        break;
                    }
                }
            }
        }
        catch (const std::exception& e) {

            STARTRACKER_ERROR("Couldn't connect to tracker: {}", e.what());
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
            const auto trackerConnectionInfo = fmt::format("{}", tracker.IsConnected() ? "Connected" : "Not Connected");
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
            const auto locationInfo = fmt::format("Location: {}, {}, {}", observer.City, observer.RegionName, observer.Country);
            UI::DrawCursor::Advance({ 0.0f, smallFontSize + regulatedItemSpacing });
            UI::Text::Draw(locationInfo.c_str(), UI::Font::Regular, smallFontSize, baseTextLightColor);

            const auto reconnectButtonWidth = ImGui::CalcTextSize("Reconnect").x * 2.5f;
            UI::DrawCursor::Set(initialCursor);
            UI::DrawCursor::Advance({ size.x - reconnectButtonWidth - itemInnerSpacing.x, itemSpacing.y });

            if (ImGui::Button("Reconnect", { reconnectButtonWidth, size.y - 2.0f * itemSpacing.y })) {

                reconnectToTracker();
            }
        }
        ImGui::EndChild();
    }

    void TrackableBodyView::drawTrackingDurationCard(const glm::vec2& size) noexcept {

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
            if (ImGui::BeginChild(fmt::format("idChildElement{}{}", name, designation).c_str(), ImVec2{ size.x, size.y }, false, ImGuiWindowFlags_NoScrollbar)) {

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
                    UI::ScopedID selectable{ fmt::format("idSelectable{}{}", name, designation) };
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
                const auto azimuthText = fmt::format("Azimuth: {} deg", positionPreview.Azimuth);
                UI::DrawCursor::Advance({ 0.0f, smallFontSize + regulatedItemSpacing });
                UI::Text::Draw(azimuthText, UI::Font::Regular, smallFontSize, baseTextLightColor);

                // Elevation-Angle of the Celestial Body
                const auto elevationText = fmt::format("Elevation: {} deg", positionPreview.Altitude);
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

                const auto headerText = fmt::format("Tracking {} ({} s)", name, trackingDuration);
                ImGui::Text("%s", headerText.c_str());
                ImGui::Separator();
            }

            const auto now = DateTime::Now();
            const auto positionPreview = Ephemeris::Coordinates::Transform::TerrestrialObserverToHorizontal(
                body->GetSphericalPosition(now),
                { observer.Latitude, observer.Longitude },
                now
            );

            const auto azimuthText = fmt::format("Azimuth: {}", positionPreview.Azimuth);
            ImGui::Text("%s", azimuthText.c_str());

            const auto elevationText = fmt::format("Elevation: {}", positionPreview.Altitude);
            ImGui::Text("%s", elevationText.c_str());

            static std::string trackingStatus{ "Not Tracking" };
            ImGui::Text("Status: %s", trackingStatus.c_str());

            if (ImGui::Button("Start", { ImGui::GetContentRegionAvail().x, 0.0f })) {

                try {

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
                catch (const std::exception& e) {

                    trackingStatus = "Failed";
                    STARTRACKER_ERROR("Cannot track: {}", e.what());
                }
            }
            ImGui::EndPopup();
        }
    }

    void TrackableBodyView::drawAddEntryMenu(std::string_view title) noexcept {

        const auto& style = ImGui::GetStyle();
        const auto itemSpacing = style.ItemSpacing;

        bool open{ true };
        if (ImGui::BeginPopupModal(title.data(), &open, ImGuiWindowFlags_Modal)) {

            constexpr static auto bufferSize = std::size_t{ 64 };

            // Name, Designation and Type
            static std::vector<char> nameBuffer(bufferSize);
            static std::vector<char> designationBuffer(bufferSize);
            static int selectedType{ 0 };

            // Fixed Body
            static double rightAscensionHour{ 0.0 };
            static double rightAscensionMinute{ 0.0 };
            static double rightAscensionSecond{ 0.0 };
            static double declinationDeg{ 0.0 };
            static double declinationArcMinute{ 0.0 };
            static double declinationArcSecond{ 0.0 };
            static double radius{ 0.0 };

            // Solar System Body
            static Ephemeris::KeplerianElements elements{};
            static Ephemeris::KeplerianElements elementsCentury{};

            const auto clearInputs = [&]() -> void {

                nameBuffer.clear();
                nameBuffer.resize(bufferSize);
                designationBuffer.clear();
                designationBuffer.resize(bufferSize);

                rightAscensionHour = 0.0;
                rightAscensionMinute = 0.0;
                rightAscensionSecond = 0.0;
                declinationDeg = 0.0;
                declinationArcMinute = 0.0;
                declinationArcSecond = 0.0;
                radius = 0.0;

                elements = { 0.0 };
                elementsCentury = { 0.0 };
            };

            if (ImGui::BeginTable("##idAddEntryMenuAlignmentTable", 2)) {

                ImGui::TableNextRow();
                {
                    ImGui::TableSetColumnIndex(0);
                    {
                        ImGui::Text("Name");
                    }
                    ImGui::TableSetColumnIndex(1);
                    {
                        UI::ScopedWidth nameInputWidth{ ImGui::GetContentRegionAvail().x };
                        ImGui::InputTextWithHint("##idAddEntryMenuName", "Pluto", nameBuffer.data(), nameBuffer.size());
                    }
                }

                ImGui::TableNextRow();
                {
                    ImGui::TableSetColumnIndex(0);
                    {
                        ImGui::Text("Designation");
                    }
                    ImGui::TableSetColumnIndex(1);
                    {
                        UI::ScopedWidth nameInputWidth{ ImGui::GetContentRegionAvail().x };
                        ImGui::InputTextWithHint("##idAddEntryMenuDesignation", "No Designation", designationBuffer.data(), designationBuffer.size());
                    }
                }

                ImGui::TableNextRow();
                {
                    ImGui::TableSetColumnIndex(0);
                    {
                        ImGui::Text("Type");
                    }
                    ImGui::TableSetColumnIndex(1);
                    {
                        UI::ScopedWidth typeComboWidth{ ImGui::GetContentRegionAvail().x };
                        ImGui::Combo("##idAddEntryMenuType", &selectedType, "Fixed Body\0Solar System Body\0\0");
                    }
                }

                if (selectedType == 0) {

                    // Fixed Body
                    ImGui::TableNextRow();
                    {
                        ImGui::TableSetColumnIndex(0);
                        {
                            ImGui::Text("Right Ascension");
                        }
                        ImGui::TableSetColumnIndex(1);
                        {
                            UI::ScopedWidth rightAscensionInputWidth{ ImGui::GetContentRegionAvail().x * 0.33f - 0.5f * itemSpacing.x };
                            ImGui::InputDouble("##idAddEntryMenuRightAscHour", &rightAscensionHour, 0.0, 0.0, "%.1f h");
                            ImGui::SameLine();
                            ImGui::InputDouble("##idAddEntryMenuRightAscMinute", &rightAscensionMinute, 0.0, 0.0, "%.1f m");
                            ImGui::SameLine();
                            ImGui::InputDouble("##idAddEntryMenuRightAscSecond", &rightAscensionSecond, 0.0, 0.0, "%.1f s");
                        }
                    }

                    ImGui::TableNextRow();
                    {
                        ImGui::TableSetColumnIndex(0);
                        {
                            ImGui::Text("Declination");
                        }
                        ImGui::TableSetColumnIndex(1);
                        {
                            UI::ScopedWidth declinationInputWidth{ ImGui::GetContentRegionAvail().x * 0.33f - 0.5f * itemSpacing.x };
                            ImGui::InputDouble("##idAddEntryMenuDeclDeg", &declinationDeg, 0.0, 0.0, "%.1f deg");
                            ImGui::SameLine();
                            ImGui::InputDouble("##idAddEntryMenuDeclArcMinute", &declinationArcMinute, 0.0, 0.0, "%.1f '");
                            ImGui::SameLine();
                            ImGui::InputDouble("##idAddEntryMenuDeclArcSecond", &declinationArcSecond, 0.0, 0.0, "%.1f ''");
                        }
                    }

                    ImGui::TableNextRow();
                    {
                        ImGui::TableSetColumnIndex(0);
                        {
                            ImGui::Text("Radius");
                        }
                        ImGui::TableSetColumnIndex(1);
                        {
                            UI::ScopedWidth radiusInputWidth{ ImGui::GetContentRegionAvail().x };
                            ImGui::InputDouble("##idAddEntryMenuRadius", &radius, 0.0, 0.0, "%.8f au");
                        }
                    }
                }
                else if (selectedType == 1) {

                    // Solar System Body
                    ImGui::TableNextRow();
                    {
                        ImGui::TableSetColumnIndex(0);
                        {
                            ImGui::Text("Semi-Major Axis");
                        }
                        ImGui::TableSetColumnIndex(1);
                        {
                            UI::ScopedWidth semiMajorAxisInputWidth{ ImGui::GetContentRegionAvail().x * 0.5f - 0.5f * itemSpacing.x };
                            ImGui::InputDouble("##idAddEntryMenuSemiMajorAxis", &elements.SemiMajorAxis, 0.0, 0.0, "%.8f au");
                            ImGui::SameLine();
                            ImGui::InputDouble("##idAddEntryMenuSemiMajorAxisCentury", &elementsCentury.SemiMajorAxis, 0.0, 0.0, "%.8f au/century");
                        }
                    }
                    ImGui::TableNextRow();
                    {
                        ImGui::TableSetColumnIndex(0);
                        {
                            ImGui::Text("Eccentricity");
                        }
                        ImGui::TableSetColumnIndex(1);
                        {
                            UI::ScopedWidth semiMajorAxisInputWidth{ ImGui::GetContentRegionAvail().x * 0.5f - 0.5f * itemSpacing.x };
                            ImGui::InputDouble("##idAddEntryMenuEccentricity", &elements.Eccentricity, 0.0, 0.0, "%.8f rad");
                            ImGui::SameLine();
                            ImGui::InputDouble("##idAddEntryMenuEccentricityCentury", &elementsCentury.Eccentricity, 0.0, 0.0, "%.8f rad/century");
                        }
                    }
                    ImGui::TableNextRow();
                    {
                        ImGui::TableSetColumnIndex(0);
                        {
                            ImGui::Text("Inclination");
                        }
                        ImGui::TableSetColumnIndex(1);
                        {
                            UI::ScopedWidth semiMajorAxisInputWidth{ ImGui::GetContentRegionAvail().x * 0.5f - 0.5f * itemSpacing.x };
                            ImGui::InputDouble("##idAddEntryMenuInclination", &elements.Inclination, 0.0, 0.0, "%.8f deg");
                            ImGui::SameLine();
                            ImGui::InputDouble("##idAddEntryMenuInclinationCentury", &elementsCentury.Inclination, 0.0, 0.0, "%.8f deg/century");
                        }
                    }
                    ImGui::TableNextRow();
                    {
                        ImGui::TableSetColumnIndex(0);
                        {
                            ImGui::Text("Mean Longitude");
                        }
                        ImGui::TableSetColumnIndex(1);
                        {
                            UI::ScopedWidth semiMajorAxisInputWidth{ ImGui::GetContentRegionAvail().x * 0.5f - 0.5f * itemSpacing.x };
                            ImGui::InputDouble("##idAddEntryMenuMeanLongitude", &elements.MeanLongitude, 0.0, 0.0, "%.8f deg");
                            ImGui::SameLine();
                            ImGui::InputDouble("##idAddEntryMenuMeanLongitudeCentury", &elementsCentury.MeanLongitude, 0.0, 0.0, "%.8f deg/century");
                        }
                    }
                    ImGui::TableNextRow();
                    {
                        ImGui::TableSetColumnIndex(0);
                        {
                            ImGui::Text("Longitude of Perihelion");
                        }
                        ImGui::TableSetColumnIndex(1);
                        {
                            UI::ScopedWidth semiMajorAxisInputWidth{ ImGui::GetContentRegionAvail().x * 0.5f - 0.5f * itemSpacing.x };
                            ImGui::InputDouble("##idAddEntryLongitudeOfPerihelion", &elements.LonPerihelion, 0.0, 0.0, "%.8f deg");
                            ImGui::SameLine();
                            ImGui::InputDouble("##idAddEntryLongitudeOfPerihelionCentury", &elementsCentury.LonPerihelion, 0.0, 0.0, "%.8f deg/century");
                        }
                    }
                    ImGui::TableNextRow();
                    {
                        ImGui::TableSetColumnIndex(0);
                        {
                            ImGui::Text("Longitude of the Ascending Node");
                        }
                        ImGui::TableSetColumnIndex(1);
                        {
                            UI::ScopedWidth semiMajorAxisInputWidth{ ImGui::GetContentRegionAvail().x * 0.5f - 0.5f * itemSpacing.x };
                            ImGui::InputDouble("##idAddEntryMenuLongitudeOfTheAscendingNode", &elements.LonAscendingNode, 0.0, 0.0, "%.8f deg");
                            ImGui::SameLine();
                            ImGui::InputDouble("##idAddEntryMenuLongitudeOfTheAscendingNodeCentury", &elementsCentury.LonAscendingNode, 0.0, 0.0, "%.8f deg/century");
                        }
                    }
                }

                ImGui::TableNextRow();
                {
                    ImGui::TableSetColumnIndex(1);
                    {
                        const auto buttonWidth = ImGui::GetContentRegionAvail().x * 0.5f - 0.5f * itemSpacing.x;

                        if (ImGui::Button("Cancel", { buttonWidth, 0.0f })) {

                            clearInputs();
                            ImGui::CloseCurrentPopup();
                        }
                        ImGui::SameLine();
                        if (ImGui::Button("Save", { buttonWidth, 0.0f })) {

                            const auto designation = [&]() -> const char* {

                                for (const auto ch : designationBuffer) {

                                    if (ch != ' ' && ch != '\0') {

                                        return designationBuffer.data();
                                    }
                                }

                                return "No Designation";
                            }();

                            if (selectedType == 0) {

                                // Fixed Body

                                Ephemeris::Coordinates::Spherical position{};
                                position.RightAscension = Math::HmsToDegrees(rightAscensionHour, rightAscensionMinute, rightAscensionSecond);
                                position.Declination = Math::DaaToDegrees(declinationDeg, declinationArcMinute, declinationArcSecond);
                                position.Radius = radius;

                                Core::BodyLibraryEntry entry{};
                                entry.Body = std::make_shared<Ephemeris::FixedBody>(nameBuffer.data(), designation, "Default.png", position);
                                entry.Texture = nullptr;

                                bodyLibrary->AddEntry(entry);
                            }
                            else if (selectedType == 1) {

                                // Solar System Body

                                Core::BodyLibraryEntry entry{};
                                entry.Body = std::make_shared<Ephemeris::SolarSystemBody>(nameBuffer.data(), "Default.png", elements, elementsCentury);
                                entry.Texture = nullptr;

                                bodyLibrary->AddEntry(entry);
                            }

                            clearInputs();
                            ImGui::CloseCurrentPopup();
                        }
                    }
                }

                ImGui::EndTable();
            }
            ImGui::EndPopup();
        }
    }
}