#include "ExperimentalView.hpp"

namespace StarTracker {

	ExperimentalView::ExperimentalView(void* nativeWindowHandle) noexcept : Core::View{ nativeWindowHandle }, isFocused{ false }, invertedTextureCoordinates{ false }, camera{}, shader{}, frameBuffer{}, model{} {

	}

	void ExperimentalView::OnInit() noexcept {

		const auto application = Core::Application::GetInstance();
		const auto windowWidth = application->GetWindow().GetWidth();
		const auto windowHeight = application->GetWindow().GetHeight();

		constexpr auto initialCameraPosition = glm::vec3{ 0.0f, 0.0f, 10.0f };
		camera = std::make_shared<Core::OpenGL::Camera>(initialCameraPosition);
		frameBuffer = std::make_shared<Core::OpenGL::FrameBuffer>(windowWidth, windowHeight);
		shader = Core::AssetDataBase::LoadShader("modelVertex.glsl", "modelFragment.glsl");
		model = Core::AssetDataBase::LoadModel("starTracker.obj");
		
		application->RegisterEventHandler([this](const Core::Events::Event& event) -> void {

			const auto windowResize = dynamic_cast<const Core::Events::WindowResizeEvent*>(&event);

			if(windowResize) {
				
				if (windowResize->GetWidth() && windowResize->GetHeight()) {

					frameBuffer->Resize(windowResize->GetWidth(), windowResize->GetHeight());
				}
			}
		});

		Core::OpenGL::Renderer::Initialize();
	}

	void ExperimentalView::OnUpdate(float deltaTime) noexcept {

		const auto application = Core::Application::GetInstance();

		if (Core::Input::IsKeyPressed(Core::KeyCode::LeftControl) && Core::Input::IsKeyPressed(Core::KeyCode::M)) {

			reloadModel();
		}

		if (ImGui::BeginMainMenuBar()) {

			if (ImGui::BeginMenu("File")) {

				ImGui::PushFont(Core::UIFont::Medium);
				ImGui::Text("3D-Model-Viewer");
				ImGui::PopFont();
				if (ImGui::MenuItem("Load Model", "Ctrl+M")) {

					reloadModel();
				}
				ImGui::Separator();
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		const auto transformMatrix = camera->GetProjectionMatrix() * camera->GetViewMatrix(deltaTime, !isFocused);
		shader->SetMat4("uTransform", transformMatrix);

		frameBuffer->Bind();
		Core::OpenGL::Renderer::SetClearColor({ 0.15f, 0.15f, 0.15f, 1.0f });
		Core::OpenGL::Renderer::Clear();
		Core::OpenGL::Renderer::DrawModel(model, shader);
		frameBuffer->Unbind();

		if (ImGui::Begin("3D-Model-Viewer")) {

			auto& style = ImGui::GetStyle();
			const auto itemSpacing = style.ItemSpacing;
			const auto fontSize = ImGui::GetFontSize();
			const auto modelInfoCardHeight = 3.0f * fontSize + (2.0f + 2.0f * 0.7f) * itemSpacing.y - 4.0f;
			drawModelInfoCard({ ImGui::GetContentRegionAvail().x , modelInfoCardHeight });
		
			const auto imageSize = ImGui::GetContentRegionAvail();
			const auto cursorPosition = ImGui::GetCursorPos();
			const auto windowPosition = ImGui::GetWindowPos();
			const auto mousePosition = Core::Input::GetMousePosition();
			const auto imagePosition = glm::vec2{ windowPosition.x + cursorPosition.x, windowPosition.y + cursorPosition.y };

			const auto mouseInBoundsX = mousePosition.x >= imagePosition.x && mousePosition.x <= imagePosition.x + imageSize.x;
			const auto mouseInBoundsY = mousePosition.y >= imagePosition.y && mousePosition.y <= imagePosition.y + imageSize.y;
			
			// Check if mouse is in bounds of the image
			if (mouseInBoundsX && mouseInBoundsY) {

				if (Core::Input::IsMousePressed(Core::MouseCode::ButtonLeft)) {

					isFocused = true;
					application->GetWindow().HideCursor();
				}
			}

			if (Core::Input::IsMousePressed(Core::MouseCode::ButtonRight)) {

				isFocused = false;
				application->GetWindow().ShowCursor();
			}

			ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(frameBuffer->GetNativeTextureHandle())), imageSize, { 0, 1 }, { 1, 0 });
		}
		ImGui::End();
	}

	void ExperimentalView::OnDestroy() noexcept {

	}

