#include <StarTracker/StarTrackerApplication.hpp>

auto main(int, char**) -> int {

	StarTracker::Core::ApplicationData applicationData{};
	applicationData.Title = "StarTracker";
	applicationData.Width = 1280;
	applicationData.Height = 720;
	applicationData.EnableDockSpace = false;
	applicationData.Fullscreen = false;
	applicationData.VerticalSync = true;

	try {

		StarTracker::StarTrackerApplication application{ applicationData };
		application.Run();
	}
	catch (const std::exception&) {

		ASSERT(false && "Fatal exception!");
	}

	return 0;
}