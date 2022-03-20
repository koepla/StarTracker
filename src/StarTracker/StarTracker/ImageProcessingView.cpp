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

        const auto textSize = ImGui::GetFontSize();
        static float kernelMatrixEditorHeight{ 0.0f };

        if (ImGui::Begin("Image Processing")) {

            ImGui::PushID("ImageProcessingAlignmentTable");
            if (ImGui::BeginTable("", 2))
            {
                // Row-0
                ImGui::TableNextRow();

                {   // Final Images
                    ImGui::PushFont(Core::UIFont::Medium);

                    const auto stackedImageId = static_cast<std::intptr_t>(stackFrameBuffer->GetNativeTextureHandle());
                    const auto kernelImageId = static_cast<std::intptr_t>(kernelFrameBuffer->GetNativeTextureHandle());

                    const auto textureWidth = ImGui::GetContentRegionAvail().x / 2.0f;
                    const auto textureHeight = static_cast<float>(stackFrameBuffer->GetHeight()) / static_cast<float>(stackFrameBuffer->GetWidth()) * textureWidth;

                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("Stacked Image");
                    ImGui::Image(reinterpret_cast<void*>(stackedImageId), {textureWidth, textureHeight }, { 0, 1 }, { 1, 0 });

                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("Kernel Image");
                    ImGui::Image(reinterpret_cast<void*>(kernelImageId), {textureWidth, textureHeight }, { 0, 1 }, { 1, 0 });

                    ImGui::PopFont();
                }

                // Row-1
                ImGui::TableNextRow();

                {   // Stacking
                    ImGui::TableSetColumnIndex(0);
                    ImGui::PushFont(Core::UIFont::Medium);
                    ImGui::Text("Loaded Images");
                    ImGui::PopFont();

                    const auto availableWidth = ImGui::GetContentRegionAvail().x;
                    ImGui::BeginChild("ImageListChildElement", ImVec2(ImGui::GetContentRegionAvail().x, kernelMatrixEditorHeight), false, ImGuiWindowFlags_HorizontalScrollbar);
                    for (const auto& currentTexture : textureList) {

                        const auto textureHeight = 5.0f * textSize;
                        const auto textureWidth = static_cast<float>(currentTexture->GetWidth()) / static_cast<float>(currentTexture->GetHeight()) * textureHeight;

                        if ((ImGui::GetCursorPosX() + textureWidth) >= (availableWidth)) {

                            ImGui::NewLine();
                        }

                        ImGui::Image(reinterpret_cast<void*>(static_cast<std::intptr_t>(currentTexture->GetNativeHandle())), {textureWidth, textureHeight }, { 0, 1 }, { 1, 0 });

                        if (currentTexture != textureList.back()) {

                            ImGui::SameLine();
                        }
                    }
                    ImGui::EndChild();
                }

                static std::array<float, 9> userKernel{ 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f };
                static std::array<float, 9> selectedKernel{ userKernel };

                {
                    // Kernel
                    ImGui::TableSetColumnIndex(1);

                    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                    ImGui::PushFont(Core::UIFont::Medium);
                    ImGui::Text("Kernel Matrix");
                    ImGui::PopFont();
                    ImGui::PopItemWidth();

                    const auto availableWidth = ImGui::GetContentRegionAvail().x;
                    const auto resetButtonTextWidth = ImGui::CalcTextSize("Reset").x * 2.5f;

                    kernelMatrixEditorHeight = ImGui::GetCursorPosY();
                    ImGui::PushItemWidth(availableWidth - resetButtonTextWidth);
                    static ImGuiInputTextFlags_ kernelMatrixInputFlags{ ImGuiInputTextFlags_None };
                    static int selectedKernelIndex{ 0 };
                    const char* kernelNames[4] = { "Custom", "Blur", "Edge-Detection", "Sharpen" };
                    ImGui::PushID("KernelEnumSlider");
                    ImGui::SliderInt("", &selectedKernelIndex, 0, 3, [&]() -> const char* {

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

                    }());
                    ImGui::PopID();
                    ImGui::PopItemWidth();

                    ImGui::SameLine();

                    ImGui::PushID("KernelMatrixResetButton");
                    if (ImGui::Button("Reset", { ImGui::GetContentRegionAvail().x, 0.0f })) {

                        kernelMatrixInputFlags = ImGuiInputTextFlags_None;
                        userKernel = { 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f };
                        selectedKernel = userKernel;
                        selectedKernelIndex = 0;
                    }
                    ImGui::PopID();

                    if (selectedKernelIndex == 0) {

                        selectedKernel = userKernel;
                    }

                    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                    ImGui::PushID("Kernel-Row-0");
                    ImGui::InputFloat3("", selectedKernel.data(), "%.6f", kernelMatrixInputFlags);
                    ImGui::PopID();
                    ImGui::PushID("Kernel-Row-1");
                    ImGui::InputFloat3("", selectedKernel.data() + 3, "%.6f", kernelMatrixInputFlags);
                    ImGui::PopID();
                    ImGui::PushID("Kernel-Row-2");
                    ImGui::InputFloat3("", selectedKernel.data() + 6, "%.6f", kernelMatrixInputFlags);
                    ImGui::PopID();
                    ImGui::PopItemWidth();

                    if (selectedKernelIndex == 0) {

                        userKernel = selectedKernel;
                    }

                    kernelMatrixEditorHeight = ImGui::GetCursorPosY() - kernelMatrixEditorHeight;
                }

                // Buttons
                {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);

                    if (ImGui::Button("Select Images for Stacking", {ImGui::GetContentRegionAvail().x, textSize * 1.4f })) {

                        const auto selectedImages = Utils::File::OpenFileDialog("Select Images", true);

                        textureList.clear();
                        for(const auto& currentImagePath : selectedImages) {

                            const auto currentTexture = std::make_shared<Core::OpenGL::Texture>();
                            currentTexture->LoadFromFile(currentImagePath);
                            textureList.emplace_back(currentTexture);
                        }

                        if(!Core::ImageProcessing::Stack(stackFrameBuffer, textureList)) {

                            std::fprintf(stderr, "Unable to Stack Images!\n");
                        }
                    }

                    ImGui::TableSetColumnIndex(1);

                    if (ImGui::Button("Apply Kernel", {ImGui::GetContentRegionAvail().x, textSize * 1.4f })) {

                        if (!Core::ImageProcessing::Kernel(kernelFrameBuffer, stackFrameBuffer, selectedKernel)) {

                            std::fprintf(stderr, "Unable to Apply Kernel!\n");
                        }
                    }
                }
                ImGui::EndTable();
            }
            ImGui::PopID();
        }
        ImGui::End();
    }

    void ImageProcessingView::OnDestroy() noexcept {

    }
}

