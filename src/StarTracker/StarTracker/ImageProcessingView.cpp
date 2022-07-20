#include "ImageProcessingView.hpp"

namespace StarTracker {

    ImageProcessingView::ImageProcessingView(void* nativeWindowHandle) noexcept : Core::View{ nativeWindowHandle }, textureList{}, filterList{}, renderFrameBuffer{}, renderVertexArray{}, renderVertexBuffer{}, renderIndexBuffer{} {

    }

    void ImageProcessingView::OnInit() noexcept {

        // Initialize ImageProcessing FrameBuffer
        renderFrameBuffer = std::make_shared<Core::OpenGL::FrameBuffer>(160, 90);
        copyShader = Core::AssetDataBase::LoadShader("copyTextureVertex.glsl", "copyTextureFragment.glsl");

        // Initialize OpenGL Buffers for showing a specific texture
        initializeBuffers();
    }

    void ImageProcessingView::OnUpdate(float deltaTime) noexcept {

        const auto& style = ImGui::GetStyle();
        const auto textSize = ImGui::GetFontSize();
        const auto itemSpacing = ImGui::GetStyle().ItemSpacing;
        const auto itemInnerSpacing = ImGui::GetStyle().ItemInnerSpacing;

        if (ImGui::BeginMainMenuBar()) {

            if (ImGui::BeginMenu("Edit")) {

                {
                    UI::ScopedFont mediumFont{ UI::Font::Medium };
                    ImGui::Text("Image Processing");
                }
                if (ImGui::MenuItem("Stack All", "Alt+S")) {

                    if (!Core::ImageProcessing::Stack(renderFrameBuffer, textureList)) {

                        STARTRACKER_WARN("Couldn't stack images");
                    }
                }
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

        if (ImGui::Begin("Image Processing", nullptr, ImGuiWindowFlags_NoScrollbar)) {

            const auto flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable;
            if (ImGui::BeginTable("##idChildImageProcessingAlignmentTable", 3, flags)) {

                const auto totalWidth = ImGui::GetContentRegionAvail().x;

                ImGui::TableSetupColumn("##idColumnFilterPanel", ImGuiTableColumnFlags_WidthStretch, 0.25f * totalWidth);
                ImGui::TableSetupColumn("##idColumnTextureListPanel", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupColumn("##idColumnRenderFrameBufferPanel", ImGuiTableColumnFlags_WidthStretch, 0.25f * totalWidth);

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

                    ImGui::TableSetColumnIndex(2);
                    {
                        drawFilterPanel();
                    }
                }

                ImGui::EndTable();
            }
        }
        ImGui::End();
    }

    void ImageProcessingView::OnDestroy() noexcept {

    }

    void ImageProcessingView::initializeBuffers() noexcept {

        renderVertexArray = std::make_shared<Core::OpenGL::VertexArray>();
        renderVertexBuffer = std::make_shared<Core::OpenGL::VertexBuffer>();
        renderIndexBuffer = std::make_shared<Core::OpenGL::IndexBuffer>();

        // Fixed texture vertices
        const static std::array<Core::OpenGL::TextureVertex, 4> vertices = {

            Core::OpenGL::TextureVertex{ glm::vec3{ -1.0f, -1.0f, 0.0f }, glm::vec3{ 1.0f, 1.0f, 1.0f }, glm::vec2{ 0.0f, 0.0f }},
            Core::OpenGL::TextureVertex{ glm::vec3{  1.0f, -1.0f, 0.0f }, glm::vec3{ 1.0f, 1.0f, 1.0f }, glm::vec2{ 1.0f, 0.0f }},
            Core::OpenGL::TextureVertex{ glm::vec3{  1.0f,  1.0f, 0.0f }, glm::vec3{ 1.0f, 1.0f, 1.0f }, glm::vec2{ 1.0f, 1.0f }},
            Core::OpenGL::TextureVertex{ glm::vec3{ -1.0f,  1.0f, 0.0f }, glm::vec3{ 1.0f, 1.0f, 1.0f }, glm::vec2{ 0.0f, 1.0f }}
        };

        // Fixed indices for drawing the rectangle
        const static std::array<std::uint32_t, 6> indices = {

                0, 1, 2, 2, 0, 3
        };

        // Buffer elements that match the Shader
        const static std::vector<Core::OpenGL::BufferElement> vertexBufferElements = {

                Core::OpenGL::BufferElement{ Core::OpenGL::ShaderDataType::Float3, "aPosition" },
                Core::OpenGL::BufferElement{ Core::OpenGL::ShaderDataType::Float3, "aColor" },
                Core::OpenGL::BufferElement{ Core::OpenGL::ShaderDataType::Float2, "aTextureCoordinates" }
        };

        // BufferLayout containing the elements
        const static Core::OpenGL::BufferLayout vertexBufferLayout{ vertexBufferElements };

        // Set up the VertexArray
        renderVertexArray->Bind();
        renderVertexBuffer->SetLayout(vertexBufferLayout);
        renderVertexBuffer->SetData(vertices.data(), static_cast<std::uint32_t>(vertices.size() * sizeof(Core::OpenGL::TextureVertex)));
        renderIndexBuffer->SetData(indices.data(), static_cast<std::uint32_t>(indices.size()));
        renderVertexArray->SetIndexBuffer(renderIndexBuffer);
        renderVertexArray->SetVertexBuffer(renderVertexBuffer);
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
        // due to the fact that the UI function for drawing textures
        // expects the native texture handle, and we cannot keep textures alive
        // by simply passing the texture handle.
        // Therefore we have to add textures that are meant to be removed to this list,
        // as their presence keeps them alive after being erased from textureList,
        // so the texture is not destroyed and ImGui can draw it in its draw call
        // at the end of the frame
        static std::vector<std::shared_ptr<Core::OpenGL::Texture>> keepAliveTextureList{};
        keepAliveTextureList.clear();

        if (ImGui::BeginChild("idChildTextureListPanel", ImGui::GetContentRegionAvail(), false, ImGuiWindowFlags_NoScrollbar)) {

            UI::ScopedColor childBackground{ ImGuiCol_ChildBg, style.Colors[ImGuiCol_FrameBg] };

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

            UI::ScopedColor transparentBackground{ ImGuiCol_ChildBg, { 0.0f, 0.0f, 0.0f, 0.0f} };

            if (ImGui::BeginChild("idChildTextureList", ImGui::GetContentRegionAvail(), false, ImGuiWindowFlags_HorizontalScrollbar)) {

                UI::DrawCursor::Advance({ itemInnerSpacing.x, 0.0f });

                for (std::size_t i = 0; i < textureList.size(); i++) {

                    const auto& currentTexture = textureList.at(i);

                    const auto textureHeight = 4.0f * fontSize;
                    const auto textureWidth = static_cast<float>(currentTexture->GetWidth()) / static_cast<float>(currentTexture->GetHeight()) * textureHeight;

                    if (textureWidth > ImGui::GetContentRegionAvail().x) {

                        ImGui::NewLine();
                        UI::DrawCursor::Advance({ itemInnerSpacing.x, 0.0f });
                    }

                    const auto textureChildContainerId = fmt::format("idChildTexture{}", currentTexture->GetNativeHandle());
                    if (ImGui::BeginChild(textureChildContainerId.c_str(), { textureWidth, textureHeight }, false, ImGuiWindowFlags_NoScrollbar)) {

                        UI::Image::DrawRounded(currentTexture->GetNativeHandle(), { textureWidth, textureHeight });
                    }
                    ImGui::EndChild();

                    const auto textureRemovePopupId = fmt::format("idRemoveTexturePopup{}", currentTexture->GetNativeHandle());

                    if (ImGui::IsItemHovered()) {

                        if (Core::Input::IsMousePressed(Core::MouseCode::ButtonRight)) {

                            ImGui::OpenPopup(textureRemovePopupId.c_str());
                        }

                        ImGui::BeginTooltip();

                        const auto& path = currentTexture->GetFilePath();
                        const auto width = currentTexture->GetWidth();
                        const auto height = currentTexture->GetHeight();
                        const auto tooltip = fmt::format("{} - {}x{}", path.filename().string(), width, height);
                        ImGui::Text("%s", tooltip.c_str());

                        ImGui::EndTooltip();
                    }

                    if (ImGui::BeginPopup(textureRemovePopupId.c_str())) {

                        if (ImGui::MenuItem("Remove")) {

                            keepAliveTextureList.emplace_back(currentTexture);
                            textureList.erase(textureList.begin() + i);
                        }
                        if (ImGui::MenuItem("Show")) {

                            const auto textureHandle = currentTexture->GetNativeHandle();
                            const auto textureWidth = currentTexture->GetWidth();
                            const auto textureHeight = currentTexture->GetHeight();
                            showTexture(textureHandle, { textureWidth, textureHeight });
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

        if (ImGui::BeginChild("idChildFrameBufferPanel", ImGui::GetContentRegionAvail(), false, ImGuiWindowFlags_NoScrollbar)) {

            if (ImGui::BeginChild("idChildFrameBufferPanelHeader", { ImGui::GetContentRegionAvail().x, fontSize }, false, ImGuiWindowFlags_NoScrollbar)) {

                UI::DrawCursor::Advance({ itemInnerSpacing.x, 0.0f });
                UI::Text::Draw("Result", UI::Font::Medium, fontSize, baseTextColor);
            }
            ImGui::EndChild();

            const auto textureHandle = renderFrameBuffer->GetNativeTextureHandle();
            const auto textureWidth = ImGui::GetContentRegionAvail().x;
            const auto textureHeight = ImGui::GetContentRegionAvail().y;
            UI::Image::DrawRounded(textureHandle, { textureWidth, textureHeight });
        }
        ImGui::EndChild();
    }

    void ImageProcessingView::drawFilterPanel() noexcept {

        const auto& style = ImGui::GetStyle();
        const auto itemSpacing = style.ItemSpacing;
        const auto itemInnerSpacing = style.ItemInnerSpacing;

        if (ImGui::BeginChild("idChildFilterPanel", ImGui::GetContentRegionAvail(), false, ImGuiWindowFlags_NoScrollbar)) {

            drawFilterHierarchyPanel();

            if (ImGui::CollapsingHeader("Kernel", ImGuiTreeNodeFlags_DefaultOpen)) {

                drawFilterKernel();
            }
            if (ImGui::CollapsingHeader("Stack", ImGuiTreeNodeFlags_DefaultOpen)) {

                drawFilterStack();
            }
        }
        ImGui::EndChild();
    }
    void ImageProcessingView::drawFilterHierarchyPanel() noexcept {

        const auto& style = ImGui::GetStyle();
        const auto fontSize = ImGui::GetFontSize();
        const auto itemSpacing = style.ItemSpacing;
        const auto itemInnerSpacing = style.ItemInnerSpacing;
        const auto& baseTextColor = style.Colors[ImGuiCol_Text];
        const auto size = ImGui::GetContentRegionAvail();

        // We need this because we want to dynamically remove textures from the textureList
        // due to the fact that the UI function for drawing Images
        // expects the native texture handle, we cannot keep textures alive
        // by simply passing the texture handle.
        // Therefore we have to add textures that are meant to be removed to this list,
        // as their presence keeps them alive after being erased from textureList,
        // so the texture is not destroyed and ImGui can draw it in its draw call
        static std::vector<std::shared_ptr<Core::OpenGL::FrameBuffer>> keepAliveTargetList{};
        keepAliveTargetList.clear();

        UI::ScopedColor childBackground{ ImGuiCol_ChildBg, style.Colors[ImGuiCol_FrameBg] };

        if (ImGui::BeginChild("idChildFilterHierarchyPanelHeader", { size.x, 0.45f * size.y }, false, ImGuiWindowFlags_NoScrollbar)) {

            UI::DrawCursor::Advance({ itemInnerSpacing.x, 0.0f });
            UI::Text::Draw("Active Filters", UI::Font::Medium, fontSize, baseTextColor);
            UI::DrawCursor::Advance({ -itemInnerSpacing.x, fontSize + itemSpacing.y });

            if (ImGui::BeginChild("idChildFilterHierarchyPanel", ImGui::GetContentRegionAvail(), false, ImGuiWindowFlags_HorizontalScrollbar)) {

                for (std::size_t i = 0; i < filterList.size(); i++) {

                    const auto& filter = filterList.at(i);
                    const auto label = fmt::format("{}", filter.Name);
                    const auto treeNodeId = fmt::format("idTreeNodeFilterHierarchyPanel{}", i);

                    if (ImGui::TreeNode(treeNodeId.c_str(), label.c_str())) {

                        const auto childId = fmt::format("idChildFilterHierarchyPanelFilterElem{}", i);
                        const auto size = ImGui::GetContentRegionAvail();
                        const auto targetWidth = filter.Target->GetWidth();
                        const auto targetHeight = filter.Target->GetHeight();
                        const auto drawHeight = 4.0f * fontSize;
                        const auto drawWidth = drawHeight * targetWidth / targetHeight;

                        if (ImGui::BeginChild(childId.c_str(), { drawWidth, drawHeight }, false)) {

                            const auto textureHandle = filter.Target->GetNativeTextureHandle();
                            UI::Image::DrawRounded(textureHandle, { drawWidth, drawHeight });
                        }
                        ImGui::EndChild();
                        ImGui::TreePop();
                    }

                    const auto treeNodePopupId = fmt::format("idTreeNodePopupFilterHierarchyPanelFilterElem{}", i);
                    if (ImGui::IsItemHovered()) {

                        if (Core::Input::IsMousePressed(Core::MouseCode::ButtonRight)) {

                            ImGui::OpenPopup(treeNodePopupId.c_str());
                        }
                    }

                    if (ImGui::BeginPopup(treeNodePopupId.c_str())) {

                        if (ImGui::MenuItem("Remove")) {

                            keepAliveTargetList.emplace_back(filter.Target);
                            filterList.erase(filterList.begin() + i);
                        }
                        if (ImGui::MenuItem("Show")) {

                            const auto textureHandle = filter.Target->GetNativeTextureHandle();
                            const auto textureWidth = filter.Target->GetWidth();
                            const auto textureHeight = filter.Target->GetHeight();
                            showTexture(textureHandle, { textureWidth, textureHeight });
                        }
                        ImGui::EndPopup();
                    }
                }
            }
            ImGui::EndChild();
        }
        ImGui::EndChild();
    }

    void ImageProcessingView::drawFilterKernel() noexcept {

        const auto& style = ImGui::GetStyle();
        const auto itemSpacing = style.ItemSpacing;
        const auto itemInnerSpacing = style.ItemInnerSpacing;

        static std::array<float, 9> userKernel{ 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f };
        static std::array<float, 9> selectedKernel{ userKernel };

        const auto totalWidth = ImGui::GetContentRegionAvail().x - 2.0f * itemInnerSpacing.x;

        {
            UI::ScopedWidth textWidth{ totalWidth };
            UI::ScopedFont mediumFont{ UI::Font::Medium };
            UI::DrawCursor::Advance({ itemInnerSpacing.x, 0.0f });
            ImGui::Text("Kernel Matrix");
        }

        static ImGuiInputTextFlags_ kernelMatrixInputFlags{ ImGuiInputTextFlags_None };
        static int selectedKernelIndex{ 0 };
        const char* kernelNames[4] = { "Custom", "Blur", "Edge-Detection", "Sharpen" };

        {
            UI::ScopedWidth sliderWidth{ totalWidth };
            UI::DrawCursor::Advance({ itemInnerSpacing.x, 0.0f });
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

        {
            if (selectedKernelIndex == 0) {

                selectedKernel = userKernel;
            }

            constexpr const char* format = "%.4f";
            UI::ScopedWidth inputWidth{ totalWidth };
            {
                UI::ScopedID kernelMatrixRowId{ "idKernelMatrixRow0" };
                UI::DrawCursor::Advance({ itemInnerSpacing.x, 0.0f });
                ImGui::InputFloat3("", selectedKernel.data(), format, kernelMatrixInputFlags);
            }
            {
                UI::ScopedID kernelMatrixRowId{ "idKernelMatrixRow1" };
                UI::DrawCursor::Advance({ itemInnerSpacing.x, 0.0f });
                ImGui::InputFloat3("", selectedKernel.data() + 3, format, kernelMatrixInputFlags);
            }
            {
                UI::ScopedID kernelMatrixRowId{ "idKernelMatrixRow2" };
                UI::DrawCursor::Advance({ itemInnerSpacing.x, 0.0f });
                ImGui::InputFloat3("", selectedKernel.data() + 6, format, kernelMatrixInputFlags);
            }

            if (selectedKernelIndex == 0) {

                userKernel = selectedKernel;
            }
        }

        UI::DrawCursor::Advance({ itemInnerSpacing.x, 0.0f });
        if (ImGui::Button("Apply", { totalWidth, 0.0f })) {

            const auto width = renderFrameBuffer->GetWidth();
            const auto height = renderFrameBuffer->GetHeight();
            auto kernelFrameBuffer = std::make_shared<Core::OpenGL::FrameBuffer>(width, height);
            if (Core::ImageProcessing::Kernel(kernelFrameBuffer, renderFrameBuffer, selectedKernel)) {

                Filter kernel{};
                kernel.Name = fmt::format("Kernel - {}", kernelNames[selectedKernelIndex]);
                kernel.Target = kernelFrameBuffer;

                filterList.emplace_back(kernel);
            }
        }
    }

    void ImageProcessingView::drawFilterStack() noexcept {

        const auto& style = ImGui::GetStyle();
        const auto itemSpacing = style.ItemSpacing;
        const auto itemInnerSpacing = style.ItemInnerSpacing;

        const auto totalWidth = ImGui::GetContentRegionAvail().x - 2.0f * itemInnerSpacing.x;

        {
            UI::ScopedWidth textWidth{ totalWidth };
            UI::ScopedFont mediumFont{ UI::Font::Medium };
            UI::DrawCursor::Advance({ itemInnerSpacing.x, 0.0f });
            if (ImGui::Button("Stack All", { totalWidth, 0.0f })) {

                const auto width = renderFrameBuffer->GetWidth();
                const auto height = renderFrameBuffer->GetHeight();
                auto stackFrameBuffer = std::make_shared<Core::OpenGL::FrameBuffer>(width, height);
                if (!Core::ImageProcessing::Stack(stackFrameBuffer, textureList)) {

                    STARTRACKER_WARN("Couldn't stack images");
                }

                Filter stack{};
                stack.Name = "Stack";
                stack.Target = stackFrameBuffer;

                filterList.emplace_back(stack);
            }
        }
    }

    void ImageProcessingView::showTexture(std::uint32_t textureNativeHandle, const glm::vec2& size) noexcept {

        renderFrameBuffer->Resize(static_cast<std::int32_t>(size.x), static_cast<std::int32_t>(size.y));

        // Bind the Texture
        copyShader->SetInt("uTexture", 0);
        glBindTextureUnit(0, textureNativeHandle);

        renderFrameBuffer->Bind();
        Core::OpenGL::Renderer::Clear();
        Core::OpenGL::Renderer::DrawIndexed(renderVertexArray, copyShader, Core::OpenGL::PrimitiveMode::Triangle);
        renderFrameBuffer->Unbind();
    }
}