#include "ImageProcessingView.hpp"

namespace StarTracker {

    ImageProcessingView::ImageProcessingView(void *nativeWindowHandle) noexcept : Core::View{nativeWindowHandle} {

    }

    void ImageProcessingView::OnInit() noexcept {

        // Initialize ImageProcessing FrameBuffer
        targetFrameBuffer = std::make_shared<Core::OpenGL::FrameBuffer>(100, 100);

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

        if (ImGui::Begin("Image Processing")) {

            const auto textSize = ImGui::GetFontSize();
            const auto width = ImGui::GetContentRegionAvail().x;
            if (ImGui::Button("Select Images", { width, textSize * 1.4f })) {

                const auto selectedImages = Utils::File::OpenFileDialog("Select Images", true);

                std::vector<std::shared_ptr<Core::OpenGL::Texture>> textureList{};
                for(const auto& currentImagePath : selectedImages) {

                    const auto currentTexture = std::make_shared<Core::OpenGL::Texture>();
                    currentTexture->LoadFromFile(currentImagePath);
                    textureList.emplace_back(currentTexture);
                }

                if(!Core::ImageProcessing::Stack(targetFrameBuffer, textureList)) {

                    std::fprintf(stderr, "Unable to Stack Images!\n");
                }
            }

            static std::array<float, 9> kernel{};
            ImGui::InputFloat3("Kernel-Row-0", kernel.data());
            ImGui::Separator();
            ImGui::InputFloat3("Kernel-Row-1", kernel.data() + 3);
            ImGui::Separator();
            ImGui::InputFloat3("Kernel-Row-2", kernel.data() + 6);

            if (ImGui::Button("Apply Kernel", { width, textSize * 1.4f })) {

                if (!Core::ImageProcessing::Kernel(targetFrameBuffer, kernel)) {

                    std::fprintf(stderr, "Unable to Apply Kernel!\n");
                }
            }

            const auto stackedImageId = static_cast<std::intptr_t>(targetFrameBuffer->GetNativeTextureHandle());
            const auto[textureWidth, textureHeight] = [&]() -> std::pair<float, float> {

                const auto frameBufferWidth = static_cast<float>(targetFrameBuffer->GetWidth());
                const auto frameBufferHeight = static_cast<float>(targetFrameBuffer->GetHeight());
                const auto availableSize = ImGui::GetContentRegionAvail();

                if(frameBufferWidth > frameBufferHeight) {

                    const auto width = availableSize.x;
                    const auto height = frameBufferHeight / frameBufferWidth * width;
                    return { width, height };
                }
                else {

                    const auto height = availableSize.y;
                    const auto width = frameBufferWidth / frameBufferHeight * height;
                    return { width, height };
                }
            }();
            ImGui::Image(reinterpret_cast<void*>(stackedImageId), {textureWidth, textureHeight }, { 0, 1 }, { 1, 0 });
        }
        ImGui::End();
    }

    void ImageProcessingView::OnDestroy() noexcept {

    }
}

