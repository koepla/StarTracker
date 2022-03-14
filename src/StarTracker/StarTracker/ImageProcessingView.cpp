#include "ImageProcessingView.hpp"

namespace StarTracker {

    ImageProcessingView::ImageProcessingView(void *nativeWindowHandle) noexcept : Core::View{nativeWindowHandle} {

    }

    void ImageProcessingView::OnInit() noexcept {

        blueTexture = Core::AssetDataBase::LoadTexture("blue.png");
        pillarsTexture = Core::AssetDataBase::LoadTexture("pillarsOfCreation.jpg");
        ASSERT(Core::ImageProcessing::Stack({ blueTexture, pillarsTexture })->IsValid());
    }

    void ImageProcessingView::OnUpdate(float deltaTime) noexcept {

        if (ImGui::Begin("Image Processing")) {

            const auto stackedImageId = static_cast<std::intptr_t>(Core::ImageProcessing::GetStackFrameBuffer()->GetNativeTextureHandle());
            const auto[textureWidth, textureHeight] = [&]() -> std::pair<float, float> {

                const auto frameBuffer = Core::ImageProcessing::GetStackFrameBuffer();
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

