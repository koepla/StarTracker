#include "UIView.hpp"

namespace StarTracker::Core {
	
	UIView::UIView(void* nativeWindowHandle, bool enableDockSpace) noexcept : View{nativeWindowHandle }, enableDockSpace{enableDockSpace } {
	
	}

	void UIView::UIBegin() const noexcept {

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (enableDockSpace) {

			static bool optFullscreenPersistant = true;
			static bool dockspaceOpen = true;
			bool optFullscreen = optFullscreenPersistant;
			static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

			ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
			if (optFullscreen) {

				ImGuiViewport* viewport = ImGui::GetMainViewport();
				ImGui::SetNextWindowPos(viewport->Pos);
				ImGui::SetNextWindowSize(viewport->Size);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
				windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
				windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
			}

			if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode) {

				windowFlags |= ImGuiWindowFlags_NoBackground;
			}

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::Begin("MainDockSpaceId", &dockspaceOpen, windowFlags);
			ImGui::PopStyleVar();

			if (optFullscreen) {
			
				ImGui::PopStyleVar(2);
			}

			ImGuiIO& io = ImGui::GetIO();
			ImGuiStyle& style = ImGui::GetStyle();
			float minWinSizeX = style.WindowMinSize.x;
			style.WindowMinSize.x = 200.0f;

			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspaceId = ImGui::GetID("MainDockSpaceId");
				ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), dockspaceFlags);
			}

			style.WindowMinSize.x = minWinSizeX;
		}
	}

	void UIView::UIEnd() const noexcept {

		if (enableDockSpace) {

			ImGui::End();
		}

		ImGuiIO& io = ImGui::GetIO();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backupCurrentContext = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backupCurrentContext);
		}
	}
	
	void UIView::OnInit() noexcept {

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; 
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGuiStyle& style = ImGui::GetStyle();

		SetTheme(UITheme::Light);
		style.FrameRounding = 4.0f;
		style.ChildRounding = 4.0f;
		style.GrabRounding = 4.0f;
		style.PopupRounding = 4.0f;
		style.ScrollbarRounding = 4.0f;
		style.TabRounding = 4.0f;
		style.WindowRounding = 0.0f;
		style.WindowBorderSize = 0.0f;

		const auto fontSize = 18.0f;
		UIFont::Light = io.Fonts->AddFontFromFileTTF("Assets/Fonts/SFNSDisplay-Light.ttf", fontSize);
		UIFont::Regular = io.Fonts->AddFontFromFileTTF("Assets/Fonts/SFNSDisplay-Regular.ttf", fontSize);
		UIFont::Medium = io.Fonts->AddFontFromFileTTF("Assets/Fonts/SFNSDisplay-Medium.ttf", fontSize);
		UIFont::SemiBold = io.Fonts->AddFontFromFileTTF("Assets/Fonts/SFNSDisplay-SemiBold.ttf", fontSize);
		UIFont::Bold = io.Fonts->AddFontFromFileTTF("Assets/Fonts/SFNSDisplay-Bold.ttf", fontSize);
		UIFont::Heavy = io.Fonts->AddFontFromFileTTF("Assets/Fonts/SFNSDisplay-Heavy.ttf", fontSize);
		UIFont::Italic = io.Fonts->AddFontFromFileTTF("Assets/Fonts/SFNSText-RegularItalic.ttf", fontSize);
		io.FontDefault = UIFont::Regular;

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		ImGui_ImplGlfw_InitForOpenGL(reinterpret_cast<GLFWwindow*>(nativeWindowHandle), true);
		ImGui_ImplOpenGL3_Init("#version 450");
	}

	void UIView::OnUpdate(float deltaTime) noexcept {


	}

	void UIView::OnDestroy() noexcept {

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void UIView::SetTheme(UITheme theme) noexcept {

		if (theme == UITheme::Dark) {

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
		else if (theme == UITheme::Light) {

			ImGui::StyleColorsLight();
		}
	}
}
