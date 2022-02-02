#include "ExperimentalView.hpp"

#include <glm/glm.hpp>

namespace StarTracker {

	ExperimentalView::ExperimentalView(void* nativeWindowHandle) noexcept : Core::View{ nativeWindowHandle } {

	}

	void ExperimentalView::OnInit() noexcept {

		// Called once on startup
	}

	void ExperimentalView::OnUpdate(float deltaTime) noexcept {

		// Called every frame
		if (ImGui::Begin("Experimental")) {

		}
		ImGui::End();
	}

	void ExperimentalView::OnDestroy() noexcept {

		// Called once on shutdown
	}
}