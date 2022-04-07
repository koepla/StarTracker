#include "StarTrackerView.hpp"

namespace StarTracker {

	StarTrackerView::StarTrackerView(void* nativeWindowHandle) noexcept : Core::View{ nativeWindowHandle } {

		experimentalView = std::make_shared<ExperimentalView>(nativeWindowHandle);
		imageProcessingView = std::make_shared<ImageProcessingView>(nativeWindowHandle);
		trackableBodyView = std::make_shared<TrackableBodyView>(nativeWindowHandle);

		const auto application = Core::Application::GetInstance();
		application->RegisterView(trackableBodyView.get());
		application->RegisterView(experimentalView.get());
		application->RegisterView(imageProcessingView.get());
	}

	void StarTrackerView::OnInit() noexcept {

	}

	void StarTrackerView::OnUpdate(float deltaTime) noexcept {

		const auto application = Core::Application::GetInstance();

		// Multiple KeyCode Detection for Generic ShortCuts
		if (Core::Input::IsKeyPressed(Core::KeyCode::LeftAlt) && Core::Input::IsKeyPressed(Core::KeyCode::F4)) {

			application->Close();
		}

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				ImGui::PushFont(UI::Font::Medium);
				ImGui::Text("General");
				ImGui::PopFont();
				if (ImGui::MenuItem("Exit", "Alt+F4")) {

					application->Close();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Settings")) {

				ImGui::PushFont(UI::Font::Medium);
				ImGui::Text("Appearance");
				ImGui::PopFont();
				static int selectedTheme{ static_cast<int>(UI::ThemeStyle::Light) };
				if (ImGui::Combo("Theme", &selectedTheme, "Light\0Dark\0\0")) {

					UI::Theme::SetStyle(static_cast<UI::ThemeStyle>(selectedTheme));
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Debug")) {

				ImGui::PushFont(UI::Font::Medium);
				ImGui::Text("Performance");
				ImGui::PopFont();
				ImGui::Text("FPS: %f", 1.0f / deltaTime);
				ImGui::Separator();

				ImGui::PushFont(UI::Font::Medium);
				ImGui::Text("Logging");
				ImGui::PopFont();
				ImGui::Text("Info Messages: %d", static_cast<int>(Core::Logger::GetInfoSize()));
				ImGui::Text("Warn Messages: %d", static_cast<int>(Core::Logger::GetWarnSize()));
				ImGui::Text("Error Messages: %d", static_cast<int>(Core::Logger::GetErrorSize()));
				ImGui::Separator();

				const auto allocationStats = Core::Allocator::GetAllocationStats();

				ImGui::PushFont(UI::Font::Medium);
				ImGui::Text("Memory");
				ImGui::PopFont();
				ImGui::Text("Active Allocations: %d", static_cast<int>(allocationStats.ActiveAllocations));
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}
	}

	void StarTrackerView::OnDestroy() noexcept {

	}
}