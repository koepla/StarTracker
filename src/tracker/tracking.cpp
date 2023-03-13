#include <filesystem>
#include <iostream>
#include <unordered_set>
#include <vector>
#include <sstream>

#include "arch/file.hpp"
#include "core/catalog-manager.hpp"
#include "core/location-manager.hpp"
#include "core/settings.hpp"
#include "core/tracker.hpp"
#include "tracking.hpp"
#include "ui.hpp"

namespace {

    void ExportComputeResult(std::string_view identifier,
                             const ephemeris::ComputeResult& result,
                             ephemeris::ComputeInfo info) {
        std::stringstream file;
        file << fmt::format("{};;\n", identifier);
        file << fmt::format("{};{};\n", info.Observer.Latitude, info.Observer.Longitude);
        file << fmt::format("{};{};{}\n", info.Steps, info.StepSize, DateTimeUnitToString(info.Unit));
        for (std::size_t i = 0; i < result.Azimuths.size(); ++i) {
            info.Date.Add(static_cast<std::int64_t>(i * info.StepSize), info.Unit);
            const auto azimuth = result.Azimuths[i];
            const auto altitude = result.Altitudes[i];
            const auto date = info.Date.ToString();
            file << fmt::format("{};{};{}\n", date, azimuth, altitude);
        }

        const auto path = arch::SaveFileDialog("Export ComputeResult to CSV");
        if (path.empty()) {
            LIBTRACKER_WARN("Could not export ComputeResult as the selected path was empty");
            return;
        }

        if (!arch::WriteFile(path, file.str())) {
            LIBTRACKER_ERROR("Could not export ComputeResult as writing to {} failed", path.string());
        }
    }

    constexpr auto COLOR_RED = ImVec4{ 1.0f, 0.0f, 0.0f, 1.0f };
    constexpr auto COLOR_GREEN = ImVec4{ 0.0f, 1.0f, 0.0f, 1.0f };

    void PreviewGraph(const ephemeris::ComputeResult& data, double now, const ImVec2& size) {
        const auto id = fmt::format("##plotId{}", reinterpret_cast<std::intptr_t>(&data));
        const auto count = data.Azimuths.size();
        constexpr auto axisFlags = ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoLabel | ImPlotAxisFlags_NoTickLabels;

        if (ImPlot::BeginPlot(id.c_str(), size, ImPlotFlags_NoLegend | ImPlotFlags_NoFrame)) {
            ImPlot::SetupAxis(ImAxis_X1, "Seconds", axisFlags);
            ImPlot::SetupAxis(ImAxis_Y1, "Angle", ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_Opposite);
            ImPlot::SetupAxisFormat(ImAxis_X1, "%g s");
            ImPlot::SetupAxisFormat(ImAxis_Y1, "%g deg");
            ImPlot::SetupAxesLimits(0.0, static_cast<double>(count - 1), -90.0, 360.0, ImPlotCond_Always);
            ImPlot::TagX(now, ImVec4(0.0f, 0.0f, 1.0f, 1.0f), "Now");
            ImPlot::DragLineX(0, &now, { 0.33f, 0.33f, 0.33f, 1.0f });
            ImPlot::SetNextLineStyle(COLOR_RED, 1.0f);
            ImPlot::PlotLine<double>(id.c_str(), data.Azimuths.data(), static_cast<int>(count));
            ImPlot::SetNextLineStyle(COLOR_GREEN, 1.0f);
            ImPlot::PlotLine<double>(id.c_str(), data.Altitudes.data(), static_cast<int>(count));
            ImPlot::EndPlot();
        }
    }

