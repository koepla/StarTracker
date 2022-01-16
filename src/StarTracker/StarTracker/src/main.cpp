#include "StarTrackerApplication.hpp"

auto main(int, char**) -> int {

	StarTracker::Core::ApplicationData applicationData{};
	applicationData.Title = "StarTracker";
	applicationData.Width = 1280;
	applicationData.Height = 720;
	applicationData.EnableDockspace = true;
	applicationData.Fullscreen = false;
	applicationData.VerticalSync = false;

	try {

		StarTracker::StarTrackerApplication application{ applicationData };
		application.Run();
	}
	catch (const std::exception& e) {

		ASSERT(false && "Fatal exception!");
	}

	return 0;
}