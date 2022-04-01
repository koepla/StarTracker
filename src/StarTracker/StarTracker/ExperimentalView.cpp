#include "ExperimentalView.hpp"

namespace StarTracker {

	ExperimentalView::ExperimentalView(void* nativeWindowHandle) noexcept : Core::View{ nativeWindowHandle }, camera{}, shader{}, frameBuffer{}, model{} {

	}

	void ExperimentalView::OnInit() noexcept {

        const auto application = Core::Application::GetInstance();
        const auto windowWidth = application->GetWindow().GetWidth();
        const auto windowHeight = application->GetWindow().GetHeight();

        constexpr auto initialCameraPosition = glm::vec3{ 0.0f, 0.0f, 10.0f };
        camera = std::make_shared<Core::OpenGL::Camera>(initialCameraPosition);
        frameBuffer = std::make_shared<Core::OpenGL::FrameBuffer>(windowWidth, windowHeight);
        shader = Core::AssetDataBase::LoadShader("modelVertex.glsl", "modelFragment.glsl");
        model = Core::AssetDataBase::LoadModel("starTracker.obj", { 1.0f, 1.0f, 1.0f });

        application->RegisterEventHandler([this](const Core::Events::Event& event) -> void {

            const auto windowResize = dynamic_cast<const Core::Events::WindowResizeEvent*>(&event);

            if(windowResize) {

                frameBuffer->Resize(windowResize->GetWidth(), windowResize->GetHeight());
            }
        });

        Core::OpenGL::Renderer::Initialize();
    }

	void ExperimentalView::OnUpdate(float deltaTime) noexcept {

        shader->SetMat4("uTransform", camera->GetProjectionMatrix() * camera->GetViewMatrix(deltaTime));

        frameBuffer->Bind();
        Core::OpenGL::Renderer::SetClearColor({ 0.15f, 0.15f, 0.15f, 1.0f });
        Core::OpenGL::Renderer::Clear();
        Core::OpenGL::Renderer::DrawModel(model, shader);
        frameBuffer->Unbind();

        if (ImGui::Begin("3D-Construction")) {

            ImGui::Text("%s", std::format("FrameRate: {}", 1.0f / deltaTime).c_str());
            const auto viewPortSize = ImGui::GetContentRegionAvail();
            ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(frameBuffer->GetNativeTextureHandle())), viewPortSize, { 0, 1 }, { 1, 0 });
		}
		ImGui::End();
    }

	void ExperimentalView::OnDestroy() noexcept {

	}
}