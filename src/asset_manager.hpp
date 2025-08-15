#pragma once

#include <unordered_map>
#include <filesystem>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Music.hpp>
#include "animation.hpp"

using TextureMap = std::unordered_map<std::string, sf::Texture>;
using AnimationMap = std::unordered_map<std::string, Animation>;
using FontMap = std::unordered_map<std::string, sf::Font>;
using SoundMap = std::unordered_map<std::string, sf::SoundBuffer>;
using MusicMap = std::unordered_map<std::string, sf::Music>;

/**
 * @brief Centralized storage and retrieval of game assets.
 * 
 * The AssetManager is resposible for loading, storing and providing access to
 * common game resources: textures, animations, fonts, sounds and musics.
 * 
 * Assets are identified by string key, allowing them to be retrieved anywhere in the code without reloading them.
 * 
 * Each asset is loaded only once and remains available as long as the AssetManager exists.
 * If an asset is not found, a placeholder resource is returned instead.
 * 
 * @note This class is non-copyable and non-movable to avoid duplication of resources.
 */
class AssetManager
{
public:
    /**
     * @brief Default constructor
     */
    explicit AssetManager() noexcept = default;

    /* Delete copy and move */
    AssetManager(const AssetManager &) noexcept = delete;
    AssetManager &operator=(const AssetManager &) noexcept = delete;
    AssetManager(AssetManager &&) noexcept = delete;
    AssetManager &operator=(AssetManager &&) noexcept = delete;

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
     * @brief Store a music
     *
     * @param name Name used to store the music in the map
     * @param path Path to the music file
     */
    void add_music(const std::string &name, const std::filesystem::path &path) noexcept;

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

    /**
     * @brief Return the stored music, or a default music if not stored. MUSIC STATE CAN BE CHANGED
     *
     * @param name Music's name
     */
    [[nodiscard]]
    sf::Music &get_music(const std::string &name) noexcept;

private:
    TextureMap m_textures{};
    AnimationMap m_animations{};
    FontMap m_fonts{};
    SoundMap m_sounds{};
    MusicMap m_musics{};
};
