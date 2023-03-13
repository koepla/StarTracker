#include <memory>

#include "libtracker/core/application.hpp"
#include "workspace.hpp"

int main(int, char**) {
    ApplicationData applicationData{};
    applicationData.Title = "StarTracker";
    applicationData.Width = 1280;
    applicationData.Height = 720;
    applicationData.EnableDockSpace = true;
    applicationData.Fullscreen = false;
    applicationData.VerticalSync = true;

    try {
        Application application{ applicationData };
        application.RegisterView(std::make_shared<Workspace>(application.GetWindow().GetNativeHandle()));
        application.Run();
    } catch (...) {
        LIBTRACKER_ASSERT(false, "Fatal error!");
    }

    return 0;
}
