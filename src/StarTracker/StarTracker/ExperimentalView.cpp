#include "ExperimentalView.hpp"

namespace StarTracker {

	ExperimentalView::ExperimentalView(void* nativeWindowHandle) noexcept : Core::View{ nativeWindowHandle } {

	}

	void ExperimentalView::OnInit() noexcept {

        const auto application = Core::Application::GetInstance();
        const auto windowWidth = application->GetWindow().GetWidth();
        const auto windowHeight = application->GetWindow().GetHeight();

        constexpr auto initialCameraPosition = glm::vec3{ 0.0f, 0.0f, 10.0f };
        camera = std::make_shared<Core::OpenGL::Camera>(initialCameraPosition);
		vertexArray = std::make_shared<Core::OpenGL::VertexArray>();
		vertexBuffer = std::make_shared<Core::OpenGL::VertexBuffer>();
		indexBuffer = std::make_shared<Core::OpenGL::IndexBuffer>();
        frameBuffer = std::make_shared<Core::OpenGL::FrameBuffer>(windowWidth, windowHeight);
        shader = Core::AssetDataBase::LoadShader("defaultVertex.glsl", "defaultFragment.glsl");

        const static std::array<glm::vec3, 4> vertices = {

			glm::vec3{ -0.5f, -0.5f, 0.0f },
			glm::vec3{  0.5f, -0.5f, 0.0f },
			glm::vec3{  0.5f,  0.5f, 0.0f },
			glm::vec3{ -0.5f,  0.5f, 0.0f },
		};

        const static std::array<std::uint32_t, 6> indices = {

			0, 1, 2, 2, 0, 3
		};

        const static std::vector<Core::OpenGL::BufferElement> vertexBufferElements = {

			Core::OpenGL::BufferElement{ Core::OpenGL::ShaderDataType::Float3, "aPosition" },
		};

		const static Core::OpenGL::BufferLayout vertexBufferLayout{ vertexBufferElements };

		vertexArray->Bind();
		vertexBuffer->SetLayout(vertexBufferLayout);
		vertexBuffer->SetData(vertices.data(), static_cast<std::uint32_t>(vertices.size() * sizeof(glm::vec3)));
		indexBuffer->SetData(indices.data(), static_cast<std::uint32_t>(indices.size()));
		vertexArray->SetIndexBuffer(indexBuffer);
		vertexArray->SetVertexBuffer(vertexBuffer);

        shader->SetMat4("uTransform", glm::mat4{ 1.0f });

        application->RegisterEventHandler([this](const Core::Events::Event& event) -> void {

            const auto windowResize = dynamic_cast<const Core::Events::WindowResizeEvent*>(&event);

            if(windowResize) {

                frameBuffer->Resize(windowResize->GetWidth(), windowResize->GetHeight());
            }
        });

        blueTexture = Core::AssetDataBase::LoadTexture("blue.png");
        pillarsTexture = Core::AssetDataBase::LoadTexture("pillarsOfCreation.jpg");
    }

	void ExperimentalView::OnUpdate(float deltaTime) noexcept {

        frameBuffer->Bind();
		shader->Bind();
        vertexArray->Bind();

        shader->SetMat4("uTransform", camera->GetProjectionMatrix() * camera->GetViewMatrix(deltaTime));

        glClear(GL_COLOR_BUFFER_BIT);
		glDrawElements(GL_TRIANGLES, static_cast<int>(indexBuffer->GetIndexCount()), GL_UNSIGNED_INT, nullptr);
        frameBuffer->Unbind();

        if (ImGui::Begin("Experimental")) {

            ImGui::Text("%s", std::format("FrameRate: {}", 1.0f / deltaTime).c_str());
            const auto viewPortSize = ImGui::GetContentRegionAvail();
            ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(frameBuffer->GetNativeTextureHandle())), viewPortSize, { 0, 1 }, { 1, 0 });
		}
		ImGui::End();

        if (ImGui::Begin("Stack")) {

            stackFrameBuffer = Core::ImageProcessing::Stack({ blueTexture, pillarsTexture });

            const auto viewPortSize = ImGui::GetContentRegionAvail();
            ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(stackFrameBuffer->GetNativeTextureHandle())), viewPortSize, { 0, 1 }, { 1, 0 });
        }
        ImGui::End();
    }

	void ExperimentalView::OnDestroy() noexcept {

	}
}