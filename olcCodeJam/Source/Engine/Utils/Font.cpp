#include <memory>
#include "Font.h"

#include <FreeType/freetype-gl/freetype-gl.h>
#include "GL/glew.h"

Font::Font(const std::string& name, const std::string& fontpath, float size) :
	m_Name(name), m_Size(size)
{
	m_Atlas = ftgl::texture_atlas_new(1024, 1024, 2);
	m_Font = ftgl::texture_font_new_from_file(m_Atlas, size, fontpath.c_str());

	m_Texture = std::make_shared<Texture>(m_Atlas->width, m_Atlas->height, GL_LUMINANCE_ALPHA, GL_RGBA8, m_Atlas->depth, GL_LINEAR, GL_CLAMP_TO_EDGE);
	m_Texture->setData(m_Atlas->data);
}

void Font::updateAtlas() const
{
	if (m_Atlas->dirty)
	{
		m_Texture->setData(m_Atlas->data);
		m_Atlas->dirty = false;
	}
}