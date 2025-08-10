#include "asset_manager.hpp"
#include <iostream>
#include <format>

void AssetManager::add_texture(const std::string &name, const std::filesystem::path &path) noexcept
{
    sf::Texture texture;
    if (!texture.loadFromFile(path))
    {
        std::cerr << std::format("Could not load texture {}\n", path.string());
        return;
    }
    m_textures[name] = texture;
}

void AssetManager::add_animation(const std::string &name, const Animation &anim) noexcept
{
    /* Check if we dont overwrite an existing animation */
    if (m_animations.find(name) != m_animations.end())
    {
        std::cerr << std::format("Animation {} already exists, overwriting it\n", name);
    }
    m_animations[name] = anim;
}

void AssetManager::add_font(const std::string &name, const std::filesystem::path &path) noexcept
{
    sf::Font font;
    if (!font.openFromFile(path))
    {
        std::cerr << std::format("Could not load font {}\n", path.string());
        return;
    }
    m_fonts[name] = font;
}

const sf::Texture &AssetManager::get_texture(const std::string &name) const noexcept
{
    const static sf::Texture texture_not_found{};
    auto it{m_textures.find(name)};
    return it != m_textures.end() ? it->second : texture_not_found;
}

const Animation &AssetManager::get_animation(const std::string &name) const noexcept
{
    static const std::shared_ptr<Animation> anim_not_found{std::make_shared<Animation>()};
    auto it{m_animations.find(name)};
    return it != m_animations.end() ? it->second : *anim_not_found;
}

const sf::Font &AssetManager::get_font(const std::string &name) const noexcept
{
    const static sf::Font font_not_found{};
    auto it{m_fonts.find(name)};
    return it != m_fonts.end() ? it->second : font_not_found;
}
