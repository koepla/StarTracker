#include "StarTrackerView.hpp"

namespace StarTracker {

	StarTrackerView::StarTrackerView(void* nativeWindowHandle) noexcept : Core::View{ nativeWindowHandle } {

		trackableBodyView = std::make_shared<TrackableBodyView>(nativeWindowHandle);
		modelView = std::make_shared<ModelView>(nativeWindowHandle);
		imageProcessingView = std::make_shared<ImageProcessingView>(nativeWindowHandle);
		astronomicalView = std::make_shared<AstronomicalView>(nativeWindowHandle);

		const auto application = Core::Application::GetInstance();
		application->RegisterView(trackableBodyView.get());
		application->RegisterView(modelView.get());
		application->RegisterView(imageProcessingView.get());
		application->RegisterView(astronomicalView.get());
	}

	void StarTrackerView::OnInit() noexcept {

	}

	void StarTrackerView::OnUpdate(float deltaTime) noexcept {

		const auto application = Core::Application::GetInstance();

		if (Core::Input::IsKeyPressed(Core::KeyCode::LeftAlt) && Core::Input::IsKeyPressed(Core::KeyCode::F4)) {

			application->Close();
		}

		if (ImGui::BeginMainMenuBar()) {

			if (ImGui::BeginMenu("File")) {

				{
					UI::ScopedFont mediumFont{ UI::Font::Medium };
					ImGui::Text("General");
				}
				if (ImGui::MenuItem("Exit", "Alt+F4")) {

					application->Close();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Settings")) {

				{
					UI::ScopedFont mediumFont{ UI::Font::Medium };
					ImGui::Text("Appearance");
				}
				static int selectedTheme{ static_cast<int>(UI::ThemeStyle::Dark) };
				if (ImGui::Combo("Theme", &selectedTheme, "Light\0Dark\0\0")) {

					UI::Theme::SetStyle(static_cast<UI::ThemeStyle>(selectedTheme));
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Debug")) {

				{
					UI::ScopedFont mediumFont{ UI::Font::Medium };
					ImGui::Text("Performance");
				}
				ImGui::Text("FPS: %f", 1.0f / deltaTime);
				ImGui::Separator();

				{
					UI::ScopedFont mediumFont{ UI::Font::Medium };
					ImGui::Text("Logger");
				}
				ImGui::Text("Info Messages: %d", static_cast<int>(Core::Logger::GetInfoSize()));
				ImGui::Text("Warn Messages: %d", static_cast<int>(Core::Logger::GetWarnSize()));
				ImGui::Text("Error Messages: %d", static_cast<int>(Core::Logger::GetErrorSize()));
				if (ImGui::MenuItem("Clear")) {

					Core::Logger::ClearInfoList();
					Core::Logger::ClearWarnList();
					Core::Logger::ClearErrorList();
				}
				ImGui::Separator();

				{
					UI::ScopedFont mediumFont{ UI::Font::Medium };
					ImGui::Text("Memory");
				}
				const auto allocationStats = Core::Allocator::GetAllocationStats();
				ImGui::Text("Active Allocations: %d", static_cast<int>(allocationStats.ActiveAllocations));

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}
	}

	void StarTrackerView::OnDestroy() noexcept {

	}
}