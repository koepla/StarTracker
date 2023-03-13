#include "control.hpp"
#include "tracking.hpp"

namespace {

    void DrawConfiguration(f32 width, f32 height) noexcept {
        const auto& style = ImGui::GetStyle();
        const auto& baseTextColor = style.Colors[ImGuiCol_Text];
        const auto& baseTextLightColor =
                baseTextColor;// ImVec4{ baseTextColor.x, baseTextColor.y, baseTextColor.z, 0.5f * baseTextColor.w };
        const auto fontSize = ImGui::GetFontSize();
        if (ImGui::BeginChild("##idChildTrackerConfig", { width, height }, false)) {
            DrawCursor::Advance(style.ItemInnerSpacing.x, 0.0f);
            Text::Draw("Config", Font::Medium, fontSize, baseTextColor);
            DrawCursor::Advance(0.0f, fontSize + style.ItemSpacing.y);
            const auto maxSize = ImGui::GetContentRegionAvail();
            if (ImGui::BeginTable("##idTrackerConfigAlignment", 2, ImGuiTableFlags_None,
                                  { maxSize.x - style.ItemInnerSpacing.x, maxSize.y })) {
                ScopedColor frameBackground{ ImGuiCol_FrameBg, style.Colors[ImGuiCol_WindowBg] };
                static ephemeris::Horizontal target{ 0.0, 0.0 };
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                DrawCursor::Advance(style.ItemInnerSpacing.x, 0.0f);
                ImGui::Text("Altitude");
                ImGui::TableNextColumn();
                DrawInputDouble(target.Altitude, "%f Degrees");

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                DrawCursor::Advance(style.ItemInnerSpacing.x, 0.0f);
                ImGui::Text("Azimuth");
                ImGui::TableNextColumn();
                DrawInputDouble(target.Azimuth, "%f Degrees");

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                DrawCursor::Advance(style.ItemInnerSpacing.x, 0.0f);
                ImGui::Text("Status");
                ImGui::TableNextColumn();
                const auto text = std::invoke([&]() -> std::string {
                    if (Tracker::Handle) {
                        const auto status = TrackerStatusToString(Tracker::Handle->GetStatus());
                        if (Tracker::Handle->InProgress()) {
                            return fmt::format("{} ({} s)", status, Tracker::Handle->GetElapsedSeconds());
                        }
                        return status;
                    }
                    return "No Information";
                });
                ImGui::Text("%s", text.c_str());

                ImGui::TableNextRow();
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
                } else {
                    if (ImGui::Button("Submit", buttonSize)) {
                        Tracker::Submit(target);
                    }
                }

                ImGui::TableNextRow();
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                DrawCursor::Advance(style.ItemInnerSpacing.x, 0.0f);
                ImGui::Text("Current");
                ImGui::TableNextColumn();
                DrawInputDouble(Settings::Get<double>("Tracker-RmsCurrent", 600.0), "%f mA");

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                DrawCursor::Advance(style.ItemInnerSpacing.x, 0.0f);
                ImGui::Text("Gear Ratio");
                ImGui::TableNextColumn();
                DrawInputDouble(Settings::Get<double>("Tracker-GearRatio", 1.0), "%f");


                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                DrawCursor::Advance(style.ItemInnerSpacing.x, 0.0f);
                ImGui::Text("Micro Steps");
                ImGui::TableNextColumn();
                DrawInputInt(Settings::Get<std::size_t>("Tracker-MicroSteps", 256), "%d");


                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                DrawCursor::Advance(style.ItemInnerSpacing.x, 0.0f);
                ImGui::Text("Status");
                ImGui::TableNextColumn();
                static std::string configureStatus = "No Information";
                ImGui::Text("%s", configureStatus.c_str());


                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::TableNextColumn();
                if (ImGui::Button("Configure", buttonSize)) {
                    if (!Tracker::UpdateConfig()) {
                        configureStatus = "Failed to update";
                    } else {
                        configureStatus = "Update successful";
                    }
                }
                ImGui::EndTable();
            }
        }
        ImGui::EndChild();
    }

    void DrawSteering(f32 width, f32 height) noexcept {
        const auto& style = ImGui::GetStyle();
        const auto fontSize = ImGui::GetFontSize();
        const auto& baseTextColor = style.Colors[ImGuiCol_Text];

        if (ImGui::BeginChild("##idChildSteering", { width, height })) {
            if (ImGui::BeginChild("##idChildSteeringHeader", { 0.0f, fontSize })) {
                DrawCursor::Advance(style.ItemInnerSpacing.x, 0.0f);
                Text::Draw("Steering", Font::Medium, fontSize, baseTextColor);
            }
            ImGui::EndChild();

            static bool steerLeft = false;
            static bool steerRight = false;
            static bool steerUp = false;
            static bool steerDown = false;

            const auto maxSize = ImGui::GetContentRegionAvail();
            if (ImGui::BeginTable("##idChildConsoleAlignment", 5, ImGuiTableFlags_None,
                                  { maxSize.x - style.ItemInnerSpacing.x, maxSize.y })) {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                DrawCursor::Advance(style.ItemInnerSpacing.x, 0.0f);

                auto buttonSize = ImGui::GetContentRegionAvail();
                buttonSize.y -= style.FramePadding.y;
                if (Tracker::Handle && Tracker::Handle->InProgress()) {
                    const auto disabled = !Tracker::Handle->IsAbortable();
                    if (disabled) {
                        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
                        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
                    }
                    if (ImGui::Button("Stop", buttonSize)) {
                        Tracker::Handle->Abort();
                    }
                    if (disabled) {
                        ImGui::PopItemFlag();
                        ImGui::PopStyleVar();
                    }
                } else {
                    if (ImGui::Button("Start", buttonSize)) {
                        Tracker::SubmitSteering({ &steerLeft, &steerRight, &steerUp, &steerDown });
                    }
                }

                ImGui::TableNextColumn();
                DrawCursor::Advance(style.ItemInnerSpacing.x, 0.0f);
                steerLeft = ImGui::Button("Left", buttonSize);
                ImGui::TableNextColumn();
                DrawCursor::Advance(style.ItemInnerSpacing.x, 0.0f);
                steerRight = ImGui::Button("Right", buttonSize);
                ImGui::TableNextColumn();
                DrawCursor::Advance(style.ItemInnerSpacing.x, 0.0f);
                steerUp = ImGui::Button("Up", buttonSize);
                ImGui::TableNextColumn();
                DrawCursor::Advance(style.ItemInnerSpacing.x, 0.0f);
                steerDown = ImGui::Button("Down", buttonSize);
                ImGui::EndTable();
            }
        }
        ImGui::EndChild();
    }

    void DrawConsole(f32 width, f32 height) noexcept {
        const auto& style = ImGui::GetStyle();
        const auto fontSize = ImGui::GetFontSize();
        const auto& baseTextColor = style.Colors[ImGuiCol_Text];

        if (ImGui::BeginChild("##idChildConsole", { width, height })) {
            if (ImGui::BeginChild("##idChildConsoleHeader", { 0.0f, fontSize })) {
                DrawCursor::Advance(style.ItemInnerSpacing.x, 0.0f);
                Text::Draw("Log Console", Font::Medium, fontSize, baseTextColor);
            }
            ImGui::EndChild();

            const auto maxSize = ImGui::GetContentRegionAvail();
            if (ImGui::BeginTable("##idChildConsoleAlignment", 2,
                                  ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingStretchSame,
                                  { maxSize.x - style.ItemInnerSpacing.x, maxSize.y })) {
                const auto& log = Logger::GetLog();
                const auto reverseLog = std::vector<Logger::Entry>(log.rbegin(), log.rend());
                for (const auto& entry : reverseLog) {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    const auto childId = fmt::format(
                            "##idLogInfo{}", static_cast<int>(reinterpret_cast<std::intptr_t>(entry.Message.c_str())));
                    switch (entry.Sev) {
                        case Logger::Severity::Info: {
                            ScopedColor color{ ImGuiCol_Text, { 0.0f, 0.75f, 0.0f, 1.0f } };
                            DrawCursor::Advance(style.ItemInnerSpacing.x, 0.0f);
                            ImGui::Text("INFO");
                            break;
                        }
                        case Logger::Severity::Warn: {
                            ScopedColor color{ ImGuiCol_Text, { 1.0f, 0.75f, 0.0f, 1.0f } };
                            DrawCursor::Advance(style.ItemInnerSpacing.x, 0.0f);
                            ImGui::Text("WARNING");
                            break;
                        }
                        case Logger::Severity::Error: {
                            ScopedColor color{ ImGuiCol_Text, { 1.0f, 0.0f, 0.0f, 1.0f } };
                            DrawCursor::Advance(style.ItemInnerSpacing.x, 0.0f);
                            ImGui::Text("ERROR");
                            break;
                        }
                    }

                    ImGui::TableNextColumn();
                    ImGui::TextWrapped("%s", entry.Message.c_str());
                }
                ImGui::EndTable();
            }
        }
        ImGui::EndChild();
    }

    void DrawPackageInspector(f32 width, f32 height) noexcept {
        const auto& style = ImGui::GetStyle();
        const auto& baseTextColor = style.Colors[ImGuiCol_Text];
        const auto& baseTextLightColor =
                ImVec4{ baseTextColor.x, baseTextColor.y, baseTextColor.z, 0.5f * baseTextColor.w };
        const auto fontSize = ImGui::GetFontSize();

        if (ImGui::BeginChild("##idPackageInspector", { width, height })) {
            if (ImGui::BeginChild("##idPackageInspectorHeader", { 0.0f, fontSize })) {
                DrawCursor::Advance(style.ItemInnerSpacing.x, 0.0f);
                Text::Draw("Package Inspector", Font::Medium, fontSize, baseTextColor);
            }
            ImGui::EndChild();
            const auto maxSize = ImGui::GetContentRegionAvail();
            if (ImGui::BeginTable("##idPackageInspectorTable", 1, ImGuiTableFlags_None,
                                  { maxSize.x - ImGui::GetStyle().ItemInnerSpacing.x, maxSize.y })) {
                auto& history = Tracker::PackageHistory;
                auto reverseHistory = std::vector<Tracker::PackageHistoryEntry>(history.rbegin(), history.rend());
                for (auto& entry : reverseHistory) {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();

                    const auto command = CommandToString(entry.Package.GetFlag());
                    const auto direction = PackageDirectionToString(entry.Direction);
                    const auto time = entry.Time.ToString();
                    const auto id = reinterpret_cast<const void*>(&entry);

                    if (ImGui::TreeNodeEx(id, ImGuiTreeNodeFlags_DefaultOpen, "%s - %s", command, time.c_str())) {
                        const auto tableId = fmt::format("##idPackageHistoryEntry{}", id);
                        if (ImGui::BeginTable(tableId.c_str(), 2, ImGuiTableFlags_RowBg)) {
                            ImGui::TableNextRow();
                            ImGui::TableNextColumn();
                            ImGui::Text("Command");
                            ImGui::TableNextColumn();
                            ImGui::Text("%s", command);

                            ImGui::TableNextRow();
                            ImGui::TableNextColumn();
                            ImGui::Text("Payload");
                            if (entry.Direction == Tracker::PackageDirection::Ingoing) {
                                const auto altitude = entry.Package.Read<f32>(0);
                                const auto azimuth = entry.Package.Read<f32>(1);
                                ImGui::TableNextRow();
                                ImGui::TableNextColumn();
                                ImGui::Text("    Altitude");
                                ImGui::TableNextColumn();
                                ImGui::Text("%s", fmt::format("{:.4f} deg", altitude).c_str());
                                ImGui::TableNextRow();
                                ImGui::TableNextColumn();
                                ImGui::Text("    Azimuth");
                                ImGui::TableNextColumn();
                                ImGui::Text("%s", fmt::format("{:.4f} deg", azimuth).c_str());
                            } else {
                                switch (entry.Package.GetFlag()) {
                                    case Command::None:
                                    case Command::Wakeup:
                                    case Command::Sleep:
                                    case Command::Origin:
                                    case Command::Ack:
                                        ImGui::TableNextColumn();
                                        ImGui::Text("None");
                                        break;
                                    case Command::Advance:
                                    case Command::Move: {
                                        const auto altitude = entry.Package.Read<f32>(0);
                                        const auto azimuth = entry.Package.Read<f32>(1);
                                        const auto angularSpeed = entry.Package.Read<f32>(2);

                                        ImGui::TableNextRow();
                                        ImGui::TableNextColumn();
                                        ImGui::Text("    Altitude");
                                        ImGui::TableNextColumn();
                                        ImGui::Text("%s", fmt::format("{:.4f} deg", altitude).c_str());
                                        ImGui::TableNextRow();
                                        ImGui::TableNextColumn();
                                        ImGui::Text("    Azimuth");
                                        ImGui::TableNextColumn();
                                        ImGui::Text("%s", fmt::format("{:.4f} deg", azimuth).c_str());
                                        ImGui::TableNextRow();
                                        ImGui::TableNextColumn();
                                        ImGui::Text("    Angular Speed");
                                        ImGui::TableNextColumn();
                                        ImGui::Text("%s", fmt::format("{:.4f} deg/s", angularSpeed).c_str());
                                        break;
                                    }
                                    case Command::Configure: {
                                        const auto altitude = entry.Package.Read<f32>(0);
                                        const auto azimuth = entry.Package.Read<f32>(1);
                                        const auto current = entry.Package.Read<f32>(2);
                                        const auto gearRatio = entry.Package.Read<f32>(3);
                                        const auto microSteps = entry.Package.Read<f32>(4);
                                        ImGui::TableNextRow();
                                        ImGui::TableNextColumn();
                                        ImGui::Text("    Altitude");
                                        ImGui::TableNextColumn();
                                        ImGui::Text("%s", fmt::format("{:.4f} deg", altitude).c_str());
                                        ImGui::TableNextRow();
                                        ImGui::TableNextColumn();
                                        ImGui::Text("    Azimuth");
                                        ImGui::TableNextColumn();
                                        ImGui::Text("%s", fmt::format("{:.4f} deg", azimuth).c_str());
                                        ImGui::TableNextRow();
                                        ImGui::TableNextColumn();
                                        ImGui::Text("    Current");
                                        ImGui::TableNextColumn();
                                        ImGui::Text("%s", fmt::format("{:.4f} mA", current).c_str());
                                        ImGui::TableNextRow();
                                        ImGui::TableNextColumn();
                                        ImGui::Text("    Gear Ratio");
                                        ImGui::TableNextColumn();
                                        ImGui::Text("%s", fmt::format("{:.4f}", gearRatio).c_str());
                                        ImGui::TableNextRow();
                                        ImGui::TableNextColumn();
                                        ImGui::Text("    MicroSteps");
                                        ImGui::TableNextColumn();
                                        ImGui::Text("%s", fmt::format("{:.0f}", microSteps).c_str());
                                        break;
                                    }
                                }
                            }

                            const auto packageSize = static_cast<int>(entry.Package.GetSize());
                            ImGui::TableNextRow();
                            ImGui::TableNextColumn();
                            ImGui::Text("Size");
                            ImGui::TableNextColumn();
                            ImGui::Text("%d", packageSize);

                            ImGui::TableNextRow();
                            ImGui::TableNextColumn();
                            ImGui::Text("Direction");
                            ImGui::TableNextColumn();
                            ImGui::Text("%s", direction);

                            ImGui::TableNextRow();
                            ImGui::TableNextColumn();
                            ImGui::Text("Time");
                            ImGui::TableNextColumn();
                            ImGui::Text("%s", time.c_str());
                        }
                        ImGui::EndTable();
                        ImGui::TreePop();
                    }
                }
                ImGui::EndTable();
            }
        }
        ImGui::EndChild();
    }
}// namespace

