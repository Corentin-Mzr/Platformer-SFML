#pragma once

#include <unordered_map>
#include <filesystem>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include "animation.hpp"

class AssetManager
{
public:
    explicit AssetManager() noexcept = default;

    /**
     * @brief Store a texture, does nothing if texture is already stored
     *
     * @param name Name used to store the texture in the map
     * @param path Path to the texture file
     */
    void add_texture(const std::string &name, const std::filesystem::path &path) noexcept;

    /**
     * @brief Store an animation, does nothing if animation is already stored
     *
     * @param name Name used to store the animation in the map
     * @param anim Animation to store
     */
    void add_animation(const std::string &name, const Animation &anim) noexcept;

    /**
     * Store a font, does nothing if fond is already stored
     *
     * @param name Name used to store the font in the map
     * @param path Path to the font file
     */
    void add_font(const std::string &name, const std::filesystem::path &path) noexcept;

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

private:
    /* Delete copy */
    AssetManager(const AssetManager &) noexcept = delete;
    AssetManager &operator=(const AssetManager &) noexcept = delete;
    AssetManager(AssetManager &&) noexcept = delete;
    AssetManager &operator=(AssetManager &&) noexcept = delete;

private:
    std::unordered_map<std::string, sf::Texture> m_textures{};
    std::unordered_map<std::string, Animation> m_animations{};
    std::unordered_map<std::string, sf::Font> m_fonts{};
};
