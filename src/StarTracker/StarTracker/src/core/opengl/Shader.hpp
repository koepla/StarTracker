#ifndef STARTRACKER_CORE_OPENGL_SHADER_H
#define STARTRACKER_CORE_OPENGL_SHADER_H

#include "../Assert.hpp"
#include "../../utils/File.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <unordered_map>

namespace StarTracker::Core::OpenGL {

	class Shader {

	private:
		std::uint32_t nativeHandle;
		std::unordered_map<std::string, std::int32_t> uniformLocationCache;

	public:
		Shader() noexcept;
		~Shader() noexcept;

		void LoadFromFile(const std::filesystem::path& vertexShaderPath, const std::filesystem::path& fragmentShaderPath) noexcept;

		void SetInt(const std::string& name, int value) noexcept;
		void SetInt2(const std::string& name, const glm::ivec2& value) noexcept;
		void SetInt3(const std::string& name, const glm::ivec3& value) noexcept;
		void SetInt4(const std::string& name, const glm::ivec4& value) noexcept;
		void SetFloat(const std::string& name, float value) noexcept;
		void SetFloat2(const std::string& name, const glm::vec2& value) noexcept;
		void SetFloat3(const std::string& name, const glm::vec3& value) noexcept;
		void SetFloat4(const std::string& name, const glm::vec4& value) noexcept;
		void SetMat4(const std::string& name, const glm::mat4& value) noexcept;

		void Bind() const noexcept;
		void Unbind() const noexcept;

	private:
		[[nodiscard]] std::int32_t getUniformLocation(const std::string& name) noexcept;

	private:
		[[nodiscard]] static std::uint32_t compileShader(const std::string& sourceCode, std::uint32_t type) noexcept;
	};
}

#endif // STARTRACKER_CORE_OPENGL_SHADER_H