    template<typename PositionsFunction>
    void DrawDetailsGraph(std::string_view id, PositionsFunction&& positionsFunction) noexcept {
        const auto& style = ImGui::GetStyle();

        const auto fullSize = ImGui::GetContentRegionAvail();
        const auto imageHeight = 6.0f * ImGui::GetFontSize();

        static ephemeris::ComputeInfo info{};
        info.Observer = LocationManager::GetGeographic();

        const auto configId = fmt::format("idChildGraphConfig{}", id);
        {
            ScopedColor childBackground{ ImGuiCol_ChildBg, style.Colors[ImGuiCol_FrameBg] };
            if (ImGui::BeginChild(configId.c_str(), { imageHeight * 2.25f, fullSize.y }, false,
                                  ImGuiWindowFlags_NoScrollbar)) {

                constexpr std::size_t min = 1;
                constexpr std::size_t max = 99999;
                const auto maxAdjusted = max / info.StepSize;

                ScopedColor frameBackground{ ImGuiCol_FrameBg, style.Colors[ImGuiCol_MenuBarBg] };

                const auto advance = style.FramePadding;
                DrawCursor::Advance(0.0f, advance.y);

                if (ImGui::TreeNodeEx("Date", ImGuiTreeNodeFlags_DefaultOpen)) {
                    auto& useCurrentDateTime = Settings::Get<bool>("Details-Graph-UseCurrentDateTime");
                    ImGui::Checkbox("Use current Date/Time", &useCurrentDateTime);
                    if (useCurrentDateTime) {
                        info.Date = DateTime::Now();
                    }
                    DrawInputDateTime("Start", info.Date, useCurrentDateTime);
                    ImGui::TreePop();
                }

                if (ImGui::TreeNodeEx("Steps", ImGuiTreeNodeFlags_DefaultOpen)) {
                    ScopedWidth width{ ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Step Size").x -
                                       3.0f * advance.x };
                    ImGui::DragScalar("Steps", ImGuiDataType_U64, &info.Steps, 1, &min, &maxAdjusted, nullptr,
                                      ImGuiSliderFlags_AlwaysClamp);

                    ImGui::DragScalar("Step Size", ImGuiDataType_U64, &info.StepSize, 1, &min, &max, nullptr,
                                      ImGuiSliderFlags_AlwaysClamp);

                    auto currentValue = static_cast<int>(info.Unit);
                    ImGui::Combo("Unit", &currentValue, "Seconds\0Minutes\0Hours\0Days\0Months\0Years\0\0");
                    info.Unit = static_cast<DateTime::Unit>(currentValue);
                    ImGui::TreePop();
                }
                if (ImGui::TreeNodeEx("Export", ImGuiTreeNodeFlags_DefaultOpen)) {
                    if (ImGui::Button("Generate CSV", { ImGui::GetContentRegionAvail().x - 2.0f * advance.x, 0.0f })) {
                        const auto positions = positionsFunction(info);
                        ExportComputeResult(id, positions, info);
                    }
                    ImGui::TreePop();
                }
            }
            ImGui::EndChild();
        }


        ImGui::SameLine();
        const auto graphId = fmt::format("idChildGraph{}", id);

        {
            ScopedColor childBackground{ ImGuiCol_ChildBg, style.Colors[ImGuiCol_FrameBg] };
            if (ImGui::BeginChild(graphId.c_str(), ImGui::GetContentRegionAvail(), false,
                                  ImGuiWindowFlags_NoScrollbar)) {
                const auto positions = positionsFunction(info);

                std::vector<double> axisDescription(positions.Azimuths.size());
                for (std::size_t i = 0; i < axisDescription.size(); i++) {
                    axisDescription[i] = static_cast<double>(i * info.StepSize);
                }

                const auto availableSize = ImGui::GetContentRegionAvail();
                const auto size = ImVec2{ availableSize.x, (availableSize.y - style.ItemInnerSpacing.y) / 2.0f };
                const auto xAxisDescription = fmt::format("{} from start", DateTimeUnitToString(info.Unit));
                if (ImPlot::BeginPlot("##Azimuth", size)) {
                    ImPlot::SetupAxes(xAxisDescription.c_str(), "Angle", ImPlotAxisFlags_AutoFit,
                                      ImPlotAxisFlags_AutoFit);
                    ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);
                    ImPlot::PlotLine("azimuth(t)", axisDescription.data(), positions.Azimuths.data(),
                                     static_cast<int>(positions.Azimuths.size()));
                    ImPlot::EndPlot();
                }
                if (ImPlot::BeginPlot("##Elevation", size)) {
                    ImPlot::SetupAxes(xAxisDescription.c_str(), "Angle", ImPlotAxisFlags_AutoFit,
                                      ImPlotAxisFlags_AutoFit);
                    ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);
                    ImPlot::PlotLine("elevation(t)", axisDescription.data(), positions.Altitudes.data(),
                                     static_cast<int>(positions.Altitudes.size()));
                    ImPlot::EndPlot();
                }
            }
            ImGui::EndChild();
        }
    }

    void DrawPlanetDetails(const std::shared_ptr<ephemeris::Planet>& planet, std::string_view title) noexcept {
        const auto& texture = CatalogManager::FetchTexture(planet->Name);
        const auto& observer = LocationManager::GetGeographic();
        const auto& style = ImGui::GetStyle();
        const auto fontSize = ImGui::GetFontSize();
        const auto imageHeight = 6.0f * fontSize;
        const auto trackingDuration = Settings::Get<double>("Tracker-Duration");

        bool open = true;
        if (ImGui::BeginPopupModal(title.data(), &open, ImGuiWindowFlags_Modal)) {
            const auto tabBarId = fmt::format("idTabBar{}", planet->Name);
            if (ImGui::BeginTabBar(tabBarId.c_str())) {
                if (ImGui::BeginTabItem("Details & Tracking")) {
                    const auto date = Tracker::Handle != nullptr ? Tracker::Handle->GetBegin() : DateTime::Now();
                    {
                        ScopedColor childBackground{ ImGuiCol_ChildBg, style.Colors[ImGuiCol_FrameBg] };
                        if (ImGui::BeginChild(fmt::format("idChildElementImagePreview{}", planet->Name).c_str(),
                                              ImVec2{ ImGui::GetContentRegionAvail().x, imageHeight }, false,
                                              ImGuiWindowFlags_NoScrollbar)) {
                            const auto cursor = ImGui::GetCursorPos();
                            Image::DrawRounded(texture->GetNativeHandle(), imageHeight, imageHeight);
                            ImGui::SetCursorPos({ cursor.x + imageHeight, cursor.y });

                            // Calculate position preview
                            ephemeris::ComputeInfo info;
                            info.Observer = observer;
                            info.Date = date;
                            info.Steps = static_cast<std::size_t>(trackingDuration);
                            info.StepSize = 1;
                            info.Unit = DateTime::Unit::Seconds;
                            const auto result = ComputeGeographic(planet, info);

                            const auto progress = DateTime::Difference(date, DateTime::Now());
                            PreviewGraph(result, static_cast<double>(progress),
                                         { ImGui::GetContentRegionAvail().x, imageHeight });
                        }
                        ImGui::EndChild();
                        if (ImGui::BeginChild(fmt::format("idChildElementPosition{}", planet->Name).c_str(),
                                              { ImGui::GetContentRegionAvail().x, imageHeight }, false,
                                              ImGuiWindowFlags_NoScrollbar)) {
                            if (ImGui::BeginTable("##idTrackingInfoAlignment", 3,
                                                  ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable |
                                                          ImGuiTableFlags_SizingStretchSame,
                                                  ImGui::GetContentRegionAvail())) {
                                const auto sphericalPreview = planet->GetEquatorialPosition(DateTime::Now());
                                const auto horizontalPreview =
                                        ObserveGeographic(sphericalPreview, observer, DateTime::Now());
                                ImGui::TableNextRow();
                                ImGui::TableNextColumn();
                                {
                                    ScopedFont medium{ Font::Medium };
                                    ImGui::Text("Right Ascension");
                                }
                                ImGui::TableNextColumn();
                                {
                                    ScopedFont medium{ Font::Medium };
                                    ImGui::Text("Declination");
                                }
                                ImGui::TableNextColumn();
                                {
                                    ScopedFont medium{ Font::Medium };
                                    ImGui::Text("Distance");
                                }
                                ImGui::TableNextRow();
                                ImGui::TableNextColumn();
                                ImGui::Text("%s", fmt::format("{:.4f} deg", sphericalPreview.RightAscension).c_str());
                                ImGui::TableNextColumn();
                                ImGui::Text("%s", fmt::format("{:.4f} deg", sphericalPreview.Declination).c_str());
                                ImGui::TableNextColumn();
                                ImGui::Text("%s", fmt::format("{:.8f} au", sphericalPreview.Radius).c_str());

                                ImGui::TableNextRow();
                                ImGui::TableNextColumn();
                                {
                                    ScopedFont medium{ Font::Medium };
                                    ImGui::Text("Azimuth");
                                }
                                ImGui::TableNextColumn();
                                {
                                    ScopedFont medium{ Font::Medium };
                                    ImGui::Text("Altitude");
                                }
                                ImGui::TableNextRow();
                                ImGui::TableNextColumn();
                                ImGui::Text("%s", fmt::format("{:.4f} deg", horizontalPreview.Azimuth).c_str());
                                ImGui::TableNextColumn();
                                ImGui::Text("%s", fmt::format("{:.4f} deg", horizontalPreview.Altitude).c_str());
                                ImGui::EndTable();
                            }
                        }
                        ImGui::EndChild();
                        if (ImGui::BeginTable("##idTrackingInfoBeginAlignment", 3)) {
                            ImGui::TableNextRow();
                            ImGui::TableNextColumn();
                            {
                                ScopedFont mediumFont{ Font::Medium };
                                if (Tracker::Handle) {
                                    const auto info = fmt::format(
                                            "{}{}", TrackerStatusToString(Tracker::Handle->GetStatus()),
                                            Tracker::Handle->InProgress()
                                                    ? fmt::format("({} s)", Tracker::Handle->GetElapsedSeconds())
                                                    : "");
                                    ImGui::Text("%s", info.c_str());
                                } else {
                                    ImGui::Text("Idle");
                                }
                            }
                            ImGui::TableNextColumn();
                            ImGui::TableNextColumn();
                            const auto buttonSize = ImVec2{ ImGui::GetContentRegionAvail().x, fontSize * 1.4f };
                            if (Tracker::Handle && Tracker::Handle->InProgress()) {
                                const auto disabled = !Tracker::Handle->IsAbortable();
                                if (disabled) {
                                    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
                                    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
                                }
                                if (ImGui::Button("Abort", buttonSize)) {
                                    Tracker::Handle->Abort();
                                }
                                if (disabled) {
                                    ImGui::PopItemFlag();
                                    ImGui::PopStyleVar();
                                }
                            } else if (ImGui::Button("Track", buttonSize)) {
                                Tracker::SubmitPlanet(planet, trackingDuration);
                            }
                            ImGui::EndTable();
                        }
                    }
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Graph")) {
                    DrawDetailsGraph(planet->Name,
                                     [&, planet](ephemeris::ComputeInfo& info) -> ephemeris::ComputeResult {
                                         return ComputeGeographic(planet, info);
                                     });
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }


            ImGui::EndPopup();
        }
    }

    void DrawFixedDetails(const std::shared_ptr<ephemeris::FixedBody>& body, std::string_view title) noexcept {
        const auto& texture = CatalogManager::FetchTexture(body->Designation);
        const auto& observer = LocationManager::GetGeographic();
        auto& style = ImGui::GetStyle();
        const auto fontSize = ImGui::GetFontSize();
        const auto imageHeight = 6.0f * fontSize;
        const auto trackingDuration = Settings::Get<double>("Tracker-Duration");

        bool open = true;
        if (ImGui::BeginPopupModal(title.data(), &open, ImGuiWindowFlags_Modal)) {
            const auto tabBarId = fmt::format("idTabBar{}", body->Designation);
            if (ImGui::BeginTabBar(tabBarId.c_str())) {
                if (ImGui::BeginTabItem("Details & Tracking")) {
                    const auto date = Tracker::Handle != nullptr ? Tracker::Handle->GetBegin() : DateTime::Now();
                    {
                        ScopedColor childBackground{ ImGuiCol_ChildBg, style.Colors[ImGuiCol_FrameBg] };
                        if (ImGui::BeginChild(fmt::format("idChildElementImagePreview{}", body->Designation).c_str(),
                                              ImVec2{ ImGui::GetContentRegionAvail().x, imageHeight }, false,
                                              ImGuiWindowFlags_NoScrollbar)) {
                            const auto cursor = ImGui::GetCursorPos();
                            Image::DrawRounded(texture->GetNativeHandle(), imageHeight, imageHeight);
                            ImGui::SetCursorPos({ cursor.x + imageHeight, cursor.y });

                            // Calculate position preview
                            ephemeris::ComputeInfo info;
                            info.Observer = observer;
                            info.Date = date;
                            info.Steps = static_cast<std::size_t>(trackingDuration);
                            info.StepSize = 1;
                            info.Unit = DateTime::Unit::Seconds;
                            const auto result = ComputeGeographic(body, info);

                            const auto progress = DateTime::Difference(date, DateTime::Now());
                            PreviewGraph(result, static_cast<double>(progress),
                                         { ImGui::GetContentRegionAvail().x, imageHeight });
                        }
                        ImGui::EndChild();
                        if (ImGui::BeginChild(
                                    fmt::format("idChildElementPosition{}", body->Designation).c_str(),
                                    { ImGui::GetContentRegionAvail().x, 7.0f * (fontSize + style.ItemInnerSpacing.y) },
                                    false, ImGuiWindowFlags_NoScrollbar)) {
                            if (ImGui::BeginTable("##idTrackingInfoAlignment", 3,
                                                  ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable |
                                                          ImGuiTableFlags_SizingStretchSame,
                                                  ImGui::GetContentRegionAvail())) {
                                const auto now = DateTime::Now();
                                const auto equatorialPreview = body->GetEquatorialPosition(now);
                                const auto horizontalPreview = ObserveGeographic(equatorialPreview, observer, now);
                                ImGui::TableNextRow();
                                ImGui::TableNextColumn();
                                {
                                    ScopedFont medium{ Font::Medium };
                                    ImGui::Text("Right Ascension");
                                }
                                ImGui::TableNextColumn();
                                {
                                    ScopedFont medium{ Font::Medium };
                                    ImGui::Text("Declination");
                                }
                                ImGui::TableNextColumn();
                                {
                                    ScopedFont medium{ Font::Medium };
                                    ImGui::Text("Distance");
                                }
                                ImGui::TableNextRow();
                                ImGui::TableNextColumn();
                                ImGui::Text("%s", fmt::format("{:.4f} deg", equatorialPreview.RightAscension).c_str());
                                ImGui::TableNextColumn();
                                ImGui::Text("%s", fmt::format("{:.4f} deg", equatorialPreview.Declination).c_str());
                                ImGui::TableNextColumn();
                                ImGui::Text("%s", fmt::format("{:.8f} au", equatorialPreview.Radius).c_str());

                                ImGui::TableNextRow();
                                ImGui::TableNextColumn();
                                {
                                    ScopedFont medium{ Font::Medium };
                                    ImGui::Text("Azimuth");
                                }
                                ImGui::TableNextColumn();
                                {
                                    ScopedFont medium{ Font::Medium };
                                    ImGui::Text("Altitude");
                                }
                                ImGui::TableNextRow();
                                ImGui::TableNextColumn();
                                ImGui::Text("%s", fmt::format("{:.4f} deg", horizontalPreview.Azimuth).c_str());
                                ImGui::TableNextColumn();
                                ImGui::Text("%s", fmt::format("{:.4f} deg", horizontalPreview.Altitude).c_str());

                                ImGui::TableNextRow();
                                ImGui::TableNextColumn();
                                {
                                    ScopedFont medium{ Font::Medium };
                                    ImGui::Text("Constellation");
                                }
                                ImGui::TableNextColumn();
                                {
                                    ScopedFont medium{ Font::Medium };
                                    ImGui::Text("Magnitude");
                                }
                                ImGui::TableNextColumn();
                                {
                                    ScopedFont medium{ Font::Medium };
                                    ImGui::Text("Dimension");
                                }
                                ImGui::TableNextRow();
                                ImGui::TableNextColumn();
                                ImGui::Text("%s", body->Const.Name.c_str());
                                ImGui::TableNextColumn();
                                ImGui::Text("%s", fmt::format("{:.4f}", body->Magnitude).c_str());
                                ImGui::TableNextColumn();
                                ImGui::Text("%s", fmt::format("{:.4f} '", body->Dimension).c_str());

                                ImGui::EndTable();
                            }
                        }
                        ImGui::EndChild();
                        if (ImGui::BeginTable("##idTrackingInfoBeginAlignment", 3)) {
                            ImGui::TableNextRow();
                            ImGui::TableNextColumn();
                            {
                                ScopedFont mediumFont{ Font::Medium };
                                if (Tracker::Handle) {
                                    const auto info = fmt::format(
                                            "{}{}", TrackerStatusToString(Tracker::Handle->GetStatus()),
                                            Tracker::Handle->InProgress()
                                                    ? fmt::format("({} s)", Tracker::Handle->GetElapsedSeconds())
                                                    : "");
                                    ImGui::Text("%s", info.c_str());
                                } else {
                                    ImGui::Text("Idle");
                                }
                            }
                            ImGui::TableNextColumn();
                            ImGui::TableNextColumn();
                            const auto buttonSize = ImVec2{ ImGui::GetContentRegionAvail().x, fontSize * 1.4f };
                            if (Tracker::Handle && Tracker::Handle->InProgress()) {
                                const auto disabled = !Tracker::Handle->IsAbortable();
                                if (disabled) {
                                    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
                                    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
                                }
                                if (ImGui::Button("Abort", buttonSize)) {
                                    Tracker::Handle->Abort();
                                }
                                if (disabled) {
                                    ImGui::PopItemFlag();
                                    ImGui::PopStyleVar();
                                }
                            } else if (ImGui::Button("Track", buttonSize)) {
                                Tracker::SubmitFixed(body, trackingDuration);
                            }
                            ImGui::EndTable();
                        }
                    }
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Graph")) {
                    DrawDetailsGraph(body->Designation,
                                     [&, body](ephemeris::ComputeInfo& info) -> ephemeris::ComputeResult {
                                         return ComputeGeographic(body, info);
                                     });
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
            ImGui::EndPopup();
        }
    }

    bool DrawPlanetInfoCard(const std::shared_ptr<ephemeris::Planet>& planet, const glm::vec2& size) noexcept {
        bool selected = false;

        {
            const auto& style = ImGui::GetStyle();
            ScopedColor childBackground{ ImGuiCol_ChildBg, style.Colors[ImGuiCol_FrameBg] };
            if (ImGui::BeginChild(fmt::format("idChildElement{}", planet->Name).c_str(), ImVec2{ size.x, size.y },
                                  false, ImGuiWindowFlags_NoScrollbar)) {

                // Item Spacings
                const auto itemSpacing = style.ItemSpacing;
                const auto itemInnerSpacing = style.ItemInnerSpacing;
                const auto regulatedItemSpacing = 0.7f * itemInnerSpacing.x;

                // Text Colors
                const auto& baseTextColor = style.Colors[ImGuiCol_Text];
                const auto& baseTextLightColor =
                        ImVec4{ baseTextColor.x, baseTextColor.y, baseTextColor.z, 0.5f * baseTextColor.w };
                // Font Sizes
                const auto fontSize = ImGui::GetFontSize();
                const auto smallFontSize = fontSize - 2.0f;

                const auto cursor = ImGui::GetCursorPos();
                {
                    const auto trackingId = fmt::format("Tracking {}", planet->Name);
                    ScopedID selectable{ fmt::format("idSelectable{}", planet->Name) };
                    ScopedColor headerActive{ ImGuiCol_HeaderActive, style.Colors[ImGuiCol_FrameBgActive] };
                    ScopedColor headerHovered{ ImGuiCol_HeaderHovered, style.Colors[ImGuiCol_FrameBgHovered] };
                    if (ImGui::Selectable("", false, ImGuiSelectableFlags_None, { size.x, size.y })) {
                        selected = true;
                    }
                }
                ImGui::SetCursorPos(cursor);

                if (const auto texture = CatalogManager::FetchTexture(planet->Name)) {
                    Image::DrawRounded(texture->GetNativeHandle(), size.y, size.y);
                }

                DrawCursor::Advance(size.y + itemSpacing.x, 0.0f);
                Text::Draw(planet->Name, Font::Medium, fontSize, baseTextColor);

                DrawCursor::Advance(0.0f, fontSize + regulatedItemSpacing);
                Text::Draw("No Designation", Font::Regular, smallFontSize, baseTextLightColor);

                // Compute the position preview
                const auto now = DateTime::Now();
                const auto spherical = planet->GetEquatorialPosition(now);
                const auto positionPreview = ObserveGeographic(spherical, LocationManager::GetGeographic(), now);

                // Azimuth-Angle of the Celestial Body
                const auto azimuthText = fmt::format("Azimuth: {:.4f} deg", positionPreview.Azimuth);
                DrawCursor::Advance(0.0f, smallFontSize + regulatedItemSpacing);
                Text::Draw(azimuthText, Font::Regular, smallFontSize, baseTextLightColor);

                // Altitude-Angle of the Celestial Body
                const auto elevationText = fmt::format("Altitude: {:.4f} deg", positionPreview.Altitude);
                DrawCursor::Advance(0.0f, smallFontSize + regulatedItemSpacing);
                Text::Draw(elevationText, Font::Regular, smallFontSize, baseTextLightColor);
            }
            ImGui::EndChild();
        }
        return selected;
    }

    bool DrawFixedBodyInfoCard(const std::shared_ptr<ephemeris::FixedBody>& body, const glm::vec2& size) noexcept {
        bool selected = false;
        const auto columnDistance = ImGui::CalcTextSize("#################################").x;

        {
            const auto& style = ImGui::GetStyle();
            ScopedColor childBackground{ ImGuiCol_ChildBg, style.Colors[ImGuiCol_FrameBg] };
            if (ImGui::BeginChild(fmt::format("idChildElement{}", body->Designation).c_str(), ImVec2{ size.x, size.y },
                                  false, ImGuiWindowFlags_NoScrollbar)) {

                // Item Spacings
                const auto itemSpacing = style.ItemSpacing;
                const auto itemInnerSpacing = style.ItemInnerSpacing;
                const auto regulatedItemSpacing = 0.7f * itemInnerSpacing.x;

                // Text Colors
                const auto& baseTextColor = style.Colors[ImGuiCol_Text];
                const auto& baseTextLightColor =
                        ImVec4{ baseTextColor.x, baseTextColor.y, baseTextColor.z, 0.5f * baseTextColor.w };
                // Font Sizes
                const auto fontSize = ImGui::GetFontSize();
                const auto smallFontSize = fontSize - 2.0f;


                if (const auto texture = CatalogManager::FetchTexture(body->Designation)) {
                    Image::DrawRounded(texture->GetNativeHandle(), size.y, size.y);
                }
                DrawCursor::Advance(size.y + itemSpacing.x, 0.0f);
                const auto selectableCursor = ImGui::GetCursorPos();

                const auto name = body->Name.empty() ? body->Designation : body->Name;
                Text::Draw(name, Font::Medium, fontSize, baseTextColor);

                DrawCursor::Advance(0.0f, fontSize + regulatedItemSpacing);
                auto cursor = ImGui::GetCursorPos();
                const auto designationText = fmt::format("Designation: {}", body->Designation);
                Text::Draw(designationText, Font::Regular, smallFontSize, baseTextLightColor);

                // Compute the position preview
                const auto now = DateTime::Now();
                const auto equatorial = body->GetEquatorialPosition(now);
                const auto positionPreview = ObserveGeographic(equatorial, LocationManager::GetGeographic(), now);

                // Azimuth-Angle of the Celestial Body
                const auto azimuthText = fmt::format("Azimuth: {:.4f} deg", positionPreview.Azimuth);
                DrawCursor::Advance(0.0f, smallFontSize + regulatedItemSpacing);
                Text::Draw(azimuthText, Font::Regular, smallFontSize, baseTextLightColor);

                // Altitude-Angle of the Celestial Body
                const auto elevationText = fmt::format("Altitude: {:.4f} deg", positionPreview.Altitude);
                DrawCursor::Advance(0.0f, smallFontSize + regulatedItemSpacing);
                Text::Draw(elevationText, Font::Regular, smallFontSize, baseTextLightColor);

                // Advance to the next column
                ImGui::SetCursorPos(cursor);
                DrawCursor::Advance(columnDistance, 0.0f);
                cursor = ImGui::GetCursorPos();

                // Classification
                const auto classificationText = fmt::format("Classification: {}", ClassificationToString(body->Type));
                Text::Draw(classificationText, Font::Regular, smallFontSize, baseTextLightColor);

                // Constellation
                const auto constellationText =
                        fmt::format("Constellation: {} ({})", body->Const.Name, body->Const.Abbreviation);
                DrawCursor::Advance(0.0f, smallFontSize + regulatedItemSpacing);
                Text::Draw(constellationText, Font::Regular, smallFontSize, baseTextLightColor);

                // Magnitude
                const auto magnitudeText = fmt::format("Magnitude: {}", body->Magnitude);
                DrawCursor::Advance(0.0f, smallFontSize + regulatedItemSpacing);
                Text::Draw(magnitudeText, Font::Regular, smallFontSize, baseTextLightColor);

                // Dimension
                ImGui::SetCursorPos(cursor);
                DrawCursor::Advance(columnDistance, 0.0f);
                const auto dimensionText = fmt::format("Dimension: {} '", body->Dimension);
                Text::Draw(dimensionText, Font::Regular, smallFontSize, baseTextLightColor);

                DrawCursor::Advance(0.0f, smallFontSize + regulatedItemSpacing);
                const auto id = fmt::format("idBodyDesc{}", body->Designation);
                if (ImGui::BeginChild(id.c_str(), ImGui::GetContentRegionAvail())) {
                    ImGui::TextWrapped("%s", body->Description.c_str());
                }
                ImGui::EndChild();

                ImGui::SetCursorPos(selectableCursor);
                {
                    ScopedID selectable{ fmt::format("idSelectable{}", body->Designation) };
                    ScopedColor headerActive{ ImGuiCol_HeaderActive, style.Colors[ImGuiCol_FrameBgActive] };
                    ScopedColor headerHovered{ ImGuiCol_HeaderHovered, style.Colors[ImGuiCol_FrameBgHovered] };
                    if (ImGui::Selectable("", false, ImGuiSelectableFlags_None, { size.x, size.y })) {
                        selected = true;
                    }
                }
            }
            ImGui::EndChild();
        }
        return selected;
    }
}// namespace

namespace Internal {

    void DrawInfoHeader() noexcept {
        // Info UI
        const auto& style = ImGui::GetStyle();
        const auto itemSpacing = style.ItemSpacing;
        const auto fontSize = ImGui::GetFontSize();
        const auto trackerInfoCardHeight = 3.0f * fontSize + (2.0f + 2.0f * 0.7f) * itemSpacing.y - 4.0f;
        auto width = ImGui::GetContentRegionAvail().x * 0.5f - itemSpacing.x * 0.5f;
        DrawTrackerInfoCard(width, trackerInfoCardHeight);
        ImGui::SameLine();

        width = width * 0.5f - itemSpacing.x * 0.5f;
        DrawTrackingDurationCard(width, trackerInfoCardHeight);
        ImGui::SameLine();
        DrawAngularSpeedCard(width, trackerInfoCardHeight);
    }

    void DrawTrackerInfoCard(f32 width, f32 height) noexcept {
        const auto& style = ImGui::GetStyle();

        ScopedColor childBackground{ ImGuiCol_ChildBg, style.Colors[ImGuiCol_FrameBg] };
        if (ImGui::BeginChild("idChildTrackerInfoCard", { width, height }, false, ImGuiWindowFlags_NoScrollbar)) {
            // Initial Cursor Position
            const auto initialCursor = ImGui::GetCursorPos();

            // Item Spacings
            const auto itemInnerSpacing = style.ItemInnerSpacing;
            const auto itemSpacing = style.ItemSpacing;
            const auto regulatedItemSpacing = 0.7f * itemInnerSpacing.x;

            // Text Colors
            const auto& baseTextColor = style.Colors[ImGuiCol_Text];
            const auto& baseTextLightColor =
                    ImVec4{ baseTextColor.x, baseTextColor.y, baseTextColor.z, 0.5f * baseTextColor.w };

            // Font Sizes
            const auto fontSize = ImGui::GetFontSize();
            const auto smallFontSize = fontSize - 2.0f;

            // Current Date and Time
            const auto now = DateTime::Now();
            DrawCursor::Advance(itemInnerSpacing.x, 0.0f);
            Text::Draw(now.ToString().c_str(), Font::Medium, fontSize, baseTextColor);

            // Status of the Tracker
            const auto trackerConnectionInfo =
                    fmt::format("{}", Tracker::IsConnected() ? "Connected" : "Not Connected");
            const auto trackerStatusTextWidth = ImGui::CalcTextSize("Tracker:").x;
            const auto trackerStatusTextColor = std::invoke([&]() -> ImVec4 {
                if (Tracker::IsConnected()) {
                    return ImVec4{ 0.25f, 0.85f, 0.15f, 1.0f };
                }
                return ImVec4{ 1.0f, 0.0f, 0.0f, 1.0f };
            });
            DrawCursor::Advance(0.0f, fontSize + regulatedItemSpacing);
            Text::Draw("Tracker:", Font::Regular, smallFontSize, baseTextLightColor);
            DrawCursor::Advance(trackerStatusTextWidth, 0.0f);
            Text::Draw(trackerConnectionInfo.c_str(), Font::Regular, smallFontSize, trackerStatusTextColor);
            DrawCursor::Advance(-1.0f * trackerStatusTextWidth, 0.0f);

            DrawCursor::Advance(0.0f, smallFontSize + regulatedItemSpacing);
            Text::Draw(LocationManager::GetFormatted().c_str(), Font::Regular, smallFontSize, baseTextLightColor);

            const auto reconnectButtonWidth = ImGui::CalcTextSize("Reconnect").x * 2.5f;
            ImGui::SetCursorPos(initialCursor);
            DrawCursor::Advance(width - reconnectButtonWidth - itemInnerSpacing.x, itemSpacing.y);

            if (ImGui::Button("Quick Connect", { reconnectButtonWidth, height - 2.0f * itemSpacing.y })) {
                Tracker::Connect();
            }
        }
        ImGui::EndChild();
    }

    void DrawTrackingDurationCard(f32 width, f32 height) noexcept {
        const auto& style = ImGui::GetStyle();
        ScopedColor childBackground{ ImGuiCol_ChildBg, style.Colors[ImGuiCol_FrameBg] };
        if (ImGui::BeginChild("idChildTrackingDurationCard", { width, height }, false, ImGuiWindowFlags_NoScrollbar)) {
            // Item Spacings
            const auto itemInnerSpacing = style.ItemInnerSpacing;
            const auto regulatedItemSpacing = 0.7f * itemInnerSpacing.x;
            const auto& baseTextColor = style.Colors[ImGuiCol_Text];
            const auto fontSize = ImGui::GetFontSize();

            DrawCursor::Advance(itemInnerSpacing.x, 0.0f);
            Text::Draw("Tracking Duration", Font::Medium, fontSize, baseTextColor);

            DrawCursor::Advance(0.0f, fontSize + regulatedItemSpacing);
            DrawDragDouble(Settings::Get<double>("Tracker-Duration", 600.0), width, height, "%f Seconds", 1.0, 1.0,
                           3600.0);
        }
        ImGui::EndChild();
    }

    void DrawAngularSpeedCard(f32 width, f32 height) noexcept {
        const auto& style = ImGui::GetStyle();
        ScopedColor childBackground{ ImGuiCol_ChildBg, style.Colors[ImGuiCol_FrameBg] };
        if (ImGui::BeginChild("idChildAngularSpeedCard", { width, height }, false, ImGuiWindowFlags_NoScrollbar)) {
            const auto itemInnerSpacing = style.ItemInnerSpacing;
            const auto regulatedItemSpacing = 0.7f * itemInnerSpacing.x;
            const auto& baseTextColor = style.Colors[ImGuiCol_Text];
            const auto fontSize = ImGui::GetFontSize();

            DrawCursor::Advance(itemInnerSpacing.x, 0.0f);
            Text::Draw("Angular Speed", Font::Medium, fontSize, baseTextColor);

            DrawCursor::Advance(0.0f, fontSize + regulatedItemSpacing);
            DrawDragDouble(Settings::Get<double>("Tracker-AngularSpeed", 0.5), width, height, "%f Degrees/Second", 1.0,
                           1.0, 3600.0);
        }
        ImGui::EndChild();
    }

}// namespace Internal

Tracking::Tracking(void* windowHandle) noexcept
    : View{ windowHandle },
      bodyRenderStartIndex(0),
      bodyRenderEndIndex(0) { }

void Tracking::OnInit() noexcept { }

void Tracking::OnUpdate(float deltaTime) noexcept {
    if (ImGui::Begin("Tracking")) {
        const auto& style = ImGui::GetStyle();
        const auto itemSpacing = style.ItemSpacing;
        const auto fontSize = ImGui::GetFontSize();
        const auto filterWidth =
                (ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Clear Filters").x - itemSpacing.x * 5.0f) /
                4.0f;

        Internal::DrawInfoHeader();

        // Filter Search Box
        constexpr std::size_t searchBufferSize = 128;
        static std::vector<char> searchBuffer(searchBufferSize);
        {
            ScopedWidth inputTextWidth{ filterWidth };
            ImGui::InputTextWithHint("##idBodySearchEngine", "Enter name or designation", searchBuffer.data(),
                                     searchBufferSize);
        }

        // Classification
        ImGui::SameLine();

        std::unordered_set<ephemeris::Classification> classificationFilter{};
        static std::vector<bool> classificationSelection(static_cast<std::size_t>(ephemeris::Classification::Planet) +
                                                         1);

        {
            ScopedWidth width{ filterWidth };
            if (ImGui::BeginCombo("##idClassification", "Classification")) {


                const auto buttonWidth = ImGui::GetContentRegionAvail().x * 0.5f - itemSpacing.x;
                if (ImGui::Button("Select All", { buttonWidth, 0 })) {
                    for (auto&& value : classificationSelection) {
                        value = true;
                    }
                }
                ImGui::SameLine();
                if (ImGui::Button("Clear", { buttonWidth, 0 })) {
                    for (auto&& value : classificationSelection) {
                        value = false;
                    }
                }

                for (std::size_t i = 0; i <= static_cast<std::size_t>(ephemeris::Classification::Planet); ++i) {
                    const auto classification = static_cast<ephemeris::Classification>(i);
                    bool selected = classificationSelection[i];
                    if (ImGui::Checkbox(ClassificationToString(classification), &selected)) {
                        classificationSelection[i] = selected;
                    }
                }
                ImGui::EndCombo();
            }
        }

        for (std::size_t i = 0; i < classificationSelection.size(); ++i) {
            const auto classification = static_cast<ephemeris::Classification>(i);
            if (classificationSelection[i]) {
                classificationFilter.insert(classification);
            }
        }

        // Constellation
        ImGui::SameLine();

        std::unordered_set<std::string_view> constellationFilter{};
        static std::vector<bool> constellationSelection(ephemeris::ConstellationExpansionTable.size());

        {
            ScopedWidth width{ filterWidth };
            if (ImGui::BeginCombo("##idConstellation", "Constellation")) {


                const auto buttonWidth = ImGui::GetContentRegionAvail().x * 0.5f - itemSpacing.x;
                if (ImGui::Button("Select All", { buttonWidth, 0 })) {
                    for (auto&& value : constellationSelection) {
                        value = true;
                    }
                }
                ImGui::SameLine();
                if (ImGui::Button("Clear", { buttonWidth, 0 })) {
                    for (auto&& value : constellationSelection) {
                        value = false;
                    }
                }

                for (std::size_t i = 0; i < ephemeris::ConstellationExpansionTable.size(); ++i) {
                    auto iterator = ephemeris::ConstellationExpansionTable.begin();
                    std::advance(iterator, i);

                    const auto& [abbreviation, name] = *iterator;
                    bool selected = constellationSelection[i];
                    if (ImGui::Checkbox(name.data(), &selected)) {
                        constellationSelection[i] = selected;
                    }
                }
                ImGui::EndCombo();
            }
        }

        for (std::size_t i = 0; i < constellationSelection.size(); ++i) {
            auto iterator = ephemeris::ConstellationExpansionTable.begin();
            std::advance(iterator, i);

            const auto& [abbreviation, name] = *iterator;
            if (constellationSelection[i]) {
                constellationFilter.insert(abbreviation);
            }
        }

        ImGui::SameLine();

        // Advanced Filters
        static bool visibilitySelection = false;
        {
            ScopedWidth width{ filterWidth };
            if (ImGui::BeginCombo("##idAdvanced", "Advanced Options")) {
                if (ImGui::TreeNode("Visibility")) {
                    if (ImGui::Checkbox("Require minimum altitude", &visibilitySelection)) { }
                    DrawInputDouble(Settings::Get<double>("Catalog-VisibilityThreshold"), "%.2f deg",
                                    !visibilitySelection);
                    ImGui::TreePop();
                }

                ImGui::EndCombo();
            }
        }

        ImGui::SameLine();

        std::optional<ephemeris::Catalog::VisibilityFilter> visibilityFilter;
        if (visibilitySelection) {
            visibilityFilter =
                    ephemeris::Catalog::VisibilityFilter{ Settings::Get<double>("Catalog-VisibilityThreshold"),
                                                          LocationManager::GetGeographic() };
        }

        if (ImGui::Button("Clear Filters", { ImGui::GetContentRegionAvail().x, 0.0f })) {
            visibilitySelection = false;
            for (auto&& value : constellationSelection) {
                value = false;
            }
            for (auto&& value : classificationSelection) {
                value = false;
            }
            std::memset(searchBuffer.data(), 0, searchBuffer.size());
        }

        // Get the filtered library
        static ephemeris::Catalog::Filter lastFilter{ "huygens", {}, {}, {} };
        static std::vector<std::shared_ptr<ephemeris::FixedBody>> bodies{};
        static std::vector<std::shared_ptr<ephemeris::Planet>> planets{};
        auto filter = ephemeris::Catalog::Filter{ searchBuffer.data(), classificationFilter,
                                                  std::move(constellationFilter), visibilityFilter };


        if (filter != lastFilter) {
            if (Settings::Get<bool>("Output-Verbose")) {
                LIBTRACKER_WARN("Rebuilding catalog");
            }
            const auto& catalog = CatalogManager::GetCatalog();
            bodies = catalog.FilterFixed(filter);
            if (classificationFilter.empty() ||
                classificationFilter.find(ephemeris::Classification::Planet) != classificationFilter.end()) {
                planets = catalog.FilterPlanets(filter.Identifier);
            } else {
                planets.clear();
            }
        }
        lastFilter = filter;

        const auto size = ImGui::GetContentRegionAvail();
        if (ImGui::BeginChild("idChildCelestialBodiesList", { size.x, size.y - fontSize - itemSpacing.y }, false,
                              ImGuiWindowFlags_AlwaysVerticalScrollbar)) {
            if (ImGui::BeginTable("Targets", 1)) {
                for (const auto& planet : planets) {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();

                    const auto celestialBodyCardHeight = 4.0f * fontSize + (2.0f + 3 * 0.7f) * itemSpacing.y - 6.0f;

                    if (DrawPlanetInfoCard(planet, { ImGui::GetContentRegionAvail().x, celestialBodyCardHeight })) {
                        ImGui::OpenPopup(planet->Name.c_str());
                    }
                    DrawPlanetDetails(planet, planet->Name.c_str());
                }

                ImGuiListClipper bodyClipper;
                bodyClipper.Begin(static_cast<int>(bodies.size()));
                while (bodyClipper.Step()) {
                    for (auto row = bodyClipper.DisplayStart; row < bodyClipper.DisplayEnd; ++row) {
                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();

                        const auto& body = bodies[row];
                        const auto celestialBodyCardHeight = 4.0f * fontSize + (2.0f + 3 * 0.7f) * itemSpacing.y - 6.0f;
                        if (DrawFixedBodyInfoCard(body,
                                                  { ImGui::GetContentRegionAvail().x, celestialBodyCardHeight })) {
                            ImGui::OpenPopup(body->Designation.c_str());
                        }
                        DrawFixedDetails(body, body->Designation);
                    }
                }
                bodyRenderStartIndex = bodyClipper.DisplayStart;
                bodyRenderEndIndex = bodyClipper.DisplayEnd;
                ImGui::EndTable();
            }
        }
        ImGui::EndChild();
        ImGui::Text("Number of targets: %d (%d:%d)", static_cast<int>(planets.size() + bodies.size()),
                    bodyRenderStartIndex, bodyRenderEndIndex);
    }
    ImGui::End();
}

void Tracking::OnDestroy() noexcept {
    LIBTRACKER_ASSERT(Tracker::Disconnect(), "Failed to disconnect from tracker!");
}