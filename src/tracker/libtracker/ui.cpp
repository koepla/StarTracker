#include <charconv>

#include "ui.hpp"

#include <libtracker/core/settings.hpp>

void Theme::SetStyle(ThemeStyle style) noexcept {
    using namespace std::string_literals;
    if (style == ThemeStyle::Dark) {
        // Taken from https://github.com/TheCherno/Hazel
        ImGui::StyleColorsDark();
        auto& colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_WindowBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Headers
        colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Buttons
        colors[ImGuiCol_Button] = colors[ImGuiCol_SliderGrab];
        colors[ImGuiCol_ButtonHovered] = colors[ImGuiCol_SliderGrabActive];
        colors[ImGuiCol_ButtonActive] = colors[ImGuiCol_SliderGrab];

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
        colors[ImGuiCol_PopupBg] = colors[ImGuiCol_WindowBg];
        Settings::Add("Appearance-Theme", "Dark"s);
    } else if (style == ThemeStyle::Light) {
        ImGui::StyleColorsLight();
        auto& colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_PopupBg] = colors[ImGuiCol_WindowBg];
        Settings::Add("Appearance-Theme", "Light"s);
    }
}

int Theme::GetStyleIndex() noexcept {
    const auto& theme = Settings::Get<std::string>("Appearance-Theme");
    if (theme == "Dark") {
        return 1;
    }
    if (theme == "Light") {
        return 0;
    }
    return -1;
}

void DrawCursor::Advance(f32 x, f32 y) noexcept {
    const auto cursorPosition = ImGui::GetCursorPos();
    ImGui::SetCursorPos({ cursorPosition.x + x, cursorPosition.y + y });
}

void Image::Draw(u32 textureNativeHandle, f32 width, f32 height) noexcept {
    ImGui::Image(reinterpret_cast<ImTextureID>(textureNativeHandle), { width, height }, { 0.0f, 1.0f }, { 1.0f, 0.0f });
}

void Image::DrawRounded(u32 textureNativeHandle, f32 width, f32 height) noexcept {
    const auto drawList = ImGui::GetWindowDrawList();
    const auto cursorPosition = ImGui::GetCursorPos();
    const auto windowPosition = ImGui::GetWindowPos();
    const auto drawPosition = ImVec2{ windowPosition.x + cursorPosition.x, windowPosition.y + cursorPosition.y };
    const auto imageColor = ImGui::GetColorU32(ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f });

    drawList->AddImageRounded(reinterpret_cast<ImTextureID>(textureNativeHandle), drawPosition,
                              { drawPosition.x + width, drawPosition.y + height }, { 0.0f, 1.0f }, { 1.0f, 0.0f },
                              imageColor, 4.0f);
}

void Text::Draw(std::string_view text, ImFont* font, f32 fontSize, const ImVec4& color) noexcept {
    const auto drawList = ImGui::GetWindowDrawList();
    const auto cursorPosition = ImGui::GetCursorPos();
    const auto windowPosition = ImGui::GetWindowPos();
    const auto textColor = ImGui::GetColorU32(color);

    const auto drawPosition = ImVec2{ windowPosition.x + cursorPosition.x, windowPosition.y + cursorPosition.y };
    drawList->AddText(font, fontSize, drawPosition, textColor, text.data());
}

ScopedColor::ScopedColor(ImGuiCol index, const ImVec4& color) noexcept {
    ImGui::PushStyleColor(index, color);
}

ScopedColor::~ScopedColor() noexcept {
    ImGui::PopStyleColor();
}

ScopedID::ScopedID(std::string_view id) noexcept {
    ImGui::PushID(id.data());
}

ScopedID::~ScopedID() noexcept {
    ImGui::PopID();
}

ScopedStyleVar::ScopedStyleVar(ImGuiStyleVar styleVar, f32 value) noexcept {
    ImGui::PushStyleVar(styleVar, value);
}

ScopedStyleVar::ScopedStyleVar(ImGuiStyleVar styleVar, const ImVec2& value) noexcept {
    ImGui::PushStyleVar(styleVar, value);
}

ScopedStyleVar::~ScopedStyleVar() noexcept {
    ImGui::PopStyleVar();
}

