#include <memory>
#include <unordered_map>

#include "FontManager.h"

std::vector<std::shared_ptr<Font>> FontManager::m_Fonts;

const std::shared_ptr<Font>& FontManager::getFont(const std::string& name)
{
    for (const auto& font : m_Fonts)
    {
        if (font->getName() == name)
            return font;
    }
    return std::shared_ptr<Font>();
}

void FontManager::addFont(const std::string& name, const std::string filepath, float size)
{
    std::shared_ptr<Font> font = std::make_shared<Font>(name, filepath, size);
    m_Fonts.push_back(font);
}