#include "StarTrackerView.hpp"

namespace StarTracker {

    StarTrackerView::StarTrackerView(void *nativeWindowHandle) noexcept : Core::View{ nativeWindowHandle } {

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
                if (ImGui::MenuItem("Exit", "ALT+F4")) {

                    application->Close();
                }
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }
    }

    void StarTrackerView::OnDestroy() noexcept {

    }
}