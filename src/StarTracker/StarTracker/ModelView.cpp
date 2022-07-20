#include "ModelView.hpp"

namespace StarTracker {

    ModelView::ModelView(void* nativeWindowHandle) noexcept : Core::View{ nativeWindowHandle }, isFocused{ false }, invertedTextureCoordinates{ false }, camera{}, shader{}, frameBuffer{}, model{} {

    }

    void ModelView::OnInit() noexcept {

        const auto application = Core::Application::GetInstance();
        const auto windowWidth = application->GetWindow().GetWidth();
        const auto windowHeight = application->GetWindow().GetHeight();

        constexpr auto initialCameraPosition = glm::vec3{ 0.0f, 0.0f, 10.0f };
        camera = std::make_shared<Core::OpenGL::Camera>(initialCameraPosition);
        frameBuffer = std::make_shared<Core::OpenGL::FrameBuffer>(windowWidth, windowHeight);
        shader = Core::AssetDataBase::LoadShader("modelVertex.glsl", "modelFragment.glsl");
        model = Core::AssetDataBase::LoadModel("StarTrackerMount.obj");
    }

    void ModelView::OnUpdate(float deltaTime) noexcept {

        if (ImGui::Begin("3D Model Viewer")) {

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

    void ModelView::OnDestroy() noexcept {

    }

    void ModelView::drawModelInfoCard(const glm::vec2& size) noexcept {

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
            const auto cardInfo = "3D Model Viewer (Wavefront OBJ)";
            UI::DrawCursor::Advance({ itemInnerSpacing.x, 0.0f });
            UI::Text::Draw(cardInfo, UI::Font::Medium, fontSize, baseTextColor);

            // Path-Info
            const auto modelPathInfo = fmt::format("File: {}", model->GetFilePath().filename().string());
            UI::DrawCursor::Advance({ 0.0f, fontSize + regulatedItemSpacing });
            UI::Text::Draw(modelPathInfo, UI::Font::Regular, smallFontSize, baseTextLightColor);

            // Geometry-Info
            const auto& modelGeometry = model->GetGeometryInfo();
            const auto modelGeometryInfo = fmt::format("Geometry: {} indices, {} vertices", modelGeometry.IndexCount, modelGeometry.VertexCount);
            UI::DrawCursor::Advance({ 0.0f, smallFontSize + regulatedItemSpacing });
            UI::Text::Draw(modelGeometryInfo, UI::Font::Regular, smallFontSize, baseTextLightColor);
        }
        ImGui::EndChild();
    }
}