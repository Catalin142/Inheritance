#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <string>

class Texture
{
private:
	unsigned int m_TextureID;
	unsigned int m_Width;
	unsigned int m_Height;
	unsigned int m_Format;

public:
	Texture() = default;
	Texture(unsigned int width, unsigned int height, unsigned int format, unsigned int internatlformat, unsigned int depth, 
		unsigned int Filter = GL_LINEAR, unsigned int Wrap = GL_CLAMP_TO_EDGE);
	Texture(const std::string& filepath, unsigned int Filter = GL_LINEAR, unsigned int Wrap = GL_CLAMP_TO_EDGE);
	~Texture();

	void setData(void* data);

	void Bind(unsigned int slot = 0) const;

	const unsigned int getWidth() const { return m_Width; }
	const unsigned int getHeight() const { return m_Height; }
	const unsigned int GetID() const { return m_TextureID; }

	bool operator==(const Texture& tex);

	const std::string m_Filepath;
};