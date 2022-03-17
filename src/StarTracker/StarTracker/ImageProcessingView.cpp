#include "ImageProcessingView.hpp"

namespace StarTracker {

    ImageProcessingView::ImageProcessingView(void *nativeWindowHandle) noexcept : Core::View{nativeWindowHandle} {

    }

    void ImageProcessingView::OnInit() noexcept {

        // Initialize ImageProcessing FrameBuffer
        targetFrameBuffer = std::make_shared<Core::OpenGL::FrameBuffer>(100, 100);
    }

    void ImageProcessingView::OnUpdate(float deltaTime) noexcept {

        if (ImGui::Begin("Image Processing")) {

            const auto textSize = ImGui::GetFontSize();
            const auto availableSize = ImGui::GetContentRegionAvail();
            if (ImGui::Button("Select Images", { availableSize.x, textSize * 1.4f })) {

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

