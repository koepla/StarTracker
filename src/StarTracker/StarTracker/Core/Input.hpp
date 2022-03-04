#ifndef STARTRACKER_CORE_INPUT_H
#define STARTRACKER_CORE_INPUT_H

#include <StarTracker/Core/Application.hpp>
#include <StarTracker/Core/InputCode.hpp>

#include <glm/glm.hpp>

namespace StarTracker::Core {

	class Input {

	public:
		[[nodiscard]] static bool IsKeyPressed(KeyCode keyCode) noexcept;
		[[nodiscard]] static bool IsMousePressed(MouseCode mouseCode) noexcept;
		[[nodiscard]] static glm::vec2 GetMousePosition() noexcept;
	};
}

#endif // STARTRACKER_CORE_INPUT_H

