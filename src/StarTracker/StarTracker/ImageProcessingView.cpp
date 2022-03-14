#include "ImageProcessingView.hpp"

namespace StarTracker {

    ImageProcessingView::ImageProcessingView(void *nativeWindowHandle) noexcept : Core::View{nativeWindowHandle}, textureList{} {

    }

    void ImageProcessingView::OnInit() noexcept {

        // Initialize ImageProcessing Buffers
        ASSERT(Core::ImageProcessing::Initialize() && "Couldn't initialize ImageProcessing");
    }

    void ImageProcessingView::OnUpdate(float deltaTime) noexcept {

        if (ImGui::Begin("Image Processing")) {

            const auto textSize = ImGui::GetFontSize();
            const auto availableSize = ImGui::GetContentRegionAvail();
            if (ImGui::Button("Select Images", { availableSize.x, textSize * 1.4f })) {

                const auto result = Utils::File::OpenFileDialog(true);

                std::printf("Found %d textures!\n", static_cast<int>(result.size()));

                textureList.clear();
                for(const auto& path : result) {

                    auto texture = std::make_shared<Core::OpenGL::Texture>();
                    texture->LoadFromFile(path);
                    textureList.emplace_back(texture);
                }

                if (!Core::ImageProcessing::Stack(textureList)->IsValid()) {

                    ImGui::OpenPopup("ImageStackError");
                }

                bool popUpOpen = true;
                if (ImGui::BeginPopupModal("ImageStackError", &popUpOpen, ImGuiWindowFlags_AlwaysAutoResize)) {

                    ImGui::Text("Couldn't Stack Images!");
                    ImGui::EndPopup();
                }
            }

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

