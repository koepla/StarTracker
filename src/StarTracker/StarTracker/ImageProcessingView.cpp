#include "ImageProcessingView.hpp"

namespace StarTracker {

    ImageProcessingView::ImageProcessingView(void *nativeWindowHandle) noexcept : Core::View{nativeWindowHandle} {

    }

    void ImageProcessingView::OnInit() noexcept {

        // Initialize ImageProcessing Buffers
        frameBuffer = std::make_shared<Core::OpenGL::FrameBuffer>(100, 100);
        blue = Core::AssetDataBase::LoadTexture("blue.png");
        pillars = Core::AssetDataBase::LoadTexture("pillarsOfCreation.jpg");
    }

    void ImageProcessingView::OnUpdate(float deltaTime) noexcept {

        if (ImGui::Begin("Image Processing")) {

            const auto textSize = ImGui::GetFontSize();
            const auto availableSize = ImGui::GetContentRegionAvail();
            if (ImGui::Button("Stack Images", { availableSize.x, textSize * 1.4f })) {

                if(!Core::ImageProcessing::Stack(frameBuffer, { blue, pillars })) {

                    std::fprintf(stderr, "Couldn't Stack Textures!\n");
                }
            }

            const auto stackedImageId = static_cast<std::intptr_t>(frameBuffer->GetNativeTextureHandle());
            const auto[textureWidth, textureHeight] = [&]() -> std::pair<float, float> {

                const auto frameBufferWidth = static_cast<float>(frameBuffer->GetWidth());
                const auto frameBufferHeight = static_cast<float>(frameBuffer->GetHeight());
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

