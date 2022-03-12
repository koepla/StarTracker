#include "UIView.hpp"

namespace StarTracker::Core {
	
	UIView::UIView(void* nativeWindowHandle, bool enableDockSpace) noexcept : View{nativeWindowHandle }, enableDockSpace{enableDockSpace } {
	
	}

	void UIView::UIBegin() noexcept {

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

	void UIView::UIEnd() noexcept {

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

		ImGui::StyleColorsLight();

		float fontSize = 22.0f;
		UIFont::Light = io.Fonts->AddFontFromFileTTF("Assets/Fonts/SFNSDisplay-Light.ttf", fontSize);
		UIFont::Regular = io.Fonts->AddFontFromFileTTF("Assets/Fonts/SFNSDisplay-Regular.ttf", fontSize);
		UIFont::Medium = io.Fonts->AddFontFromFileTTF("Assets/Fonts/SFNSDisplay-Medium.ttf", fontSize);
		UIFont::SemiBold = io.Fonts->AddFontFromFileTTF("Assets/Fonts/SFNSDisplay-SemiBold.ttf", fontSize);
		UIFont::Bold = io.Fonts->AddFontFromFileTTF("Assets/Fonts/SFNSDisplay-Bold.ttf", fontSize);
		UIFont::Heavy = io.Fonts->AddFontFromFileTTF("Assets/Fonts/SFNSDisplay-Heavy.ttf", fontSize);
		UIFont::Italic = io.Fonts->AddFontFromFileTTF("Assets/Fonts/SFNSText-RegularItalic.ttf", fontSize);
		io.FontDefault = UIFont::Regular;

		ImGuiStyle& style = ImGui::GetStyle();
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
}