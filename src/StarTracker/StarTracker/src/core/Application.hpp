#ifndef STARTRACKER_CORE_APPLICATION_H
#define STARTRACKER_CORE_APPLICATION_H

#include "View.hpp"
#include "UIView.hpp"
#include "Window.hpp"

#include <vector>

namespace StarTracker::Core {

	struct ApplicationData {

		std::int32_t Width;
		std::int32_t Height;
		std::string Title;
		bool EnableDockspace;
		bool Fullscreen;
		bool VerticalSync;
	};

	class Application {

	protected:
		inline static Application* instance = nullptr;

	protected:
		Window window;
		UIView userInterfaceView;
		std::vector<View*> viewList;

	public:
		Application() = delete;
		Application(const Application&) = delete;
		explicit Application(const ApplicationData& applicationData) noexcept;

		void Run() noexcept;
		void Close() noexcept;

		void AddToViewList(View* view) noexcept;

		[[nodiscard]] Window& GetWindow() noexcept;

	public:
		[[nodiscard]] inline static Application* GetInstance() noexcept;
	};
}

#endif // STARTRACKER_CORE_APPLICATION_H