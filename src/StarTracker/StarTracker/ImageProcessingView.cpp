#include "ImageProcessingView.hpp"

namespace StarTracker {

	ImageProcessingView::ImageProcessingView(void* nativeWindowHandle) noexcept : Core::View{ nativeWindowHandle }, textureList{}, renderFrameBuffer{} {

	}

	void ImageProcessingView::OnInit() noexcept {

		// Initialize ImageProcessing FrameBuffer
		renderFrameBuffer = std::make_shared<Core::OpenGL::FrameBuffer>(160, 90);
	}

	void ImageProcessingView::OnUpdate(float deltaTime) noexcept {

		const auto& style = ImGui::GetStyle();
		const auto textSize = ImGui::GetFontSize();
		const auto itemSpacing = ImGui::GetStyle().ItemSpacing;
		const auto itemInnerSpacing = ImGui::GetStyle().ItemInnerSpacing;

		const auto stackAll = [&]() -> void {

			if (!Core::ImageProcessing::Stack(renderFrameBuffer, textureList)) {

				STARTRACKER_WARN("Couldn't stack images");
			}
		};

		if (Core::Input::IsKeyPressed(Core::KeyCode::LeftAlt) && Core::Input::IsKeyPressed(Core::KeyCode::S)) {

			stackAll();
		}

		if (ImGui::BeginMainMenuBar()) {

			if (ImGui::BeginMenu("Edit")) {

				{
					UI::ScopedFont mediumFont{ UI::Font::Medium };
					ImGui::Text("Image Processing");
				}
				if (ImGui::MenuItem("Stack All", "Alt+S")) {

					stackAll();
				}
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		if (ImGui::Begin("Image Processing", nullptr, ImGuiWindowFlags_NoScrollbar)) {

			const auto flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable;
			if (ImGui::BeginTable("##idChildImageProcessingAlignmentTable", 2, flags)) {

				const auto totalWidth = ImGui::GetContentRegionAvail().x;
				ImGui::TableSetupColumn("##idColumnTextureListPanel", ImGuiTableColumnFlags_WidthStretch, 0.25f * totalWidth);
				ImGui::TableSetupColumn("##idColumnRenderFrameBufferPanel", ImGuiTableColumnFlags_WidthStretch);

				ImGui::TableNextRow();
				{
					ImGui::TableSetColumnIndex(0);
					{
						drawTextureListPanel();
					}

					ImGui::TableSetColumnIndex(1);
					{
						drawRenderFrameBufferPanel();
					}
				}

				ImGui::EndTable();
			}
		}
		ImGui::End();
	}

	void ImageProcessingView::OnDestroy() noexcept {

	}

	void ImageProcessingView::drawTextureListPanel() noexcept {

		const auto& style = ImGui::GetStyle();
		const auto fontSize = ImGui::GetFontSize();
		const auto itemSpacing = style.ItemSpacing;
		const auto itemInnerSpacing = style.ItemInnerSpacing;
		const auto& baseTextColor = style.Colors[ImGuiCol_Text];
		const auto infoCardHeight = 3.0f * fontSize + (2.0f + 2.0f * 0.7f) * itemSpacing.y - 4.0f;
		const auto& baseChildBackground = style.Colors[ImGuiCol_FrameBg];
		const auto& darkerChildBackground = ImVec4{ baseChildBackground.x, baseChildBackground.y, baseChildBackground.z, 0.5f * baseChildBackground.w };

		UI::ScopedColor childBackground{ ImGuiCol_ChildBg, darkerChildBackground };

		// We need this because we want to dynamically remove textures from the textureList
		// due to the fact that the UI function for drawing Images
		// expects the native texture handle, we cannot keep textures alive
		// by simply passing the texture handle.
		// Therefore we have to add textures that are meant to be removed to this list,
		// as their presence keeps them alive after being erased from textureList,
		// so the texture is not destroyed and ImGui can draw it in its draw call
		static std::vector<std::shared_ptr<Core::OpenGL::Texture>> keepAliveTextureList{};
		keepAliveTextureList.clear();

		if (ImGui::BeginChild("idChildTextureListPanel", ImGui::GetContentRegionAvail(), false, ImGuiWindowFlags_NoScrollbar)) {

			{
				UI::ScopedColor headerChildBackground{ ImGuiCol_ChildBg, baseChildBackground };
				if (ImGui::BeginChild("idChildTextureListPanelHeader", { ImGui::GetContentRegionAvail().x, infoCardHeight }, false, ImGuiWindowFlags_HorizontalScrollbar)) {

					UI::DrawCursor::Advance({ itemInnerSpacing.x, 0.0f });
					UI::Text::Draw("Image List", UI::Font::Medium, fontSize, baseTextColor);
					UI::DrawCursor::Advance({ 0.0f, fontSize + itemSpacing.y });

					const auto totalSize = ImGui::GetContentRegionAvail();
					const auto buttonWidth = totalSize.x - itemInnerSpacing.x;
					const auto buttonHeight = totalSize.y - itemInnerSpacing.y;

					if (ImGui::Button("Import", { buttonWidth, buttonHeight })) {

						const auto paths = Utils::File::OpenFileDialog("Select Images", true);

						if (!paths.empty()) {

							for (const auto& path : paths) {

								auto texture = std::make_shared<Core::OpenGL::Texture>();
								if (texture->LoadFromFile(path)) {

									textureList.emplace_back(texture);
								}
								else {

									STARTRACKER_WARN("Failed to load texture {}", path.filename().string());
								}
							}
						}
					}
				}
				ImGui::EndChild();
			}

			UI::ScopedColor transparentBackground{ ImGuiCol_ChildBg, { 0.0f, 0.0f, 0.0f, 0.0f} };

			if (ImGui::BeginChild("idChildTextureList", ImGui::GetContentRegionAvail(), false, ImGuiWindowFlags_HorizontalScrollbar)) {

				UI::DrawCursor::Advance({ itemInnerSpacing.x, 0.0f });

				for (auto i = std::size_t{ 0 }; i < textureList.size(); i++) {

					const auto& currentTexture = textureList.at(i);

					const auto textureHeight = 4.0f * fontSize;
					const auto textureWidth = static_cast<float>(currentTexture->GetWidth()) / static_cast<float>(currentTexture->GetHeight()) * textureHeight;

					if (textureWidth > ImGui::GetContentRegionAvail().x) {

						ImGui::NewLine();
						UI::DrawCursor::Advance({ itemInnerSpacing.x, 0.0f });
					}

					const auto textureChildContainerId = std::format("idChildTexture{}", currentTexture->GetNativeHandle());
					if (ImGui::BeginChild(textureChildContainerId.c_str(), { textureWidth, textureHeight }, false, ImGuiWindowFlags_NoScrollbar)) {

						UI::Image::DrawRounded(currentTexture->GetNativeHandle(), { textureWidth, textureHeight });
					}
					ImGui::EndChild();

					const auto textureRemovePopupId = std::format("idRemoveTexturePopup{}", currentTexture->GetNativeHandle());

					if (ImGui::IsItemHovered()) {

						if (Core::Input::IsMousePressed(Core::MouseCode::ButtonRight)) {

							ImGui::OpenPopup(textureRemovePopupId.c_str());
						}

						ImGui::BeginTooltip();

						const auto& path = currentTexture->GetFilePath();
						const auto width = currentTexture->GetWidth();
						const auto height = currentTexture->GetHeight();
						const auto tooltip = std::format("{} - {}x{}", path.filename().string(), width, height);
						ImGui::Text("%s", tooltip.c_str());

						ImGui::EndTooltip();
					}

					if (ImGui::BeginPopup(textureRemovePopupId.c_str())) {

						if (ImGui::MenuItem("Remove")) {

							keepAliveTextureList.emplace_back(currentTexture);
							textureList.erase(textureList.begin() + i);
						}
						ImGui::EndPopup();
					}

					/*
					* We have to check if the textureList is empty because the user can
					* remove images while the textureList is iterated through
					*/ 
					if (!textureList.empty()) {

						if (currentTexture != textureList.back()) {

							ImGui::SameLine();
						}
					}
				}
			}
			ImGui::EndChild();
		}
		ImGui::EndChild();
	}

	void ImageProcessingView::drawRenderFrameBufferPanel() noexcept {

		const auto& style = ImGui::GetStyle();
		const auto fontSize = ImGui::GetFontSize();
		const auto itemSpacing = style.ItemSpacing;
		const auto itemInnerSpacing = style.ItemInnerSpacing;
		const auto& baseTextColor = style.Colors[ImGuiCol_Text];
		const auto infoCardHeight = fontSize;
		const auto& baseChildBackground = style.Colors[ImGuiCol_FrameBg];
		const auto& darkerChildBackground = ImVec4{ baseChildBackground.x, baseChildBackground.y, baseChildBackground.z, 0.5f * baseChildBackground.w };

		UI::ScopedColor childBackground{ ImGuiCol_ChildBg, baseChildBackground };

		if (ImGui::BeginChild("idChildFrameBufferPanel", ImGui::GetContentRegionAvail(), false, ImGuiWindowFlags_NoScrollbar)) {

			{
				UI::ScopedColor headerChildBackground{ ImGuiCol_ChildBg, baseChildBackground };
				if (ImGui::BeginChild("idChildFrameBufferPanelHeader", { ImGui::GetContentRegionAvail().x, infoCardHeight }, false, ImGuiWindowFlags_NoScrollbar)) {

					UI::DrawCursor::Advance({ itemInnerSpacing.x, 0.0f });
					UI::Text::Draw("Result", UI::Font::Medium, fontSize, baseTextColor);
				}
				ImGui::EndChild();
			}

			const auto textureHandle = renderFrameBuffer->GetNativeTextureHandle();
			const auto textureWidth = ImGui::GetContentRegionAvail().x;
			const auto textureHeight = ImGui::GetContentRegionAvail().y;
			UI::Image::DrawRounded(textureHandle, { textureWidth, textureHeight });
		}
		ImGui::EndChild();
	}
}