#ifndef STARTRACKER_CORE_APPLICATION_H
#define STARTRACKER_CORE_APPLICATION_H

#include <StarTracker/Core/View.hpp>
#include <StarTracker/Core/UIView.hpp>
#include <StarTracker/Core/Window.hpp>

#include <vector>

namespace StarTracker::Core {

	struct ApplicationData {

		std::int32_t Width;
		std::int32_t Height;
		std::string Title;
		bool EnableDockSpace;
		bool Fullscreen;
		bool VerticalSync;
	};

	class Application {

	protected:
		static inline Application* instance = nullptr;

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

		void RegisterView(View* view) noexcept;
		void RegisterEventHandler(const Events::EventHandler& eventHandler) noexcept;

		[[nodiscard]] Window& GetWindow() noexcept;

	public:
		[[nodiscard]] static Application* GetInstance() noexcept;
	};
}

#endif // STARTRACKER_CORE_APPLICATION_H