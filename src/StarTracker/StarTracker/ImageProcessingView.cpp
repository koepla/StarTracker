#include "ImageProcessingView.hpp"

namespace StarTracker {

    ImageProcessingView::ImageProcessingView(void *nativeWindowHandle) noexcept : Core::View{nativeWindowHandle}, textureList{} {

    }

    void ImageProcessingView::OnInit() noexcept {

        // Initialize ImageProcessing FrameBuffer
        stackFrameBuffer = std::make_shared<Core::OpenGL::FrameBuffer>(160, 90);
        kernelFrameBuffer = std::make_shared<Core::OpenGL::FrameBuffer>(160, 90);

        const auto application = Core::Application::GetInstance();
        application->RegisterEventHandler([&](const Core::Events::Event& event) -> void {

            const auto keyEvent = dynamic_cast<const Core::Events::KeyEvent*>(&event);

            if(keyEvent) {

                if(keyEvent->IsPressed() && keyEvent->GetKeyCode() == Core::KeyCode::Escape) {

                    application->Close();
                }
            }
        });
    }

    void ImageProcessingView::OnUpdate(float deltaTime) noexcept {

        const auto textSize = ImGui::GetFontSize();
        const auto availableSize = ImGui::GetContentRegionAvail();

        static float verticalButtonPosition{ 0.0f };

        if (ImGui::Begin("Image Processing")) {

            ImGui::PushID("ImageProcessingAlignmentTable");
            if (ImGui::BeginTable("", 2))
            {
                ImGui::TableNextRow();

                {   // Final Images
                    ImGui::PushFont(Core::UIFont::Medium);

                    const auto stackedImageId = static_cast<std::intptr_t>(stackFrameBuffer->GetNativeTextureHandle());
                    const auto kernelImageId = static_cast<std::intptr_t>(kernelFrameBuffer->GetNativeTextureHandle());

                    const auto textureWidth = ImGui::GetContentRegionAvail().x / 2.0f;
                    const auto textureHeight = static_cast<float>(stackFrameBuffer->GetHeight()) / static_cast<float>(stackFrameBuffer->GetWidth()) * textureWidth;

                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("Stacked Image");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("Kernel Image");
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Image(reinterpret_cast<void*>(stackedImageId), {textureWidth, textureHeight }, { 0, 1 }, { 1, 0 });
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Image(reinterpret_cast<void*>(kernelImageId), {textureWidth, textureHeight }, { 0, 1 }, { 1, 0 });

                    ImGui::PopFont();
                }

                {   // Stacking
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);

                    ImGui::PushFont(Core::UIFont::Medium);
                    ImGui::Text("Image List");
                    ImGui::PopFont();

                    for (const auto& currentTexture : textureList) {

                        const auto textureHeight = 5.0f * textSize;
                        const auto textureWidth = static_cast<float>(currentTexture->GetWidth()) / static_cast<float>(currentTexture->GetHeight()) * textureHeight;

                        if ((ImGui::GetCursorPosX() + textureWidth) >= (availableSize.x / 2.0f)) {

                            ImGui::NewLine();
                        }

                        ImGui::Image(reinterpret_cast<void*>(static_cast<std::intptr_t>(currentTexture->GetNativeHandle())), {textureWidth, textureHeight }, { 0, 1 }, { 1, 0 });
                        ImGui::SameLine();
                    }

                    if (!textureList.empty()) {

                        ImGui::NewLine();
                        verticalButtonPosition = ImGui::GetCursorPosY();
                    }
                    else {

                        ImGui::SetCursorPosY(verticalButtonPosition);
                    }

                    if (ImGui::Button("Select Images for Stacking", {availableSize.x / 2.0f, textSize * 1.4f })) {

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
                }

                {   // Kernel
                    ImGui::TableSetColumnIndex(1);

                    ImGui::PushFont(Core::UIFont::Medium);
                    ImGui::Text("Kernel Matrix");
                    ImGui::PopFont();

                    static std::array<float, 9> kernel{ 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f };
                    ImGui::PushItemWidth(availableSize.x / 2.0f);
                    ImGui::PushID("Kernel-Row-0");
                    ImGui::InputFloat3("", kernel.data(), "%.6f");
                    ImGui::PopID();
                    ImGui::Separator();
                    ImGui::PushID("Kernel-Row-1");
                    ImGui::InputFloat3("", kernel.data() + 3, "%.6f");
                    ImGui::PopID();
                    ImGui::Separator();
                    ImGui::PushID("Kernel-Row-2");
                    ImGui::InputFloat3("", kernel.data() + 6, "%.6f");
                    ImGui::PopID();
                    ImGui::PopItemWidth();

                    if (!textureList.empty()) {

                        ImGui::SetCursorPosY(verticalButtonPosition);
                    }
                    else {

                        verticalButtonPosition = ImGui::GetCursorPosY();
                    }

                    if (ImGui::Button("Apply Kernel", {availableSize.x / 2.0f, textSize * 1.4f })) {

                        if (!Core::ImageProcessing::Kernel(kernelFrameBuffer, stackFrameBuffer, kernel)) {

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

