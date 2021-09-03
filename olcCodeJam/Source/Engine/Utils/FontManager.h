#pragma once

#include "Font.h"

class FontManager
{
private:
	static std::vector<std::shared_ptr<Font>> m_Fonts;

	FontManager() = default;
	~FontManager() = default;

public:
	static const std::shared_ptr<Font>& getFont(const std::string& name);
	static void addFont(const std::string& name, const std::string filepath, float size);
};