	void ExperimentalView::reloadModel() noexcept {

		const auto paths = Utils::File::OpenFileDialog("Select Model", true);

		if (!paths.empty()) {

			std::filesystem::path modelPath{};
			std::filesystem::path texturePath{};

			for (const auto& path : paths) {

				if (path.string().ends_with(".obj") || path.string().ends_with(".OBJ")) {

					modelPath = path;
				}
				else if (
					path.string().ends_with(".png") || 
					path.string().ends_with(".PNG") ||
					path.string().ends_with(".jpg") ||
					path.string().ends_with(".JPG") ||
					path.string().ends_with("jpeg") ||
					path.string().ends_with("JPEG")) {

					texturePath = path;
				}
			}
			
			/* We don't really want to keep it in memory, because 
			* it is client specific and it would increase Video-Memory size
			* if we would keep-alive every loaded model
			*/
			if (texturePath.empty()) {

				model->LoadFromFile(modelPath, invertedTextureCoordinates);
				shader = Core::AssetDataBase::LoadShader("modelVertex.glsl", "modelFragment.glsl");
			}
			else {

				model->LoadFromFile(modelPath, texturePath, invertedTextureCoordinates);
				shader = Core::AssetDataBase::LoadShader("modelTextureVertex.glsl", "modelTextureFragment.glsl");
			}
			
		}   
	}

	void ExperimentalView::drawModelInfoCard(const glm::vec2& size) noexcept {

		const auto style = ImGui::GetStyle();
		ImGui::PushStyleColor(ImGuiCol_ChildBg, style.Colors[ImGuiCol_FrameBg]);
		if (ImGui::BeginChild("idChildConstructionInfoCard", { size.x, size.y }, false, ImGuiWindowFlags_NoScrollbar)) {

			const auto cardInfo = "3D-Model-Viewer (Wavefront OBJ)";
			const auto modelPathInfo = std::format("FilePath: {}", model->GetFilePath().string());
			const auto modelGeometry = model->GetGeometryInfo();
			const auto modelGeometryInfo = std::format("Geometry: {} indices, {} vertices", modelGeometry.IndexCount, modelGeometry.VertexCount);

			const auto drawList = ImGui::GetWindowDrawList();
			const auto cursorPosition = ImGui::GetCursorPos();
			const auto windowPosition = ImGui::GetWindowPos();
			const auto itemInnerSpacing = style.ItemInnerSpacing;
			const auto itemSpacing = style.ItemSpacing;
			const auto baseTextColor = style.Colors[ImGuiCol_Text];
			const auto textColor = ImGui::GetColorU32(baseTextColor);
			const auto textLightColor = ImGui::GetColorU32({ baseTextColor.x, baseTextColor.y, baseTextColor.z, 0.5f * baseTextColor.w });
			const auto fontSize = ImGui::GetFontSize();
			auto drawPosition = ImVec2{ windowPosition.x + cursorPosition.x + itemInnerSpacing.x, windowPosition.y + cursorPosition.y };
			const auto regulatedItemSpacing = 0.7f * itemInnerSpacing.x;

			// Model-Info
			drawList->AddText(Core::UIFont::Medium, fontSize, drawPosition, textColor, cardInfo);
			drawPosition = { drawPosition.x, drawPosition.y + fontSize + regulatedItemSpacing };

			// Path-Info
			drawList->AddText(Core::UIFont::Regular, fontSize - 2.0f, drawPosition, textLightColor, modelPathInfo.c_str());
			drawPosition = { drawPosition.x, drawPosition.y + fontSize - 2.0f + regulatedItemSpacing };

			// Geometry-Info
			drawList->AddText(Core::UIFont::Regular, fontSize - 2.0f, drawPosition, textLightColor, modelGeometryInfo.c_str());


			const auto reloadButtonWidth = ImGui::CalcTextSize("Reload Model").x * 2.5f;
			ImGui::SetCursorPosX(cursorPosition.x + size.x - reloadButtonWidth - itemInnerSpacing.x);
			ImGui::SetCursorPosY(cursorPosition.y + itemInnerSpacing.y);

			ImGui::PushStyleColor(ImGuiCol_FrameBg, style.Colors[ImGuiCol_MenuBarBg]);

			ImGui::Checkbox("Inverted Texture Coordinates", &invertedTextureCoordinates);
			ImGui::PopStyleColor();

			ImGui::SetCursorPosX(cursorPosition.x + size.x - reloadButtonWidth - itemInnerSpacing.x);
			ImGui::SetCursorPosY(cursorPosition.y + 3.0f * itemInnerSpacing.y + itemSpacing.y + fontSize);

			if (ImGui::Button("Reload Model", { reloadButtonWidth, size.y - 3.0f * itemSpacing.y - 2.25f * itemInnerSpacing.y - fontSize })) {

				reloadModel();
			}

			ImGui::EndChild();
			ImGui::PopStyleColor();
		}
	}
}