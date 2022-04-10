#ifndef STARTRACKER_CORE_UI_H
#define STARTRACKER_CORE_UI_H

#include <StarTracker/Core/Core.hpp>

#include <glm/glm.hpp>

#pragma warning(disable : 26812)
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#pragma warning(default : 26812)

namespace StarTracker::UI {

	struct Font {

		inline static ImFont* Light = nullptr;
		inline static ImFont* Regular = nullptr;
		inline static ImFont* Medium = nullptr;
		inline static ImFont* SemiBold = nullptr;
		inline static ImFont* Bold = nullptr;
		inline static ImFont* Heavy = nullptr;
		inline static ImFont* Italic = nullptr;
	};

	enum class ThemeStyle : std::int16_t {

		Light = 0,
		Dark = 1
	};

	class Theme {

	public:
		/**
		* @brief Sets the UI Theme
		*
		* @param theme Either Light or Dark
		*
		*/
		static void SetStyle(ThemeStyle style) noexcept;
	};

	class DrawCursor {

	public:
		/**
		* @brief Advances the ImGui DrawCursor by the specified offset
		* 
		* @param offset The offset
		* 
		*/
		static void Advance(const glm::vec2& offset) noexcept;

		/**
		* @brief Sets the ImGui DrawCursor to the specified position
		*
		* @param position The position
		*
		*/
		static void Set(const glm::vec2& position) noexcept;

		/**
		* @brief Retrieves the position of the ImGui DrawCursor
		*
		* @return The position of the ImGui DrawCursor
		*
		*/
		[[nodiscard]] static const glm::vec2& Get() noexcept;
	};

	class Image {

	public:
		static void Draw(std::uint32_t textureNativeHandle, const glm::vec2& size) noexcept;
		static void DrawRounded(std::uint32_t textureNativeHandle, const glm::vec2& size) noexcept;
	};

	class Text {

	public:
		static void Draw(std::string_view text, ImFont* font, float fontSize, const ImVec4& color) noexcept;
	};

	class ScopedID {

	public:
		explicit ScopedID(std::string_view id) noexcept;
		~ScopedID() noexcept;
	};

	class ScopedFont {

	public:
		explicit ScopedFont(ImFont* font) noexcept;
		~ScopedFont() noexcept;
	};

	class ScopedColor {

	public:
		ScopedColor(ImGuiCol index, const ImVec4& color) noexcept;
		~ScopedColor() noexcept;
	};

	class ScopedStyleVar {

	public:
		ScopedStyleVar(ImGuiStyleVar styleVar, float value) noexcept;
		ScopedStyleVar(ImGuiStyleVar styleVar, const ImVec2& value) noexcept;
		~ScopedStyleVar() noexcept;
	};

	class ScopedWidth {

	public:
		explicit ScopedWidth(float width) noexcept;
		~ScopedWidth() noexcept;
	};
}

#endif // STARTRACKER_CORE_UI_H