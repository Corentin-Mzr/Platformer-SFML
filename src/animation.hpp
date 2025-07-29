#pragma once

#include <string>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

class Animation
{
public:
    explicit Animation() noexcept = default;
    explicit Animation(const std::string &name, const sf::Texture &t);
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
     * @brief Return the animation's texture size
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
    sf::Sprite &get_sprite();

private:
    std::optional<sf::Sprite> m_sprite{};
    unsigned m_frame_count{1};
    unsigned m_current_frame{};
    unsigned m_speed{};
    sf::Vector2i m_size{1, 1};
    std::string m_name{"default"};
};
