#include <StarAPI/StarAPI.hpp>

#include "core/Core.hpp"
#include "core/Assert.hpp"
#include "core/Application.hpp"
#include "utils/serial/Package.hpp"
#include "utils/serial/Serial.hpp"

#include "utils/GeoLocation.hpp"
#include "utils/HttpRequest.hpp"
#include "utils/Stopwatch.hpp"

#include <iostream>
#include <filesystem>

auto main(int, const char**) -> int {

	StarTracker::Core::ApplicationData applicationData{};
	applicationData.Title = "StarTracker";
	applicationData.Width = 1280;
	applicationData.Height = 720;
	applicationData.EnableDockspace = true;
	applicationData.Fullscreen = false;

	StarTracker::Core::Application application{ applicationData };
	application.Run();

	return 0;
}