#include "UI.hpp"

namespace StarTracker::UI {

	void Theme::SetStyle(ThemeStyle theme) noexcept {

		if (theme == ThemeStyle::Dark) {

			// Taken from https://github.com/TheCherno/Hazel
			ImGui::StyleColorsDark();
			auto& colors = ImGui::GetStyle().Colors;
			colors[ImGuiCol_WindowBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

			// Headers
			colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
			colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
			colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

			// Buttons
			colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
			colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
			colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

			// Frame BG
			colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
			colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
			colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

			// Tabs
			colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
			colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
			colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
			colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
			colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

			// Title
			colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
			colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
			colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		}
		else if (theme == ThemeStyle::Light) {

			ImGui::StyleColorsLight();
		}
	}

	void DrawCursor::Advance(const glm::vec2& offset) noexcept {

		const auto cursorPosition = ImGui::GetCursorPos();
		ImGui::SetCursorPos({ cursorPosition.x + offset.x, cursorPosition.y + offset.y });
	}

	void Image::Draw(std::uint32_t textureNativeHandle, const glm::vec2& size) noexcept {

		const auto textureHandle = static_cast<std::intptr_t>(textureNativeHandle);
		ImGui::Image(reinterpret_cast<void*>(textureHandle), { size.x, size.y }, { 0.0f, 1.0f }, { 1.0f, 0.0f });
	}

	void Image::DrawRounded(std::uint32_t textureNativeHandle, const glm::vec2& size) noexcept {

		const auto drawList = ImGui::GetWindowDrawList();
		const auto cursorPosition = ImGui::GetCursorPos();
		const auto windowPosition = ImGui::GetWindowPos();
		const auto drawPosition = ImVec2{ windowPosition.x + cursorPosition.x, windowPosition.y + cursorPosition.y };
		const auto imageColor = ImGui::GetColorU32(ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f });

		const auto textureHandle = static_cast<std::intptr_t>(textureNativeHandle);
		drawList->AddImageRounded(reinterpret_cast<void*>(textureHandle), drawPosition, { drawPosition.x + size.x, drawPosition.y + size.y }, { 0.0f, 1.0f }, { 1.0f, 0.0f }, imageColor, 4.0f);
	}

	void DrawCursor::Set(const glm::vec2& position) noexcept {

		ImGui::SetCursorPos({ position.x, position.y });
	}

	const glm::vec2& DrawCursor::Get() noexcept {

		const auto cursorPosition = ImGui::GetCursorPos();
		return { cursorPosition.x, cursorPosition.y };
	}

	void Text::Draw(const std::string& text, ImFont* font, float fontSize, const ImVec4& color) noexcept {

		const auto drawList = ImGui::GetWindowDrawList();
		const auto cursorPosition = ImGui::GetCursorPos();
		const auto windowPosition = ImGui::GetWindowPos();
		const auto textColor = ImGui::GetColorU32(color);

		const auto drawPosition = ImVec2{ windowPosition.x + cursorPosition.x, windowPosition.y + cursorPosition.y };
		drawList->AddText(font, fontSize, drawPosition, textColor, text.c_str());
	}

	ScopedColor::ScopedColor(ImGuiCol index, const ImVec4& color) noexcept {

		ImGui::PushStyleColor(index, color);
	}

	ScopedColor::~ScopedColor() noexcept {

		ImGui::PopStyleColor();
	}

	ScopedID::ScopedID(const std::string& id) noexcept {

		ImGui::PushID(id.c_str());
	}

	ScopedID::~ScopedID() noexcept {

		ImGui::PopID();
	}

	ScopedStyleVar::ScopedStyleVar(ImGuiStyleVar styleVar, float value) noexcept {

		ImGui::PushStyleVar(styleVar, value);
	}

	ScopedStyleVar::ScopedStyleVar(ImGuiStyleVar styleVar, const ImVec2& value) noexcept {

		ImGui::PushStyleVar(styleVar, value);
	}

	ScopedStyleVar::~ScopedStyleVar() noexcept {

		ImGui::PopStyleVar();
	}

	ScopedWidth::ScopedWidth(float width) noexcept {

		ImGui::PushItemWidth(width);
	}
	ScopedWidth::~ScopedWidth() noexcept {

		ImGui::PopItemWidth();
	}

	ScopedFont::ScopedFont(ImFont* font) noexcept {

		ImGui::PushFont(font);
	}

	ScopedFont::~ScopedFont() noexcept {

		ImGui::PopFont();
	}
}