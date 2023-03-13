#include "workspace.hpp"
#include "libtracker.hpp"

namespace {

    void RemoveDescriptiveLocation() {
        Settings::Remove("Location-City");
        Settings::Remove("Location-RegionName");
        Settings::Remove("Location-Country");
    }

    void DrawSettingsPanel(bool& openSettings) noexcept {
        bool open = true;
        if (ImGui::BeginPopupModal("Settings", &open, ImGuiWindowFlags_Modal)) {
            if (ImGui::CollapsingHeader("Location", ImGuiTreeNodeFlags_DefaultOpen)) {
                if (ImGui::BeginTable("##idLocationMenuAlignmentTable", 2)) {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("Latitude");
                    ImGui::TableNextColumn();
                    if (DrawInputDouble(Settings::Get<double>("Location-Latitude", 51.476852), "%.6f deg")) {
                        RemoveDescriptiveLocation();
                    }

                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("Longitude");
                    ImGui::TableNextColumn();
                    if (DrawInputDouble(Settings::Get<double>("Location-Longitude", 0.0), "%.6f deg")) {
                        RemoveDescriptiveLocation();
                    }

                    if (LocationManager::IsDescriptive()) {
                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::Text("City");
                        ImGui::TableNextColumn();
                        DrawInputText(Settings::Get<std::string>("Location-City"), true);
                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::Text("Region");
                        ImGui::TableNextColumn();
                        DrawInputText(Settings::Get<std::string>("Location-RegionName"), true);
                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::Text("Country");
                        ImGui::TableNextColumn();
                        DrawInputText(Settings::Get<std::string>("Location-Country"), true);
                    }

                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    static std::string failureMessage;
                    if (!failureMessage.empty()) {
                        Text::Draw(failureMessage, Font::Medium, ImGui::GetFontSize(), { 1.0f, 0.0f, 0.0f, 1.0f });
                    }
                    ImGui::TableNextColumn();
                    const auto buttonWidth = ImGui::GetContentRegionAvail().x;
                    if (ImGui::Button("Fetch Online", { buttonWidth, 0.0f })) {
                        if (!LocationManager::FetchOnline()) {
                            failureMessage = "Could not fetch location";
                        } else {
                            failureMessage = "";
                        }
                    }

                    ImGui::EndTable();
                }
            }
            if (ImGui::CollapsingHeader("Tracker", ImGuiTreeNodeFlags_DefaultOpen)) {
                if (ImGui::BeginTable("##idTrackerMenuAlignmentTable", 2)) {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("Tracking Duration");
                    ImGui::TableNextColumn();
                    DrawInputDouble(Settings::Get<double>("Tracker-Duration", 600), "%f s");

                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("Angular Speed");
                    ImGui::TableNextColumn();
                    DrawInputDouble(Settings::Get<double>("Tracker-AngularSpeed", 0.5), "%f deg/s");

                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("Package History Limit");
                    ImGui::TableNextColumn();
                    DrawInputInt(Settings::Get<usize>("Tracker-PackageHistoryLimit", 200));

                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("Port");
                    ImGui::TableNextColumn();

                    const auto preview = Settings::Get<std::string>("Tracker-Port");
                    const auto ports = Tracker::GetPortNames();

                    {
                        ScopedWidth width{ ImGui::GetContentRegionAvail().x };
                        if (ImGui::BeginCombo("##idPortCombo", preview.c_str())) {
                            for (const auto& port : ports) {
                                if (ImGui::Selectable(port.c_str())) {
                                    Settings::Get<std::string>("Tracker-Port") = port;
                                }
                            }
                            ImGui::EndCombo();
                        }
                    }

                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::TableNextColumn();
                    if (ImGui::Button("Connect", { ImGui::GetContentRegionAvail().x, 0.0f })) {
                        Tracker::Connect();
                    }

                    ImGui::EndTable();
                }
            }
            if (ImGui::CollapsingHeader("Appearance", ImGuiTreeNodeFlags_DefaultOpen)) {
                if (ImGui::BeginTable("##idLocationMenuAlignmentTable", 2)) {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("Theme");
                    ImGui::TableNextColumn();
                    static int themeIndex = Theme::GetStyleIndex();
                    {
                        ScopedWidth comboWidth{ ImGui::GetContentRegionAvail().x };
                        if (ImGui::Combo("##idAppearanceTheme", &themeIndex, "Light\0Dark\0\0")) {
                            Theme::SetStyle(static_cast<ThemeStyle>(themeIndex));
                        }
                    }
                    ImGui::EndTable();
                }
            }
            if (ImGui::CollapsingHeader("General", ImGuiTreeNodeFlags_DefaultOpen)) {
                if (ImGui::BeginTable("##idGeneralMenuAlignmentTable", 2)) {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("Verbose Output");
                    ImGui::TableNextColumn();
                    {
                        ScopedWidth comboWidth{ ImGui::GetContentRegionAvail().x };
                        ImGui::Checkbox("##idVerboseOutput", &Settings::Get<bool>("Output-Verbose", false));
                    }
                    ImGui::EndTable();
                }
            }
            ImGui::EndPopup();
        } else if (openSettings) {
            AssetDatabase::SaveSettings("settings.json");
            openSettings = false;
        }
    }
}// namespace

Workspace::Workspace(void* windowHandle) noexcept : View{ windowHandle } {
    AssetDatabase::LoadSettings("settings.json");
    AssetDatabase::LoadCatalogManager("ngc2000.dat", "names.dat", "planets.json");
    Tracker::Initialize();

    graphics::Renderer::Initialize();
    ImageProcessing::Initialize();

    if (!LocationManager::IsConfigured() && !LocationManager::FetchOnline()) {
        LIBTRACKER_WARN("Location needs configuration");
        openSettings = true;
    }

    tracking = std::make_shared<Tracking>(nativeWindowHandle);
    processing = std::make_shared<Processing>(nativeWindowHandle);
    control = std::make_shared<Control>(nativeWindowHandle);

    const auto application = Application::GetInstance();
    application->RegisterView(tracking);
    application->RegisterView(processing);
    application->RegisterView(control);
}

void Workspace::OnInit() noexcept { }

void Workspace::OnUpdate(float deltaTime) noexcept {
    const auto application = Application::GetInstance();
    if (Input::IsKeyPressed(KeyCode::LeftAlt) && Input::IsKeyPressed(KeyCode::F4)) {
        application->Close();
    } else if (Input::IsKeyPressed(KeyCode::LeftAlt) && Input::IsKeyPressed(KeyCode::S)) {
        openSettings = true;
    }

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            {
                ScopedFont mediumFont{ Font::Medium };
                ImGui::Text("General");
            }
            if (ImGui::MenuItem("Exit", "Alt+F4")) {
                application->Close();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Settings")) {
            if (ImGui::MenuItem("Change", "Alt+S")) {
                openSettings = true;
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Metrics")) {
            {
                ScopedFont mediumFont{ Font::Medium };
                ImGui::Text("Performance");
            }
            ImGui::Text("FPS: %f", 1.0f / deltaTime);
            ImGui::Separator();
            {
                ScopedFont mediumFont{ Font::Medium };
                ImGui::Text("Logger");
            }
            ImGui::Text("Messages: %d", static_cast<int>(Logger::GetLog().size()));
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    if (openSettings) {
        ImGui::OpenPopup("Settings");
    }
    DrawSettingsPanel(openSettings);
}

void Workspace::OnDestroy() noexcept {
    AssetDatabase::SaveSettings("settings.json");
}