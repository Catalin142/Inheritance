#pragma once

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

struct ShaderSource
{
	friend class Shader;
private:
	std::string vertexShader;
	std::string fragmentShader;
	ShaderSource(const std::string& a, const std::string& b) : vertexShader(a), fragmentShader(b) {}
};

class Shader
{
private:
	std::unordered_map<std::string, int> m_LocationCache;
	unsigned int m_Handle;

public:
	Shader() = default;
	Shader(const std::string& vertex, const std::string& fragment);
	Shader(const std::string& shaderPath);

	~Shader();

	void Bind();
	void Unbind();

	const void setUniform1i(const std::string& name, int value);
	const void setUniformMat4(const std::string& name, const glm::mat4& mat);
	const void setUniform1f(const std::string& name, float value);
	const void setUniformVec2f(const std::string& name, const glm::vec2& vec2f);
	const void setUniformVec3f(const std::string& name, const glm::vec3& vec3f);
	const void setUniformVec4f(const std::string& name, const glm::vec4& vec4f);
	const void setUniformIntArray(const std::string& name, int* values, unsigned int size);

private:
	unsigned int compileShader(unsigned int type, const std::string& shader);
	unsigned int createProgram(const std::string& vertex, const std::string& fragment);
	std::string readShader(const std::string& path);
	ShaderSource readShaderFile(const std::string& path);
	int SearchUnfiformLocation(const std::string& name);
};

