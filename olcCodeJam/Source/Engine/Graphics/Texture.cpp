#include <string>

#include "Texture.h"

#include "std_image.h"

Texture::Texture(unsigned int width, unsigned int height, unsigned int format, unsigned int internatlformat, unsigned int depth, unsigned int Filter, unsigned int Wrap) :
    m_Width(width), m_Height(height)
{
    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    glTexStorage2D(GL_TEXTURE_2D, depth, internatlformat, m_Width, m_Height);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Wrap);

    m_Format = format;
}

Texture::Texture(const std::string& filepath, unsigned int Filter, unsigned int Wrap) :
    m_TextureID(0), m_Width(0), m_Height(0), m_Filepath(filepath)
{
    int width, height, bbp;
    stbi_set_flip_vertically_on_load(true);

    unsigned char* texData = nullptr;
    texData = stbi_load(filepath.c_str(), &width, &height, &bbp, 0);

    m_Width = width;
    m_Height = height;

    unsigned int internalFormat = 0, Format = 0;
    if (bbp == 3) {
        internalFormat = GL_RGB8;
        Format = GL_RGB;
    }

    else if (bbp == 4) {
        internalFormat = GL_RGBA8;
        Format = GL_RGBA;
    }

    else if (bbp == 1)
    {
        internalFormat = GL_RGBA;
        Format = GL_RED;
    }

    m_Format = Format;
    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (Filter == GL_LINEAR) ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Wrap);

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, Format, GL_UNSIGNED_BYTE, texData);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(texData);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_TextureID);
}

void Texture::setData(void* data)
{
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, m_Format, GL_UNSIGNED_BYTE, data);
}

void Texture::Bind(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
}

bool Texture::operator==(const Texture& tex)
{
    if (m_TextureID == tex.GetID())
        return true;
    return false;
}