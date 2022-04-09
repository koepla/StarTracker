#include "Shader.hpp"

namespace StarTracker::Core::OpenGL {

	Shader::Shader() noexcept : nativeHandle{}, uniformLocationCache{} {

	}

	Shader::~Shader() noexcept {

		glDeleteShader(nativeHandle);
	}

	void Shader::LoadFromFile(const std::filesystem::path& vertexShaderPath, const std::filesystem::path& fragmentShaderPath) noexcept {

		const auto vertexShaderSource = Utils::File::ReadFile(vertexShaderPath);
		const auto fragmentShaderSource = Utils::File::ReadFile(fragmentShaderPath);

		const auto vertexShaderProgram = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
		const auto fragmentShaderProgram = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

		const auto shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShaderProgram);
		glAttachShader(shaderProgram, fragmentShaderProgram);
		glLinkProgram(shaderProgram);

		std::int32_t linkSuccess{};
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkSuccess);

		if (!linkSuccess) {

			std::int32_t infoLength{};
			std::vector<char> failureInfo{};
			glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &infoLength);
			failureInfo.resize(infoLength);
			glGetProgramInfoLog(shaderProgram, infoLength, nullptr, failureInfo.data());
			glDeleteProgram(shaderProgram);
			glDeleteProgram(vertexShaderProgram);
			glDeleteProgram(fragmentShaderProgram);
		
			STARTRACKER_ERROR("Shader linking failed: {}", failureInfo.data());
			ASSERT(false && "Shader linking failed, see `failureInfo`!");
		}

		std::int32_t uniformCount{};
		glGetProgramiv(shaderProgram, GL_ACTIVE_UNIFORMS, &uniformCount);
	
		std::int32_t maxUniformLength{};
		glGetProgramiv(shaderProgram, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformLength);

		if (uniformCount > 0 && maxUniformLength > 0) {

			std::vector<char> uniformNameBuffer{};
			uniformNameBuffer.resize(maxUniformLength);

			for (std::int32_t i = 0; i < uniformCount; i++) {

				std::int32_t length{};
				std::int32_t size{};
				std::uint32_t dataType{};

				glGetActiveUniform(shaderProgram, i, maxUniformLength, &length, &size, &dataType, uniformNameBuffer.data());
				const auto location = glGetUniformLocation(shaderProgram, uniformNameBuffer.data());
				uniformLocationCache[uniformNameBuffer.data()] = location;

				STARTRACKER_INFO("[{}, {}] Uniform {} has location {}", vertexShaderPath.filename().string(), fragmentShaderPath.filename().string(), uniformNameBuffer.data(), location);
			}
		}

		nativeHandle = shaderProgram;
	}

	void Shader::SetInt(std::string_view name, int value) noexcept {

		glUseProgram(nativeHandle);
		glUniform1i(getUniformLocation(name), value);
	}

	void Shader::SetInt2(std::string_view name, const glm::ivec2& value) noexcept {

		glUseProgram(nativeHandle);
		glUniform2i(getUniformLocation(name), value.x, value.y);
	}

	void Shader::SetInt3(std::string_view name, const glm::ivec3& value) noexcept {

		glUseProgram(nativeHandle);
		glUniform3i(getUniformLocation(name), value.x, value.y, value.z);
	}

	void Shader::SetInt4(std::string_view name, const glm::ivec4& value) noexcept {

		glUseProgram(nativeHandle);
		glUniform4i(getUniformLocation(name), value.x, value.y, value.z, value.w);
	}

	void Shader::SetFloat(std::string_view name, float value) noexcept {

		glUseProgram(nativeHandle);
		glUniform1f(getUniformLocation(name), value);
	}

	void Shader::SetFloat2(std::string_view name, const glm::vec2& value) noexcept {

		glUseProgram(nativeHandle);
		glUniform2f(getUniformLocation(name), value.x, value.y);
	}

	void Shader::SetFloat3(std::string_view name, const glm::vec3& value) noexcept {

		glUseProgram(nativeHandle);
		glUniform3f(getUniformLocation(name), value.x, value.y, value.z);
	}

	void Shader::SetFloat4(std::string_view name, const glm::vec4& value) noexcept {

		glUseProgram(nativeHandle);
		glUniform4f(getUniformLocation(name), value.x, value.y, value.z, value.w);
	}

	void Shader::SetMat3(std::string_view name, const glm::mat3 &value) noexcept {

		glUseProgram(nativeHandle);
		glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
	}

	void Shader::SetMat4(std::string_view name, const glm::mat4& value) noexcept {

		glUseProgram(nativeHandle);
		glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
	}

	void Shader::Bind() const noexcept {

		glUseProgram(nativeHandle);
	}

	void Shader::Unbind() const noexcept {

		glUseProgram(0);
	}

	std::int32_t Shader::getUniformLocation(std::string_view name) noexcept {

		if (uniformLocationCache.find(name) != uniformLocationCache.end()) {

			return uniformLocationCache[name];
		}

		const auto location = glGetUniformLocation(nativeHandle, name.data());

		if (location == -1) {

			STARTRACKER_ERROR("Uniform {} doesn't exist", name);
			ASSERT(false && "Uniform `name` doesn't exist");
		}
		else {

			uniformLocationCache[name] = location;
		}

		return location;
	}

	std::uint32_t Shader::compileShader(std::string_view sourceCode, std::uint32_t type) noexcept {

		const auto shaderProgram = glCreateShader(type);
		const GLchar* shaderSourceCode = sourceCode.data();

		glShaderSource(shaderProgram, 1, &shaderSourceCode, nullptr);
		glCompileShader(shaderProgram);

		std::int32_t compileSuccess{};
		glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &compileSuccess);

		if (!compileSuccess) {

			std::int32_t infoLength{};
			std::vector<char> failureInfo{};
			glGetShaderiv(shaderProgram, GL_INFO_LOG_LENGTH, &infoLength);
			failureInfo.resize(infoLength);
			glGetShaderInfoLog(shaderProgram, infoLength, &infoLength, failureInfo.data());

			glDeleteProgram(shaderProgram);

			STARTRACKER_ERROR("Shader compilation failed: {}", failureInfo.data());
			ASSERT(false && "Shader compilation failed, see `failureInfo`!");
		}

		return shaderProgram;
	}
}