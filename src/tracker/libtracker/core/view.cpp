#include "view.hpp"
#include "ui.hpp"
#include "window.hpp"

View::View(void* windowHandle) noexcept : nativeWindowHandle{ windowHandle } { }

void View::SetNativeWindowHandle(void* windowHandle) noexcept {
    nativeWindowHandle = windowHandle;
}

UIView::UIView(void* windowHandle, bool enableDockSpace) noexcept
    : View{ windowHandle },
      enableDockSpace{ enableDockSpace } { }

void UIView::UIBegin() const noexcept {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (enableDockSpace) {
        static bool optFullscreenPersistent = true;
        static bool dockSpaceOpen = true;
        const bool optFullscreen = optFullscreenPersistent;
        static ImGuiDockNodeFlags dockSpaceFlags = ImGuiDockNodeFlags_None;

        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (optFullscreen) {
            const auto viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                           ImGuiWindowFlags_NoMove;
            windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }

        if (dockSpaceFlags & ImGuiDockNodeFlags_PassthruCentralNode) {
            windowFlags |= ImGuiWindowFlags_NoBackground;
        }

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("MainDockSpaceId", &dockSpaceOpen, windowFlags);
        ImGui::PopStyleVar();

        if (optFullscreen) {
            ImGui::PopStyleVar(2);
        }

        const auto& io = ImGui::GetIO();
        ImGuiStyle& style = ImGui::GetStyle();
        const auto minWinSizeX = style.WindowMinSize.x;
        style.WindowMinSize.x = 200.0f;

        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
            const auto dockSpaceId = ImGui::GetID("MainDockSpaceId");
            ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), dockSpaceFlags);
        }

        style.WindowMinSize.x = minWinSizeX;
    }
}

void UIView::UIEnd() const noexcept {
    if (enableDockSpace) {
        ImGui::End();
    }

    const auto& io = ImGui::GetIO();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* backupCurrentContext = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backupCurrentContext);
    }
}

void UIView::OnInit() noexcept {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    auto& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    auto& style = ImGui::GetStyle();

    Theme::SetStyle(static_cast<ThemeStyle>(Theme::GetStyleIndex()));
    style.FrameRounding = 4.0f;
    style.ChildRounding = 4.0f;
    style.GrabRounding = 4.0f;
    style.PopupRounding = 4.0f;
    style.ScrollbarRounding = 4.0f;
    style.TabRounding = 4.0f;
    style.WindowRounding = 4.0f;
    style.WindowBorderSize = 0.0f;

    constexpr auto fontSize = 18.0f;
    Font::Light = io.Fonts->AddFontFromFileTTF("assets/fonts/SFNSDisplay-Light.ttf", fontSize);
    Font::Regular = io.Fonts->AddFontFromFileTTF("assets/fonts/SFNSDisplay-Regular.ttf", fontSize);
    Font::Medium = io.Fonts->AddFontFromFileTTF("assets/fonts/SFNSDisplay-Medium.ttf", fontSize);
    Font::SemiBold = io.Fonts->AddFontFromFileTTF("assets/fonts/SFNSDisplay-Semibold.ttf", fontSize);
    Font::Bold = io.Fonts->AddFontFromFileTTF("assets/fonts/SFNSDisplay-Bold.ttf", fontSize);
    Font::Heavy = io.Fonts->AddFontFromFileTTF("assets/fonts/SFNSDisplay-Heavy.ttf", fontSize);
    Font::Italic = io.Fonts->AddFontFromFileTTF("assets/fonts/SFNSText-RegularItalic.ttf", fontSize);
    io.FontDefault = Font::Regular;

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(nativeWindowHandle), true);
    ImGui_ImplOpenGL3_Init("#version 450");
}

void UIView::OnUpdate(f32 deltaTime) noexcept { }

void UIView::OnDestroy() noexcept {
    ImPlot::DestroyContext();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
