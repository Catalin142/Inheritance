#pragma once

#include "Engine/Graphics/Shader.h"
#include "Engine/Graphics/Texture.h"

namespace ftgl {
	struct texture_atlas_t;
	struct texture_font_t;
}

class Font
{
private:
	ftgl::texture_atlas_t* m_Atlas;
	ftgl::texture_font_t* m_Font;
	float m_Size;
	const std::string m_Name;
	std::shared_ptr<Texture> m_Texture;

public:
	Font(const std::string& name, const std::string& fontpath, float size);

	ftgl::texture_font_t* getFont() const { updateAtlas();  return m_Font; }
	ftgl::texture_atlas_t* getAtlas() { updateAtlas();  return m_Atlas; }
	const std::shared_ptr<Texture>& getTexture() const { updateAtlas(); return m_Texture; }
	const std::string& getName() const { return m_Name; }
	const float getSize() const { return m_Size; }
private:
	void updateAtlas() const;

};
