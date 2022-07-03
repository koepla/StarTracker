#include <StarTracker/StarTrackerApplication.hpp>

namespace StarTracker {

	int Main(int argc, char** argv) {

		(void)argv, (void)argv;

		Core::ApplicationData applicationData{};
		applicationData.Title = "StarTracker";
		applicationData.Width = 1280;
		applicationData.Height = 720;
		applicationData.EnableDockSpace = true;
		applicationData.Fullscreen = false;
		applicationData.VerticalSync = false;

		try {

			StarTrackerApplication application{ applicationData };
			application.Run();
		}
		catch (const std::exception&) {

			ASSERT(false && "Fatal exception!");
		}

		return 0;
	}
}

int main(int argc, char** argv) {

	return StarTracker::Main(argc, argv);
}