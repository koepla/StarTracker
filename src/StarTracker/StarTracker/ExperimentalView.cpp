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
		model = Core::AssetDataBase::LoadModel("StarTrackerMount.obj");
		
		Core::OpenGL::Renderer::Initialize();
	}

	void ExperimentalView::OnUpdate(float deltaTime) noexcept {

		// TODO(Plank): Update to new UI API
		const auto application = Core::Application::GetInstance();

		if (Core::Input::IsKeyPressed(Core::KeyCode::LeftControl) && Core::Input::IsKeyPressed(Core::KeyCode::M)) {

			reloadModel();
		}

		if (ImGui::BeginMainMenuBar()) {

			if (ImGui::BeginMenu("File")) {

				{
					UI::ScopedFont mediumFont{ UI::Font::Medium };
					ImGui::Text("3D-Model-Viewer");
				}

				if (ImGui::MenuItem("Load Model", "Ctrl+M")) {

					reloadModel();
				}
				ImGui::Separator();
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		if (ImGui::Begin("3D-Model-Viewer")) {

			const auto& style = ImGui::GetStyle();
			const auto itemSpacing = style.ItemSpacing;
			const auto fontSize = ImGui::GetFontSize();
			const auto modelInfoCardHeight = 3.0f * fontSize + (2.0f + 2.0f * 0.7f) * itemSpacing.y - 4.0f;
			drawModelInfoCard({ ImGui::GetContentRegionAvail().x , modelInfoCardHeight });
		
			const auto& imageSize = ImGui::GetContentRegionAvail();
			const auto& cursorPosition = ImGui::GetCursorPos();
			const auto& windowPosition = ImGui::GetWindowPos();
			const auto& mousePosition = Core::Input::GetMousePosition();
			const auto imagePosition = glm::vec2{ windowPosition.x + cursorPosition.x, windowPosition.y + cursorPosition.y };

			const auto mouseInBoundsX = mousePosition.x >= imagePosition.x && mousePosition.x <= imagePosition.x + imageSize.x;
			const auto mouseInBoundsY = mousePosition.y >= imagePosition.y && mousePosition.y <= imagePosition.y + imageSize.y;
			
			// Check if mouse is in bounds of the image
			if (mouseInBoundsX && mouseInBoundsY) {

				if (Core::Input::IsMousePressed(Core::MouseCode::ButtonLeft)) {

					isFocused = true;
					Core::Input::SetCursorMode(Core::Input::CursorMode::Disabled);
				}
			}

			if (Core::Input::IsMousePressed(Core::MouseCode::ButtonRight)) {

				isFocused = false;
				Core::Input::SetCursorMode(Core::Input::CursorMode::Default);
			}

			shader->SetFloat3("uCameraPosition", camera->GetPosition());
			shader->SetFloat3("uObjectColor", { 1.0f, 1.0f, 1.0f });
			shader->SetFloat3("uLightColor", { 1.0f, 0.95f, 0.89f });
			shader->SetMat4("uModel", glm::mat4(1.0f));
			shader->SetMat4("uView", camera->GetViewMatrix(deltaTime, !isFocused));
			shader->SetMat4("uProjection", camera->GetProjectionMatrix({ imageSize.x, imageSize.y }));

			frameBuffer->Bind();
			Core::OpenGL::Renderer::SetClearColor({ 0.15f, 0.15f, 0.15f, 1.0f });
			Core::OpenGL::Renderer::Clear();
			Core::OpenGL::Renderer::DrawModel(model, shader);
			frameBuffer->Unbind();
			
			if (ImGui::BeginChild("idChildModelFrameBuffer", ImGui::GetContentRegionAvail(), false, ImGuiWindowFlags_NoScrollbar)) {

				static ImVec2 lastImageSize{};

				if (lastImageSize.x != imageSize.x || lastImageSize.y != lastImageSize.y) {

					frameBuffer->Resize(static_cast<std::int32_t>(imageSize.x), static_cast<std::int32_t>(imageSize.y));
					lastImageSize.x = imageSize.x;
					lastImageSize.y = imageSize.y;
				}

				UI::Image::DrawRounded(frameBuffer->GetNativeTextureHandle(), { imageSize.x, imageSize.y });
			}
			ImGui::EndChild();
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

				if (model->LoadFromFile(modelPath, invertedTextureCoordinates)) {

					shader = Core::AssetDataBase::LoadShader("modelVertex.glsl", "modelFragment.glsl");
				}
			}
			else {

				if (model->LoadFromFile(modelPath, texturePath, invertedTextureCoordinates)) {

					shader = Core::AssetDataBase::LoadShader("modelTextureVertex.glsl", "modelTextureFragment.glsl");
				}
			}
		}   
	}

	void ExperimentalView::drawModelInfoCard(const glm::vec2& size) noexcept {

		const auto& style = ImGui::GetStyle();
		UI::ScopedColor childBackground{ ImGuiCol_ChildBg, style.Colors[ImGuiCol_FrameBg] };

		if (ImGui::BeginChild("idChildConstructionInfoCard", { size.x, size.y }, false, ImGuiWindowFlags_NoScrollbar)) {

			// Initial Cursor Position
			const auto& initialCursor = UI::DrawCursor::Get();

			// Item Spacings
			const auto itemSpacing = style.ItemSpacing;
			const auto itemInnerSpacing = style.ItemInnerSpacing;
			const auto regulatedItemSpacing = 0.7f * itemInnerSpacing.x;

			// Text Colors
			const auto& baseTextColor = style.Colors[ImGuiCol_Text];
			const auto& baseTextLightColor = ImVec4{ baseTextColor.x, baseTextColor.y, baseTextColor.z, 0.5f * baseTextColor.w };

			// Font Sizes
			const auto fontSize = ImGui::GetFontSize();
			const auto smallFontSize = fontSize - 2.0f;

			// Model-Info
			const auto cardInfo = "3D-Model-Viewer (Wavefront OBJ)";
			UI::DrawCursor::Advance({ itemInnerSpacing.x, 0.0f });
			UI::Text::Draw(cardInfo, UI::Font::Medium, fontSize, baseTextColor);

			// Path-Info
			const auto modelPathInfo = std::format("File: {}", model->GetFilePath().filename().string());
			UI::DrawCursor::Advance({ 0.0f, fontSize + regulatedItemSpacing });
			UI::Text::Draw(modelPathInfo, UI::Font::Regular, smallFontSize, baseTextLightColor);

			// Geometry-Info
			const auto& modelGeometry = model->GetGeometryInfo();
			const auto modelGeometryInfo = std::format("Geometry: {} indices, {} vertices", modelGeometry.IndexCount, modelGeometry.VertexCount);
			UI::DrawCursor::Advance({ 0.0f, smallFontSize + regulatedItemSpacing });
			UI::Text::Draw(modelGeometryInfo, UI::Font::Regular, smallFontSize, baseTextLightColor);

			const auto reloadButtonWidth = ImGui::CalcTextSize("Reload Model").x * 2.5f;
			UI::DrawCursor::Set(initialCursor);
			UI::DrawCursor::Advance({ size.x - reloadButtonWidth - itemInnerSpacing.x, itemSpacing.y });

			{
				UI::ScopedColor frameBackground{ ImGuiCol_FrameBg, style.Colors[ImGuiCol_MenuBarBg] };
				ImGui::Checkbox("Inverted Texture Coordinates", &invertedTextureCoordinates);
			}

			const auto advanceOffset = itemSpacing.y + 2.0f * itemInnerSpacing.y + fontSize + regulatedItemSpacing;
			UI::DrawCursor::Set(initialCursor);
			UI::DrawCursor::Advance({ size.x - reloadButtonWidth - itemInnerSpacing.x, advanceOffset });
			if (ImGui::Button("Reload Model", { reloadButtonWidth, size.y - advanceOffset - itemSpacing.y })) {

				reloadModel();
			}
		}
		ImGui::EndChild();
	}
}