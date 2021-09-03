#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <fstream>

#include "Shader.h"

Shader::Shader(const std::string& vertex, const std::string& fragment)
	:m_Handle(0)
{
	std::string vertexSource = readShader(vertex);
	std::string fragmentSource = readShader(fragment);
	m_Handle = createProgram(vertexSource, fragmentSource);
}

Shader::Shader(const std::string& shaderPath)
	:m_Handle(0)
{
	ShaderSource shaderSource = readShaderFile(shaderPath);
	m_Handle = createProgram(shaderSource.vertexShader, shaderSource.fragmentShader);
}

Shader::~Shader()
{
	glDeleteProgram(m_Handle);
}

void Shader::Bind()
{
	glUseProgram(m_Handle);
}

void Shader::Unbind()
{
	glUseProgram(0);
}

unsigned int Shader::createProgram(const std::string& vertex, const std::string& fragment)
{
	unsigned int program = glCreateProgram();
	unsigned int  vertexShader = compileShader(GL_VERTEX_SHADER, vertex);
	unsigned int  fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragment);

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;
}

unsigned int  Shader::compileShader(unsigned int  type, const std::string& source)
{
	if (source == "ERR")
		return -1;

	unsigned int  id = glCreateShader(type);

	const char* shaderSource = source.c_str();
	glShaderSource(id, 1, &shaderSource, 0);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (!result)
	{
		int lenght;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght);
		char* message = (char*)_malloca(lenght * sizeof(char));
		glGetShaderInfoLog(id, lenght, &lenght, message);
			glDeleteShader(id);
		return 0;
	}

	return id;
}

std::string Shader::readShader(const std::string& path)
{
	std::fstream stream(path.c_str());

	if (!stream)
		return "ERR";

	std::string source;
	std::string line;
	while (std::getline(stream, line))
		source += line + "\n";

	return source;
}

ShaderSource Shader::readShaderFile(const std::string& path)
{
	std::fstream stream(path.c_str());

	if (!stream)
			return { "ERR" , "ERR" };

	enum class mode
	{
		DEFAULT,
		VERTEX,
		FRAGMENT,
	};

	mode readMode = mode::DEFAULT;

	std::string line;
	std::string vertexSource;
	std::string fragmentSource;

	while (std::getline(stream, line))
	{
		if (line.find("#vertex") != std::string::npos)
		{
			readMode = mode::VERTEX;
			continue;
		}
		else if (line.find("#fragment") != std::string::npos)
		{
			readMode = mode::FRAGMENT;
			continue;
		}

		if (readMode == mode::VERTEX)
			vertexSource += line + "\n";
		else if (readMode == mode::FRAGMENT)
			fragmentSource += line + "\n";
	}

	return { vertexSource, fragmentSource };
}

const void Shader::setUniform1i(const std::string& name, int value)
{
	glUseProgram(m_Handle);
	glUniform1i(SearchUnfiformLocation(name), value);
}

const void Shader::setUniformMat4(const std::string& name, const glm::mat4& mat)
{
	glUseProgram(m_Handle);
	glUniformMatrix4fv(SearchUnfiformLocation(name), 1, GL_FALSE, &mat[0][0]);
}

const void Shader::setUniform1f(const std::string& name, float value)
{
	glUseProgram(m_Handle);
	glUniform1f(SearchUnfiformLocation(name), value);
}

const void Shader::setUniformVec2f(const std::string& name, const glm::vec2& vec2f)
{
	glUseProgram(m_Handle);
	glUniform2f(SearchUnfiformLocation(name), vec2f.x, vec2f.y);
}

const void Shader::setUniformVec3f(const std::string& name, const glm::vec3& vec3f)
{
	glUseProgram(m_Handle);
	glUniform3f(SearchUnfiformLocation(name), vec3f.x, vec3f.y, vec3f.z);
}

const void Shader::setUniformVec4f(const std::string& name, const glm::vec4& vec4f)
{
	glUseProgram(m_Handle);
	glUniform4f(SearchUnfiformLocation(name), vec4f.x, vec4f.y, vec4f.z, vec4f.w);
}

const void Shader::setUniformIntArray(const std::string& name, int* values, unsigned int size)
{
	glUseProgram(m_Handle);
	glUniform1iv(SearchUnfiformLocation(name), size, values);
}

int Shader::SearchUnfiformLocation(const std::string& name)
{
	if (m_LocationCache.find(name) != m_LocationCache.end())
		return m_LocationCache[name];

	int location = glGetUniformLocation(m_Handle, name.c_str());
	if (location == -1)
		return -1;
	m_LocationCache[name] = location;
	return location;

}

