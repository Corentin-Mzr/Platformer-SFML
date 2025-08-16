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

    if (!m_textures.try_emplace(name, texture).second)
    {
        std::cerr << std::format("Texture {} already stored\n", name);
    }
}

void AssetManager::add_animation(const std::string &name, const Animation &anim) noexcept
{
    if (!m_animations.try_emplace(name, std::move(anim)).second)
    {
        std::cerr << std::format("Animation {} already stored\n", name);
    }
}

void AssetManager::add_font(const std::string &name, const std::filesystem::path &path) noexcept
{
    sf::Font font;
    if (!font.openFromFile(path))
    {
        std::cerr << std::format("Could not load font {}\n", path.string());
        return;
    }

    if (!m_fonts.try_emplace(name, std::move(font)).second)
    {
        std::cerr << std::format("Font {} already stored\n", name);
    }
}

void AssetManager::add_sound(const std::string &name, const std::filesystem::path &path) noexcept
{
    sf::SoundBuffer sound;
    if (!sound.loadFromFile(path))
    {
        std::cerr << std::format("Could not load sound {}\n", path.string());
        return;
    }
    
    if (!m_sounds.try_emplace(name, std::move(sound)).second)
    {
        std::cerr << std::format("Sound {} already stored\n", name);
    }
}

void AssetManager::add_music(const std::string &name, const std::filesystem::path &path) noexcept
{
    sf::Music music;
    if (!music.openFromFile(path))
    {
        std::cerr << std::format("Could not load music {}\n", path.string());
        return;
    }

    if (!m_musics.try_emplace(name, std::move(music)).second)
    {
        std::cerr << std::format("Music {} already stored\n", name);
    }
}

const sf::Texture &AssetManager::get_texture(const std::string &name) const
{
    return m_textures.at(name);

    // const static sf::Texture texture_not_found{};
    // auto it{m_textures.find(name)};
    // return it != m_textures.end() ? it->second : texture_not_found;
}

const Animation &AssetManager::get_animation(const std::string &name) const
{
    return m_animations.at(name);

    // const static std::shared_ptr<Animation> anim_not_found{std::make_shared<Animation>()};
    // auto it{m_animations.find(name)};
    // return it != m_animations.end() ? it->second : *anim_not_found;
}

const sf::Font &AssetManager::get_font(const std::string &name) const
{
    return m_fonts.at(name);

    // const static sf::Font font_not_found{};
    // auto it{m_fonts.find(name)};
    // return it != m_fonts.end() ? it->second : font_not_found;
}

const sf::SoundBuffer &AssetManager::get_sound(const std::string &name) const
{
    return m_sounds.at(name);

    // const static sf::SoundBuffer sound_not_found{};
    // auto it{m_sounds.find(name)};
    // return it != m_sounds.end() ? it->second : sound_not_found;
}

sf::Music &AssetManager::get_music(const std::string &name)
{
    return m_musics.at(name);
    
    // static sf::Music music_not_found{};
    // auto it{m_musics.find(name)};
    // return it != m_musics.end() ? it->second : music_not_found;
}
