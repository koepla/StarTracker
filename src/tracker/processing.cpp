#include "processing.hpp"

#include <libtracker/libtracker.hpp>

namespace {

    void SaveImage(const std::shared_ptr<graphics::FrameBuffer>& image) noexcept {
        auto path = arch::SaveFileDialog("Save Image");
        if (!path.empty()) {
            if (!image->WriteToFile(path)) {
                LIBTRACKER_ERROR("Could not save image, operation failed");
            }
        } else {
            LIBTRACKER_WARN("Could not save image because the specified path was empty");
        }
    }
}// namespace

Processing::Processing(void* windowHandle) noexcept : View{ windowHandle } { }

void Processing::OnInit() noexcept {
    // SetOnlineLocation ImageProcessing FrameBuffer
    renderFrameBuffer = std::make_shared<graphics::FrameBuffer>(160, 90);
    copyShader = AssetDatabase::LoadShader("copyTextureVertex.glsl", "copyTextureFragment.glsl");

    // SetOnlineLocation OpenGL Buffers for showing a specific texture
    initializeBuffers();
}

void Processing::OnUpdate(float deltaTime) noexcept {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            {
                ScopedFont mediumFont{ Font::Medium };
                ImGui::Text("Processing");
            }
            if (ImGui::MenuItem("Save", "Ctrl+S")) {
                SaveImage(renderFrameBuffer);
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    if (Input::IsKeyPressed(KeyCode::LeftControl) && Input::IsKeyPressed(KeyCode::S)) {
        SaveImage(renderFrameBuffer);
    }

    if (ImGui::Begin("Image Processing", nullptr, ImGuiWindowFlags_NoScrollbar)) {
        constexpr auto flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable;
        if (ImGui::BeginTable("##idChildImageProcessingAlignmentTable", 3, flags)) {
            const auto totalWidth = ImGui::GetContentRegionAvail().x;
            ImGui::TableSetupColumn("##idColumnImageListPanel", ImGuiTableColumnFlags_WidthStretch, 0.2f * totalWidth);
            ImGui::TableSetupColumn("##idColumnFramebufferPanel", ImGuiTableColumnFlags_WidthStretch,
                                    0.6f * totalWidth);
            ImGui::TableSetupColumn("##idColumnFilterPanel", ImGuiTableColumnFlags_WidthStretch, 0.2f * totalWidth);

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            drawTextureListPanel();
            ImGui::TableSetColumnIndex(1);
            drawRenderFrameBufferPanel();
            ImGui::TableSetColumnIndex(2);
            drawFilterPanel();
            ImGui::EndTable();
        }
    }
    ImGui::End();
}

void Processing::OnDestroy() noexcept { }

void Processing::initializeBuffers() noexcept {
    renderVertexArray = std::make_shared<graphics::VertexArray>();
    renderVertexBuffer = std::make_shared<graphics::VertexBuffer>();
    renderIndexBuffer = std::make_shared<graphics::IndexBuffer>();

    // Fixed texture vertices
    const static std::array vertices = {
        graphics::TextureVertex{ glm::vec3{ -1.0f, -1.0f, 0.0f }, glm::vec3{ 1.0f, 1.0f, 1.0f },
                                 glm::vec2{ 0.0f, 0.0f } },
        graphics::TextureVertex{ glm::vec3{ 1.0f, -1.0f, 0.0f }, glm::vec3{ 1.0f, 1.0f, 1.0f },
                                 glm::vec2{ 1.0f, 0.0f } },
        graphics::TextureVertex{ glm::vec3{ 1.0f, 1.0f, 0.0f }, glm::vec3{ 1.0f, 1.0f, 1.0f },
                                 glm::vec2{ 1.0f, 1.0f } },
        graphics::TextureVertex{ glm::vec3{ -1.0f, 1.0f, 0.0f }, glm::vec3{ 1.0f, 1.0f, 1.0f },
                                 glm::vec2{ 0.0f, 1.0f } }
    };

    // Fixed indices for drawing the rectangle
    const static std::array<u32, 6> indices = { 0, 1, 2, 2, 0, 3 };

    // Buffer elements that match the Shader
    const static std::vector vertexBufferElements = {
        graphics::BufferElement{ graphics::ShaderDataType::Float3, "aPosition" },
        graphics::BufferElement{ graphics::ShaderDataType::Float3, "aColor" },
        graphics::BufferElement{ graphics::ShaderDataType::Float2, "aTextureCoordinates" }
    };

    // BufferLayout containing the elements
    const static graphics::BufferLayout vertexBufferLayout{ vertexBufferElements };

    // Set up the VertexArray
    renderVertexArray->Bind();
    renderVertexBuffer->SetLayout(vertexBufferLayout);
    renderVertexBuffer->SetData(vertices.data(), static_cast<u32>(vertices.size() * sizeof(graphics::TextureVertex)));
    renderIndexBuffer->SetData(indices.data(), static_cast<u32>(indices.size()));
    renderVertexArray->SetIndexBuffer(renderIndexBuffer);
    renderVertexArray->SetVertexBuffer(renderVertexBuffer);
}

void Processing::drawTextureListPanel() noexcept {
    const auto& style = ImGui::GetStyle();
    const auto fontSize = ImGui::GetFontSize();
    const auto itemSpacing = style.ItemSpacing;
    const auto itemInnerSpacing = style.ItemInnerSpacing;
    const auto& baseTextColor = style.Colors[ImGuiCol_Text];
    const auto& baseTextLightColor =
            ImVec4{ baseTextColor.x, baseTextColor.y, baseTextColor.z, 0.5f * baseTextColor.w };
    const auto infoCardHeight = 3.0f * fontSize + (2.0f + 2.0f * 0.7f) * itemSpacing.y - 4.0f;
    const auto& baseChildBackground = style.Colors[ImGuiCol_FrameBg];
    const auto& darkerChildBackground =
            ImVec4{ baseChildBackground.x, baseChildBackground.y, baseChildBackground.z, 0.5f * baseChildBackground.w };

    ScopedColor childBackground{ ImGuiCol_ChildBg, darkerChildBackground };

    // We need this because we want to dynamically remove textures from the textureList
    // due to the fact that the UI function for drawing textures
    // expects the native texture handle, and we cannot keep textures alive
    // by simply passing the texture handle.
    // Therefore we have to add textures that are meant to be removed to this list,
    // as their presence keeps them alive after being erased from textureList,
    // so the texture is not destroyed and ImGui can draw it in its draw call
    // at the end of the frame
    static std::vector<std::shared_ptr<graphics::Texture>> keepAliveTextureList{};
    const auto textureListPanelSize = ImGui::GetContentRegionAvail();
    keepAliveTextureList.clear();
    if (ImGui::BeginChild("idChildTextureListPanel", textureListPanelSize, false, ImGuiWindowFlags_NoScrollbar)) {
        const auto cursor = ImGui::GetCursorPos();
        ScopedColor innerChildBackground{ ImGuiCol_ChildBg, style.Colors[ImGuiCol_FrameBg] };
        if (ImGui::BeginChild("idChildTextureListPanelHeader", { ImGui::GetContentRegionAvail().x, infoCardHeight },
                              false, ImGuiWindowFlags_HorizontalScrollbar)) {
            DrawCursor::Advance(itemInnerSpacing.x, 0.0f);
            Text::Draw("Images", Font::Medium, fontSize, baseTextColor);
            DrawCursor::Advance(0.0f, fontSize + itemSpacing.y);
            const auto totalSize = ImGui::GetContentRegionAvail();
            const auto buttonWidth = 0.5f * (totalSize.x - itemInnerSpacing.x - itemSpacing.x);
            const auto buttonHeight = totalSize.y - itemInnerSpacing.y;
            if (ImGui::Button("Import", { buttonWidth, buttonHeight })) {
                const auto paths = arch::OpenFileDialog("Select Images", true);
                if (!paths.empty()) {
                    for (const auto& path : paths) {
                        auto texture = std::make_shared<graphics::Texture>();
                        if (texture->LoadFromFile(path)) {
                            textureList.emplace_back(texture);
                        } else {
                            LIBTRACKER_WARN("Failed to load texture {}", path.filename().string());
                        }
                    }
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Clear", { buttonWidth, buttonHeight })) {
                textureList.clear();
            }
        }
        ImGui::EndChild();

        const auto textureListSize = ImGui::GetContentRegionAvail();
        ScopedColor transparentBackground{ ImGuiCol_ChildBg, { 0.0f, 0.0f, 0.0f, 0.0f } };
        if (ImGui::BeginChild("idChildTextureList",
                              { textureListSize.x, textureListSize.y - fontSize - itemInnerSpacing.y }, false,
                              ImGuiWindowFlags_HorizontalScrollbar)) {
            DrawCursor::Advance(itemInnerSpacing.x, 0.0f);
            for (std::size_t i = 0; i < textureList.size(); i++) {
                const auto& currentTexture = textureList.at(i);
                const auto textureHeight = 4.0f * fontSize;
                const auto textureWidth = static_cast<float>(currentTexture->GetWidth()) /
                                          static_cast<float>(currentTexture->GetHeight()) * textureHeight;
                if (textureWidth > ImGui::GetContentRegionAvail().x) {
                    ImGui::NewLine();
                    DrawCursor::Advance(itemInnerSpacing.x, 0.0f);
                }

                const auto textureChildContainerId = fmt::format("idChildTexture{}", currentTexture->GetNativeHandle());
                if (ImGui::BeginChild(textureChildContainerId.c_str(), { textureWidth, textureHeight }, false,
                                      ImGuiWindowFlags_NoScrollbar)) {
                    Image::DrawRounded(currentTexture->GetNativeHandle(), textureWidth, textureHeight);
                }
                ImGui::EndChild();

                const auto textureRemovePopupId =
                        fmt::format("idRemoveTexturePopup{}", currentTexture->GetNativeHandle());

                if (ImGui::IsItemHovered()) {
                    if (Input::IsMousePressed(MouseCode::ButtonRight)) {
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
                        const auto showTextureWidth = currentTexture->GetWidth();
                        const auto showTextureHeight = currentTexture->GetHeight();
                        showTexture(textureHandle, showTextureWidth, showTextureHeight);
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

        ImGui::SetCursorPos(cursor);
        DrawCursor::Advance(itemInnerSpacing.x, textureListPanelSize.y - itemInnerSpacing.y - fontSize);
        Text::Draw(fmt::format("Loaded images: {}", textureList.size()), Font::Regular, fontSize, baseTextLightColor);
    }
    ImGui::EndChild();
}

void Processing::drawRenderFrameBufferPanel() const noexcept {
    const auto& style = ImGui::GetStyle();
    const auto fontSize = ImGui::GetFontSize();
    const auto itemInnerSpacing = style.ItemInnerSpacing;
    const auto& baseTextColor = style.Colors[ImGuiCol_Text];

    if (ImGui::BeginChild("idChildFrameBufferPanel", ImGui::GetContentRegionAvail(), false,
                          ImGuiWindowFlags_NoScrollbar)) {
        if (ImGui::BeginChild("idChildFrameBufferPanelHeader", { ImGui::GetContentRegionAvail().x, fontSize }, false,
                              ImGuiWindowFlags_NoScrollbar)) {
            DrawCursor::Advance(itemInnerSpacing.x, 0.0f);
            Text::Draw("Result", Font::Medium, fontSize, baseTextColor);
        }
        ImGui::EndChild();

        const auto textureHandle = renderFrameBuffer->GetNativeTextureHandle();
        const auto bufferWidth = renderFrameBuffer->GetWidth();
        const auto bufferHeight = renderFrameBuffer->GetHeight();
        float textureWidth;
        float textureHeight;
        if (bufferWidth > bufferHeight) {
            textureWidth = ImGui::GetContentRegionAvail().x;
            textureHeight = static_cast<float>(bufferHeight) / static_cast<float>(bufferWidth) * textureWidth;
        } else {
            textureHeight = ImGui::GetContentRegionAvail().y;
            textureWidth = static_cast<float>(bufferWidth) / static_cast<float>(bufferHeight) * textureHeight;
        }
        const auto dx = ImGui::GetContentRegionAvail().x - textureWidth;
        const auto dy = ImGui::GetContentRegionAvail().y - textureHeight;
        DrawCursor::Advance(dx / 2.0f, dy / 2.0f);
        Image::DrawRounded(textureHandle, textureWidth, textureHeight);
    }
    ImGui::EndChild();
}

void Processing::drawFilterPanel() noexcept {
    if (ImGui::BeginChild("idChildFilterPanel", ImGui::GetContentRegionAvail(), false, ImGuiWindowFlags_NoScrollbar)) {
        drawFilterHierarchyPanel();
        if (ImGui::CollapsingHeader("Kernel", ImGuiTreeNodeFlags_DefaultOpen)) {
            drawFilterKernel();
        }
        if (ImGui::CollapsingHeader("Stack", ImGuiTreeNodeFlags_DefaultOpen)) {
            drawFilterStack();
        }
        if (ImGui::CollapsingHeader("Export", ImGuiTreeNodeFlags_DefaultOpen)) {
            const auto innerSpacing = ImGui::GetStyle().ItemInnerSpacing.x;
            const auto buttonWidth = ImGui::GetContentRegionAvail().x - 2.0f * innerSpacing;

            DrawCursor::Advance(innerSpacing, 0.0f);
            if (ImGui::Button("Save as Portable Network Graphics", { buttonWidth, 0.0f })) {
                SaveImage(renderFrameBuffer);
            }
        }
    }
    ImGui::EndChild();
}
void Processing::drawFilterHierarchyPanel() noexcept {
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
    // Therefore, we have to add textures that are meant to be removed to this list,
    // as their presence keeps them alive after being erased from textureList,
    // so the texture is not destroyed and ImGui can draw it in its draw call
    static std::vector<std::shared_ptr<graphics::FrameBuffer>> keepAliveTargetList{};
    keepAliveTargetList.clear();

    ScopedColor childBackground{ ImGuiCol_ChildBg, style.Colors[ImGuiCol_FrameBg] };
    if (ImGui::BeginChild("idChildFilterHierarchyPanelHeader", { size.x, 0.45f * size.y }, false,
                          ImGuiWindowFlags_NoScrollbar)) {
        DrawCursor::Advance(itemInnerSpacing.x, 0.0f);
        Text::Draw("Active Filters", Font::Medium, fontSize, baseTextColor);
        DrawCursor::Advance(-itemInnerSpacing.x, fontSize + itemSpacing.y);

        if (ImGui::BeginChild("idChildFilterHierarchyPanel", ImGui::GetContentRegionAvail(), false,
                              ImGuiWindowFlags_HorizontalScrollbar)) {
            for (std::size_t i = 0; i < filterList.size(); i++) {
                const auto& filter = filterList.at(i);
                const auto treeNodeId = fmt::format("idTreeNodeFilterHierarchyPanel{}", i);
                if (ImGui::TreeNode(treeNodeId.c_str(), "%s", filter.Name.c_str())) {
                    const auto childId = fmt::format("idChildFilterHierarchyPanelFilterElem{}", i);
                    const auto targetWidth = static_cast<f32>(filter.Target->GetWidth());
                    const auto targetHeight = static_cast<f32>(filter.Target->GetHeight());
                    const auto drawHeight = 4.0f * fontSize;
                    const auto drawWidth = drawHeight * targetWidth / targetHeight;
                    if (ImGui::BeginChild(childId.c_str(), { drawWidth, drawHeight }, false)) {
                        const auto textureHandle = filter.Target->GetNativeTextureHandle();
                        Image::DrawRounded(textureHandle, drawWidth, drawHeight);
                    }
                    ImGui::EndChild();
                    ImGui::TreePop();
                }

                const auto treeNodePopupId = fmt::format("idTreeNodePopupFilterHierarchyPanelFilterElem{}", i);
                if (ImGui::IsItemHovered()) {
                    if (Input::IsMousePressed(MouseCode::ButtonRight)) {
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
                        showTexture(textureHandle, textureWidth, textureHeight);
                    }
                    if (ImGui::MenuItem("Save")) {
                        SaveImage(filter.Target);
                    }
                    ImGui::EndPopup();
                }
            }
        }
        ImGui::EndChild();
    }
    ImGui::EndChild();
}

void Processing::drawFilterKernel() noexcept {
    const auto& style = ImGui::GetStyle();
    const auto itemInnerSpacing = style.ItemInnerSpacing;

    static std::array userKernel{ 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f };
    static std::array selectedKernel{ userKernel };

    const auto totalWidth = ImGui::GetContentRegionAvail().x - 2.0f * itemInnerSpacing.x;
    {
        ScopedWidth textWidth{ totalWidth };
        ScopedFont mediumFont{ Font::Medium };
        DrawCursor::Advance(itemInnerSpacing.x, 0.0f);
        ImGui::Text("Kernel Matrix");
    }

    static ImGuiInputTextFlags_ kernelMatrixInputFlags{ ImGuiInputTextFlags_None };
    static int selectedKernelIndex{ 0 };
    const char* kernelNames[4] = { "Custom", "Blur", "Edge-Detection", "Sharpen" };

    {
        ScopedWidth sliderWidth{ totalWidth };
        DrawCursor::Advance(itemInnerSpacing.x, 0.0f);
        ImGui::SliderInt("##idKernelEnumSlider", &selectedKernelIndex, 0, 3, [&]() -> const char* {
            if (selectedKernelIndex == 0) {
                kernelMatrixInputFlags = ImGuiInputTextFlags_None;
                selectedKernel = userKernel;
                return kernelNames[0];
            }
            if (selectedKernelIndex == 1) {
                kernelMatrixInputFlags = ImGuiInputTextFlags_ReadOnly;
                selectedKernel = ImageProcessing::KernelBlur;
                return kernelNames[1];
            }
            if (selectedKernelIndex == 2) {
                kernelMatrixInputFlags = ImGuiInputTextFlags_ReadOnly;
                selectedKernel = ImageProcessing::KernelEdgeDetection;
                return kernelNames[2];
            }
            if (selectedKernelIndex == 3) {
                kernelMatrixInputFlags = ImGuiInputTextFlags_ReadOnly;
                selectedKernel = ImageProcessing::KernelSharpen;
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
        ScopedWidth inputWidth{ totalWidth };
        {
            ScopedID kernelMatrixRowId{ "idKernelMatrixRow0" };
            DrawCursor::Advance(itemInnerSpacing.x, 0.0f);
            ImGui::InputFloat3("", selectedKernel.data(), format, kernelMatrixInputFlags);
        }
        {
            ScopedID kernelMatrixRowId{ "idKernelMatrixRow1" };
            DrawCursor::Advance(itemInnerSpacing.x, 0.0f);
            ImGui::InputFloat3("", selectedKernel.data() + 3, format, kernelMatrixInputFlags);
        }
        {
            ScopedID kernelMatrixRowId{ "idKernelMatrixRow2" };
            DrawCursor::Advance(itemInnerSpacing.x, 0.0f);
            ImGui::InputFloat3("", selectedKernel.data() + 6, format, kernelMatrixInputFlags);
        }

        if (selectedKernelIndex == 0) {
            userKernel = selectedKernel;
        }
    }

    DrawCursor::Advance(itemInnerSpacing.x, 0.0f);
    if (ImGui::Button("Apply", { totalWidth, 0.0f })) {
        const auto width = renderFrameBuffer->GetWidth();
        const auto height = renderFrameBuffer->GetHeight();
        const auto kernelFrameBuffer = std::make_shared<graphics::FrameBuffer>(width, height);
        if (ImageProcessing::Kernel(kernelFrameBuffer, renderFrameBuffer, selectedKernel)) {
            Filter kernel{};
            kernel.Name = fmt::format("Kernel - {}", kernelNames[selectedKernelIndex]);
            kernel.Target = kernelFrameBuffer;
            filterList.emplace_back(kernel);
        }
    }
}

void Processing::drawFilterStack() noexcept {
    const auto& style = ImGui::GetStyle();
    const auto itemInnerSpacing = style.ItemInnerSpacing;
    const auto totalWidth = ImGui::GetContentRegionAvail().x - 2.0f * itemInnerSpacing.x;
    ScopedWidth textWidth{ totalWidth };
    DrawCursor::Advance(itemInnerSpacing.x, 0.0f);
    if (ImGui::Button("Stack All", { totalWidth, 0.0f })) {
        const auto width = renderFrameBuffer->GetWidth();
        const auto height = renderFrameBuffer->GetHeight();
        const auto stackFrameBuffer = std::make_shared<graphics::FrameBuffer>(width, height);
        if (!ImageProcessing::Stack(stackFrameBuffer, textureList)) {
            LIBTRACKER_WARN("Couldn't stack images");
        }

        Filter stack{};
        stack.Name = "Stack";
        stack.Target = stackFrameBuffer;
        filterList.emplace_back(stack);
    }
}

void Processing::showTexture(u32 textureNativeHandle, s32 width, s32 height) const noexcept {
    renderFrameBuffer->Resize(width, height);

    copyShader->SetInt("uTexture", 0);
    glBindTextureUnit(0, textureNativeHandle);

    renderFrameBuffer->Bind();
    graphics::Renderer::Clear();
    graphics::Renderer::DrawIndexed(renderVertexArray, copyShader, graphics::PrimitiveMode::Triangle);
    renderFrameBuffer->Unbind();
}