ScopedWidth::ScopedWidth(f32 width) noexcept {
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

bool DrawInputDateTime(std::string_view label, DateTime& dateTime, bool readOnly) noexcept {
    const auto fullWidth = ImGui::GetContentRegionAvail().x;
    const auto inputWidth = fullWidth / 3.0f - ImGui::GetStyle().ItemSpacing.x;

    auto inputInt64 = [&label, readOnly, inputWidth](s64& value, const char* format) -> s64 {
        ScopedWidth width{ inputWidth };
        const auto id = fmt::format("##id{}{}", label, reinterpret_cast<std::intptr_t>(&value));
        const auto flags = readOnly ? ImGuiSliderFlags_ReadOnly : ImGuiSliderFlags_None;
        constexpr s64 min = 1;
        constexpr s64 max = 1;
        return ImGui::DragScalar(id.c_str(), ImGuiDataType_S64, &value, 1.0f, &min, &max, format, flags);
    };

    auto validationCopy = dateTime;
    auto inputChanged = inputInt64(validationCopy.Year, "%d");
    ImGui::SameLine();
    inputChanged += inputInt64(validationCopy.Month, "%d");
    ImGui::SameLine();
    inputChanged += inputInt64(validationCopy.Day, "%d");

    inputChanged += inputInt64(validationCopy.Hour, "%d");
    ImGui::SameLine();
    inputChanged += inputInt64(validationCopy.Minute, "%d");
    ImGui::SameLine();
    inputChanged += inputInt64(validationCopy.Second, "%d");

    if (ImGui::ButtonEx("Previous", { inputWidth, 0.0f }, ImGuiButtonFlags_Repeat)) {
        inputChanged += 1;
        validationCopy.AddDays(-1);
    }
    ImGui::SameLine();
    if (ImGui::ButtonEx("Next", { inputWidth, 0.0f }, ImGuiButtonFlags_Repeat)) {
        inputChanged += 1;
        validationCopy.AddDays(1);
    }
    ImGui::SameLine();
    if (ImGui::ButtonEx("Now", { inputWidth, 0.0f }, ImGuiButtonFlags_Repeat)) {
        inputChanged += 1;
        validationCopy = DateTime::Now();
    }

    const auto changeIsValid = inputChanged && validationCopy.IsValid();
    if (changeIsValid) {
        dateTime = validationCopy;
    }

    return changeIsValid;
}

bool DrawInputText(std::string& input, bool readonly) noexcept {
    if (input.size() > 256) {
        return false;
    }
    std::string buffer(256, 0);
    std::copy(input.begin(), input.end(), buffer.begin());
    auto address = reinterpret_cast<std::intptr_t>(&input);
    ScopedID inputID{ fmt::format("##idInputText{}", address) };
    ScopedWidth inputWidth{ ImGui::GetContentRegionAvail().x };
    if (ImGui::InputText("##", buffer.data(), buffer.size(),
                         readonly ? ImGuiInputTextFlags_ReadOnly : ImGuiInputTextFlags_None)) {
        input = std::move(buffer);
        return true;
    }
    return false;
}

bool DrawInputDouble(f64& input, const char* format, bool readonly) noexcept {
    auto address = reinterpret_cast<std::intptr_t>(&input);
    ScopedID inputID{ fmt::format("##idInputDouble{}", address) };
    ScopedWidth inputWidth{ ImGui::GetContentRegionAvail().x };
    return ImGui::InputDouble("##", &input, 0, 0, format,
                              readonly ? ImGuiInputTextFlags_ReadOnly : ImGuiInputTextFlags_None);
}

bool DrawInputInt(usize& input, const char* format, bool readonly) noexcept {
    auto address = reinterpret_cast<std::intptr_t>(&input);
    ScopedID inputID{ fmt::format("##idInputDouble{}", address) };
    ScopedWidth inputWidth{ ImGui::GetContentRegionAvail().x };
    return ImGui::InputScalar("##", ImGuiDataType_U64, &input, nullptr, nullptr, format,
                              readonly ? ImGuiInputTextFlags_ReadOnly : ImGuiInputTextFlags_None);
}

bool DrawDragDouble(f64& input,
                    f32 width,
                    f32 height,
                    const char* format,
                    f64 speed,
                    f64 min,
                    f64 max,
                    bool readonly) noexcept {
    const auto id = fmt::format("##idInputDrag{}", reinterpret_cast<std::intptr_t>(&input));
    const auto& style = ImGui::GetStyle();
    const auto fullHeight = height - 2.0f * style.ItemSpacing.y - 0.75f * style.ItemInnerSpacing.y;
    const auto regulatedItemSpacing = 0.7f * style.ItemInnerSpacing.x;
    const auto framePadding =
            ImVec2{ style.ItemInnerSpacing.x,
                    fullHeight - 2.0f * ImGui::GetFontSize() - regulatedItemSpacing - style.ItemInnerSpacing.y };
    ScopedColor frameBackground{ ImGuiCol_FrameBg, style.Colors[ImGuiCol_WindowBg] };
    ScopedWidth dragWidth{ width - 2.0f * style.ItemInnerSpacing.x };
    ScopedStyleVar framePaddingVar{ ImGuiStyleVar_FramePadding, framePadding };
    return ImGui::DragScalar(id.c_str(), ImGuiDataType_Double, &input, static_cast<f32>(speed), &min, &max, format,
                             readonly ? ImGuiSliderFlags_NoInput : ImGuiSliderFlags_None);
}