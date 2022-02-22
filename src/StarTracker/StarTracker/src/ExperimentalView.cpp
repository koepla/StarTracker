#include "ExperimentalView.hpp"

#include <glm/glm.hpp>

namespace StarTracker {

	ExperimentalView::ExperimentalView(void* nativeWindowHandle) noexcept : Core::View{ nativeWindowHandle } {

	}

	void ExperimentalView::OnInit() noexcept {

		vertexArray = std::make_shared<Core::OpenGL::VertexArray>();
		vertexBuffer = std::make_shared<Core::OpenGL::VertexBuffer>();
		indexBuffer = std::make_shared<Core::OpenGL::IndexBuffer>();
		shader = std::make_shared<Core::OpenGL::Shader>();

		static std::array<glm::vec3, 4> vertices = {

			glm::vec3{ -0.5f, -0.5f, 0.0f },
			glm::vec3{  0.5f, -0.5f, 0.0f },
			glm::vec3{  0.5f,  0.5f, 0.0f },
			glm::vec3{ -0.5f,  0.5f, 0.0f }
		};

		static std::array<std::uint32_t, 6> indices = {

			0, 1, 2, 2, 0, 3
		};

		static std::vector<Core::OpenGL::BufferElement> vertexBufferElements = {

			Core::OpenGL::BufferElement{ Core::OpenGL::ShaderDataType::Float3, "aPosition" }
		};

		Core::OpenGL::BufferLayout vertexBufferLayout{ vertexBufferElements };

		vertexArray->Bind();

		vertexBuffer->SetLayout(vertexBufferLayout);
		vertexBuffer->SetData(vertices.data(), static_cast<std::uint32_t>(vertices.size() * sizeof(glm::vec3)));

		indexBuffer->SetData(indices.data(), static_cast<std::uint32_t>(indices.size()));
	
		vertexArray->SetIndexBuffer(indexBuffer);
		vertexArray->SetVertexBuffer(vertexBuffer);

		shader->LoadFromFile("assets/shaders/defaultVertex.glsl", "assets/shaders/defaultFragment.glsl");
	}

	void ExperimentalView::OnUpdate(float deltaTime) noexcept {

		shader->Bind();
		vertexArray->Bind();
		glDrawElements(GL_TRIANGLES, indexBuffer->GetIndexCount(), GL_UNSIGNED_INT, nullptr);

		// Called every frame
		if (ImGui::Begin("Experimental")) {

			
		}
		ImGui::End();
	}

	void ExperimentalView::OnDestroy() noexcept {

		// Called once on shutdown
	}
}