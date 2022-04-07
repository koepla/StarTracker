#include "ImageProcessingView.hpp"

namespace StarTracker {

	ImageProcessingView::ImageProcessingView(void *nativeWindowHandle) noexcept : Core::View{nativeWindowHandle}, textureList{} {

	}

	void ImageProcessingView::OnInit() noexcept {

		// Initialize ImageProcessing FrameBuffer
		stackFrameBuffer = std::make_shared<Core::OpenGL::FrameBuffer>(160, 90);
		kernelFrameBuffer = std::make_shared<Core::OpenGL::FrameBuffer>(160, 90);
	}

	void ImageProcessingView::OnUpdate(float deltaTime) noexcept {

		const auto style = ImGui::GetStyle();
		const auto textSize = ImGui::GetFontSize();
		const auto itemSpacing = ImGui::GetStyle().ItemSpacing;
		const auto itemInnerSpacing = ImGui::GetStyle().ItemInnerSpacing;
		static auto kernelMatrixEditorHeight{ 0.0f };

		if (ImGui::Begin("Image Processing")) {

			if (ImGui::BeginTable("##idImageProcessingAlignmentTable", 2))
			{
				// Row-0
				ImGui::TableNextRow();

				{   // Final Images
					UI::ScopedFont mediumFont{ UI::Font::Medium };

					const auto textureWidth = ImGui::GetContentRegionAvail().x / 2.0f;
					const auto textureHeight = static_cast<float>(stackFrameBuffer->GetHeight()) / static_cast<float>(stackFrameBuffer->GetWidth()) * textureWidth;

					ImGui::TableSetColumnIndex(0);
					ImGui::Text("Stacked Image");
					if (ImGui::BeginChild("idChildStackFrameBuffer", { textureWidth, textureHeight}, false, ImGuiWindowFlags_NoScrollbar)) {

						UI::Image::DrawRounded(stackFrameBuffer->GetNativeTextureHandle(), { textureWidth - itemInnerSpacing.x, textureHeight });
					}
					ImGui::EndChild();

					ImGui::TableSetColumnIndex(1);
					ImGui::Text("Kernel Image");
					if (ImGui::BeginChild("idChildKernelFrameBuffer", { textureWidth, textureHeight }, false, ImGuiWindowFlags_NoScrollbar)) {

						UI::Image::DrawRounded(kernelFrameBuffer->GetNativeTextureHandle(), { textureWidth - itemInnerSpacing.x, textureHeight });
					}
					ImGui::EndChild();
				}

				// Row-1
				ImGui::TableNextRow();

				{   // Stacking
					UI::ScopedFont mediumFont{ UI::Font::Medium };
					UI::ScopedColor childBackground{ ImGuiCol_ChildBg, style.Colors[ImGuiCol_FrameBg] };

					ImGui::TableSetColumnIndex(0);
					ImGui::Text("Loaded Images");

					if (ImGui::BeginChild("idTextureList", ImVec2(ImGui::GetContentRegionAvail().x, kernelMatrixEditorHeight), false, ImGuiWindowFlags_HorizontalScrollbar)) {

						UI::DrawCursor::Advance({ itemInnerSpacing.x, itemInnerSpacing.y });

						for (const auto& currentTexture : textureList) {

							const auto textureHeight = kernelMatrixEditorHeight - 2.0f * itemInnerSpacing.y;
							const auto textureWidth = static_cast<float>(currentTexture->GetWidth()) / static_cast<float>(currentTexture->GetHeight()) * textureHeight;

							if (textureWidth > ImGui::GetContentRegionAvail().x) {

								ImGui::NewLine();
								UI::DrawCursor::Advance({ itemInnerSpacing.x, 0.0f });
							}

							UI::Image::Draw(currentTexture->GetNativeHandle(), { textureWidth, textureHeight });

							if (currentTexture != textureList.back()) {

								ImGui::SameLine();
							}
						}
					}
					ImGui::EndChild();
				}

				static std::array<float, 9> userKernel{ 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f };
				static std::array<float, 9> selectedKernel{ userKernel };

				{
					// Kernel
					ImGui::TableSetColumnIndex(1);

					{
						UI::ScopedWidth textWidth{ ImGui::GetContentRegionAvail().x };
						UI::ScopedFont mediumFont{ UI::Font::Medium };
						ImGui::Text("Kernel Matrix");
					}

					const auto availableWidth = ImGui::GetContentRegionAvail().x;
					const auto resetButtonTextWidth = ImGui::CalcTextSize("Reset").x * 2.5f;

					kernelMatrixEditorHeight = UI::DrawCursor::Get().y;
					
					static ImGuiInputTextFlags_ kernelMatrixInputFlags{ ImGuiInputTextFlags_None };
					static int selectedKernelIndex{ 0 };
					const char* kernelNames[4] = { "Custom", "Blur", "Edge-Detection", "Sharpen" };

					{
						UI::ScopedWidth sliderWidth{ availableWidth - resetButtonTextWidth };
						ImGui::SliderInt("##idKernelEnumSlider", &selectedKernelIndex, 0, 3, [&]() -> const char* {

						if (selectedKernelIndex == 0) {

							kernelMatrixInputFlags = ImGuiInputTextFlags_None;
							selectedKernel = userKernel;
							return kernelNames[0];
						}
						else if (selectedKernelIndex == 1) {

							kernelMatrixInputFlags = ImGuiInputTextFlags_ReadOnly;
							selectedKernel = Core::ImageProcessing::KernelBlur;
							return kernelNames[1];
						}
						else if (selectedKernelIndex == 2) {

							kernelMatrixInputFlags = ImGuiInputTextFlags_ReadOnly;
							selectedKernel = Core::ImageProcessing::KernelEdgeDetection;
							return kernelNames[2];
						}
						else if (selectedKernelIndex == 3) {

							kernelMatrixInputFlags = ImGuiInputTextFlags_ReadOnly;
							selectedKernel = Core::ImageProcessing::KernelSharpen;
							return kernelNames[3];
						}

						return "Invalid";
						}());
					}

					ImGui::SameLine();

					{
						UI::ScopedID kernelMatrixResetId{ "idKernelMatrixReset" };
						if (ImGui::Button("Reset", { ImGui::GetContentRegionAvail().x, 0.0f })) {

							kernelMatrixInputFlags = ImGuiInputTextFlags_None;
							userKernel = { 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f };
							selectedKernel = userKernel;
							selectedKernelIndex = 0;
						}
						if (selectedKernelIndex == 0) {

							selectedKernel = userKernel;
						}
					}

					{
						UI::ScopedWidth inputWidth{ ImGui::GetContentRegionAvail().x };
						{
							UI::ScopedID kernelMatrixRowId{ "idKernelMatrixRow0" };
							ImGui::InputFloat3("", selectedKernel.data(), "%.6f", kernelMatrixInputFlags);
						}
						{
							UI::ScopedID kernelMatrixRowId{ "idKernelMatrixRow1" };
							ImGui::InputFloat3("", selectedKernel.data() + 3, "%.6f", kernelMatrixInputFlags);
						}
						{
							UI::ScopedID kernelMatrixRowId{ "idKernelMatrixRow2" };
							ImGui::InputFloat3("", selectedKernel.data() + 6, "%.6f", kernelMatrixInputFlags);
						}

						if (selectedKernelIndex == 0) {

							userKernel = selectedKernel;
						}
					}

					kernelMatrixEditorHeight = ImGui::GetCursorPosY() - kernelMatrixEditorHeight - itemSpacing.y;
				}

				// Buttons
				{
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);

					if (ImGui::Button("Select Images for Stacking", { ImGui::GetContentRegionAvail().x, textSize * 1.4f })) {

						const auto selectedImages = Utils::File::OpenFileDialog("Select Images", true);

						if(!selectedImages.empty()) {

							textureList.clear();
							for(const auto& currentImagePath : selectedImages) {

								const auto currentTexture = std::make_shared<Core::OpenGL::Texture>();
								if (currentTexture->LoadFromFile(currentImagePath)) {

									textureList.emplace_back(currentTexture);
								}
							}

							if(!Core::ImageProcessing::Stack(stackFrameBuffer, textureList)) {

							}
						}
					}

					ImGui::TableSetColumnIndex(1);

					if (ImGui::Button("Apply Kernel", {ImGui::GetContentRegionAvail().x, textSize * 1.4f })) {

						if (!Core::ImageProcessing::Kernel(kernelFrameBuffer, stackFrameBuffer, selectedKernel)) {

						}
					}
				}
				ImGui::EndTable();
			}
		}
		ImGui::End();
	}

	void ImageProcessingView::OnDestroy() noexcept {

	}
}