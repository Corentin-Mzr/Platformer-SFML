#pragma once

#include <unordered_map>
#include <filesystem>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include "animation.hpp"

using TextureMap = std::unordered_map<std::string, sf::Texture>;
using AnimationMap = std::unordered_map<std::string, Animation>;
using FontMap = std::unordered_map<std::string, sf::Font>;
using SoundMap = std::unordered_map<std::string, sf::SoundBuffer>;

class AssetManager
{
public:
    explicit AssetManager() noexcept = default;

    /**
     * @brief Store a texture
     *
     * @param name Name used to store the texture in the map
     * @param path Path to the texture file
     */
    void add_texture(const std::string &name, const std::filesystem::path &path) noexcept;

    /**
     * @brief Store an animation
     *
     * @param name Name used to store the animation in the map
     * @param anim Animation to store
     */
    void add_animation(const std::string &name, const Animation &anim) noexcept;

    /**
     * @brief Store a font
     *
     * @param name Name used to store the font in the map
     * @param path Path to the font file
     */
    void add_font(const std::string &name, const std::filesystem::path &path) noexcept;

    /**
     * @brief Store a sound
     * 
     * @param name Name used to store the sound in the map
     * @param path Path to the sound file
     */
    void add_sound(const std::string &name, const std::filesystem::path &path) noexcept;

    /**
     * @brief Return the stored texture, or a default texture if not stored
     *
     * @param name Texture's name
     */
    [[nodiscard]]
    const sf::Texture &get_texture(const std::string &name) const noexcept;

    /**
     * @brief Return the stored animation, or a default animation if not stored
     *
     * @param name Animation's name
     */
    [[nodiscard]]
    const Animation &get_animation(const std::string &name) const noexcept;

    /**
     * @brief Return the stored font, or a default font if not stored
     *
     * @param name Font's name
     */
    [[nodiscard]]
    const sf::Font &get_font(const std::string &name) const noexcept;

    /**
     * @brief Return the stored sound, or a default sound if not stored
     * 
     * @param name Sound's name
     */
    [[nodiscard]]
    const sf::SoundBuffer &get_sound(const std::string &name) const noexcept;

private:
    /* Delete copy */
    AssetManager(const AssetManager &) noexcept = delete;
    AssetManager &operator=(const AssetManager &) noexcept = delete;
    AssetManager(AssetManager &&) noexcept = delete;
    AssetManager &operator=(AssetManager &&) noexcept = delete;

private:
    TextureMap m_textures{};
    AnimationMap m_animations{};
    FontMap m_fonts{};
    SoundMap m_sounds{};
};

