#include <StarTracker/StarTrackerApplication.hpp>

int main(int, char**) {

	StarTracker::Core::ApplicationData applicationData{};
	applicationData.Title = "StarTracker";
	applicationData.Width = 1280;
	applicationData.Height = 720;
	applicationData.EnableDockSpace = true;
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

#ifdef _WIN32

int WINAPI WinMain( _In_ HINSTANCE /*Instance*/, _In_opt_ HINSTANCE /*PrevInstance*/, _In_ LPSTR /*CmdLine*/, _In_ int /*ShowCmd*/ ) {

	return main(__argc, __argv);
}

#endif // _WIN32