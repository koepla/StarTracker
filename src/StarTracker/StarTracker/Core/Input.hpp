#ifndef STARTRACKER_CORE_INPUT_H
#define STARTRACKER_CORE_INPUT_H

#include <StarTracker/Core/Application.hpp>
#include <StarTracker/Core/InputCode.hpp>

#include <glm/glm.hpp>

namespace StarTracker::Core {

	class Input {

	public:
		/**
		* @brief Checks if the specified key is pressed
		*
		* @param keyCode keyCode of the key
		*
		* @return true if the key is currently being pressed
		* 
		*/
		[[nodiscard]] static bool IsKeyPressed(KeyCode keyCode) noexcept;

		/**
		* @brief Checks if the specified mousebutton is pressed
		*
		* @param mouseCode mouseCode of the mousebutton
		*
		* @return true if the mousebutton is currently being pressed
		* 
		*/
		[[nodiscard]] static bool IsMousePressed(MouseCode mouseCode) noexcept;

		/**
		* @brief Gathers the position of the mouse in screen-space
		*
		* @return position of the mouse
		* 
		*/
		[[nodiscard]] static glm::vec2 GetMousePosition() noexcept;
	};
}

#endif // STARTRACKER_CORE_INPUT_H

