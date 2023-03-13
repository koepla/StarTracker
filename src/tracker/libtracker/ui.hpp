#ifndef LIBTRACKER_UI_H
#define LIBTRACKER_UI_H

/**
 * User interface utility functions
 * @author Elias Engelbert Plank
 * @date 2022-2023
 */

#include <glm/glm.hpp>
#include <libengine/libengine.hpp>
#include <libtracker/core/core.hpp>

#pragma warning(disable : 26812)
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_internal.h>
#include <implot/implot.h>
#pragma warning(default : 26812)

/**
 * Struct containing static members of font variants. This could also be a namespace, but I don't like the concept of a
 * Font namespace, therefore -> struct we have
 * The fonts are loaded and assigned whenever the UIView is initialized
 */
struct Font {
    static inline ImFont* Light = nullptr;
    static inline ImFont* Regular = nullptr;
    static inline ImFont* Medium = nullptr;
    static inline ImFont* SemiBold = nullptr;
    static inline ImFont* Bold = nullptr;
    static inline ImFont* Heavy = nullptr;
    static inline ImFont* Italic = nullptr;
};

/**
 * Enum for controlling the theme
 */
enum class ThemeStyle : s16 { Light = 0, Dark = 1 };

/**
 * Helper class for managing the theme
 */
class Theme {
public:
    /**
     * Sets the UI Theme by assigning color values to ImGui style config
     * @param style Either Light or Dark
     */
    static void SetStyle(ThemeStyle style) noexcept;

    /**
     * Bad code? Retrieves the `style index`, which is really just the enum value from the settings property, and casts
     * it to an integer as we need it as an int for the user interface
     * @return style index, enum value
     */
    static int GetStyleIndex() noexcept;
};

class DrawCursor {
public:
    /**
     * Advances the ImGui DrawCursor by the specified offset
     * @param x Offset on the x axis
     * @param y Offset on the y axis
     */
    static void Advance(f32 x, f32 y) noexcept;
};

/**
 * Utility class for drawing an image in the user interface, by appending to the ImGui draw list.
 */
class Image {
public:
    /**
     * Draws an image in the user interface. It shall be noted that
     * no bounding box is taken into consideration when the next widget is drawn, so the caller must adjust the draw
     * cursor manually
     * @param textureNativeHandle Handle of the texture
     * @param width Width of the image
     * @param height Height of the image
     */
    static void Draw(u32 textureNativeHandle, f32 width, f32 height) noexcept;

    /**
     * Draws a rounded image in the user interface. It shall be noted that
     * no bounding box is taken into consideration when the next widget is drawn, so the caller must adjust the draw
     * cursor manually
     * @param textureNativeHandle Handle of the texture
     * @param width Width of the image
     * @param height Height of the image
     */
    static void DrawRounded(u32 textureNativeHandle, f32 width, f32 height) noexcept;
};

/**
 * Utility class for drawing text in the user interface
 */
class Text {
public:
    /**
     * Draws the specified text at the current draw cursor position
     * @param text Text, ascii only
     * @param font Font which shall be used for drawing
     * @param fontSize Font size
     * @param color Color of the text
     */
    static void Draw(std::string_view text, ImFont* font, f32 fontSize, const ImVec4& color) noexcept;
};

/**
 * Utility class for combining the ImGui id stack with RAII
 */
class ScopedID {
public:
    explicit ScopedID(std::string_view id) noexcept;
    ~ScopedID() noexcept;
};

/**
 * Utility class for combining the ImGui render stack with RAII
 */
class ScopedFont {
public:
    explicit ScopedFont(ImFont* font) noexcept;
    ~ScopedFont() noexcept;
};

/**
 * Utility class for combining the ImGui render stack with RAII
 */
class ScopedColor {
public:
    ScopedColor(ImGuiCol index, const ImVec4& color) noexcept;
    ~ScopedColor() noexcept;
};

/**
 * Utility class for combining the ImGui style var stack with RAII
 */
class ScopedStyleVar {
public:
    ScopedStyleVar(ImGuiStyleVar styleVar, f32 value) noexcept;
    ScopedStyleVar(ImGuiStyleVar styleVar, const ImVec2& value) noexcept;
    ~ScopedStyleVar() noexcept;
};

/**
 * Utility class for combining the ImGui render stack with RAII
 */
class ScopedWidth {
public:
    explicit ScopedWidth(f32 width) noexcept;
    ~ScopedWidth() noexcept;
};

/**
 * Draws a widget for controlling a DateTime object, draws every component other than Milliseconds
 * @param label Label, which is used as an ID internally
 * @param dateTime DateTime object that is manipulated by the widget
 * @param readOnly Read only
 * @return true if the DateTime object was changed
 */
bool DrawInputDateTime(std::string_view label, DateTime& dateTime, bool readOnly = false) noexcept;

/**
 * Draws a widget for manipulation of a std::string, abstracts away id stack
 * @param input Input string that is manipulated by the widget
 * @param readonly Read only
 * @return true if the string was changed
 */
bool DrawInputText(std::string& input, bool readonly = false) noexcept;

/**
 * Draws a widget for manipulation of a formatted f64 or double, abstracts away id stack
 * @param input Input that is manipulated by the widget
 * @param format Format string
 * @param readonly Read only
 * @return true if the input was changed
 */
bool DrawInputDouble(f64& input, const char* format = "%f", bool readonly = false) noexcept;

/**
 * Draws a widget for manipulation of a formatted int, abstracts away id stack
 * @param input Input that is manipulated by the widget
 * @param format Format string
 * @param readonly Read only
 * @return true if the input was changed
 */
bool DrawInputInt(usize& input, const char* format = "%d", bool readonly = false) noexcept;

/**
 * Draws a rather large and specialized widget for manipulation of a double value
 * @param input Input that is manipulated by the widget
 * @param width Width of the widget
 * @param height Height of the widget
 * @param format Format string
 * @param speed Speed at which the input changes when dragged over
 * @param min Lower bound
 * @param max Upper bound
 * @param readonly Read only
 * @return true if the input was changed
 */
bool DrawDragDouble(f64& input,
                    f32 width,
                    f32 height,
                    const char* format = "%f",
                    f64 speed = 1.0,
                    f64 min = 0.0,
                    f64 max = 100.0,
                    bool readonly = false) noexcept;

#endif// LIBTRACKER_UI_H