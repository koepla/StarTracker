#ifndef STARTRACKER_CORE_APPLICATION_H
#define STARTRACKER_CORE_APPLICATION_H

#include "View.hpp"
#include "Window.hpp"

#include <vector>

namespace StarTracker::Core {

	using ApplicationData = WindowData;

	class Application {

	private:
		inline static Application* instance = nullptr;

	private:
		Window window;
		std::vector<View*> viewList;

	public:
		Application() = delete;
		Application(const Application&) = delete;
		explicit Application(const ApplicationData& applicationData) noexcept;

		void Run() noexcept;
		void Close() noexcept;

		[[nodiscard]] Window& GetWindow() noexcept;

	public:
		[[nodiscard]] inline static Application* GetInstance() noexcept;
	};
}

#endif // STARTRACKER_CORE_APPLICATION_H