Control::Control(void* windowHandle) noexcept : View(windowHandle) { }

void Control::OnInit() noexcept { }

void Control::OnDestroy() noexcept { }

void Control::OnUpdate(float deltaTime) noexcept {
    if (ImGui::Begin("Control")) {
        Internal::DrawInfoHeader();

        const auto& style = ImGui::GetStyle();
        const auto width = ImGui::GetContentRegionAvail().x * 0.5f - style.ItemSpacing.x * 0.5f;
        ScopedColor childBackground{ ImGuiCol_ChildBg, style.Colors[ImGuiCol_FrameBg] };
        const auto configHeight = ImGui::GetContentRegionAvail().y * 0.5f - style.ItemSpacing.y * 0.5f;
        DrawConfiguration(width, configHeight);
        ImGui::SameLine();

        const auto steeringHeight = 3.0f * ImGui::GetFontSize() + (2.0f + 2.0f * 0.7f) * style.ItemSpacing.y - 4.0f;
        DrawSteering(width, steeringHeight);

        DrawConsole(width, configHeight);
        ImGui::SameLine();
        DrawCursor::Advance(0.0f, -(configHeight - steeringHeight));
        DrawPackageInspector(width, ImGui::GetContentRegionAvail().y);
    }
    ImGui::End();
}