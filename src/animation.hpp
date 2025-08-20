#pragma once

#include <string>
#include <SFML/Graphics/Sprite.hpp>

enum class OriginAnchor {
    Center,
    BottomLeft
};

/**
 * @brief Represents a texture-based animation made of one or more frames.
 * 
 * Manages the animation state (current frame) and provides access to a sprite for rendering.
 * 
 * An animation can be a single frame or multiple frames arranged in the texture.
 * The update() function changes the animation frame based on the defined speed.
 * 
 * @note Animation speed cannot not be zero.
 */
class Animation
{
public:
    /**
     * @brief Default constructor
     */
    explicit Animation() noexcept = default;

    /**
     * @brief Create an animation that has one frame
     * 
     * @param name Animation name
     * @param t Texture used for the animation
     */
    explicit Animation(const std::string &name, const sf::Texture &t);

    /**
     * @brief Create an animation that has multiple frames
     * 
     * @param name Animation name
     * @param t Texture used for the animation
     * @param frame_count Number of frames in the animation
     * @param speed Number of in-game frames before the animation updates
     */
    explicit Animation(std::string name, const sf::Texture &t, unsigned frame_count, unsigned speed);

    /**
     * @brief Update the animation
     */
    void update() noexcept;

    /**
     * @brief Return true if animation reaches the last frame
     */
    [[nodiscard]] 
    bool has_ended() const noexcept;

    /**
     * @brief Return animation's name
     */
    [[nodiscard]] 
    const std::string &get_name() const noexcept;

    /**
     * @brief Return the animation's based texture size
     */
    [[nodiscard]] 
    const sf::Vector2i &get_size() const noexcept;

    /**
     * @brief Return the animation's sprite
     */
    [[nodiscard]] 
    const sf::Sprite &get_sprite() const;

    /**
     * @brief Return the animation's sprite
     */
    [[nodiscard]]
    sf::Sprite &get_sprite();

    void set_scale(const sf::Vector2f &s);

    void set_origin(OriginAnchor a);

private:
    std::optional<sf::Sprite> m_sprite{};
    unsigned m_frame_count{1};
    unsigned m_current_frame{};
    unsigned m_speed{1};
    sf::Vector2i m_size{1, 1};
    std::string m_name{"default"};
};
