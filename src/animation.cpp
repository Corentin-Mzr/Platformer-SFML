#include "animation.hpp"
#include <SFML/Graphics/Texture.hpp>

Animation::Animation(const std::string &name, const sf::Texture &t) : Animation(name, t, 1, 0)
{
}

Animation::Animation(std::string name, const sf::Texture &t, unsigned frame_count, unsigned speed)
    : m_sprite(t), m_frame_count(frame_count), m_current_frame(0), m_speed(speed == 0 ? 1 : speed), m_name(std::move(name))
{
    m_size = {static_cast<int>(t.getSize().x) / static_cast<int>(frame_count), static_cast<int>(t.getSize().y)};
    m_sprite->setOrigin({0.5f * m_size.x, 0.5f * m_size.y});
    m_sprite->setTextureRect(sf::IntRect{{static_cast<int>(m_current_frame * m_size.x), 0}, m_size});
}

void Animation::update() noexcept
{
    m_current_frame++;
    const int anim_frame = (m_current_frame / m_speed) % m_frame_count;
    const sf::IntRect rect{{anim_frame * m_size.x, 0}, m_size};

    if (m_sprite.has_value()) [[likely]]
    {
        m_sprite->setTextureRect(rect);
    }
}

bool Animation::has_ended() const noexcept
{
    return m_current_frame == m_frame_count;
}

const std::string &Animation::get_name() const noexcept
{
    return m_name;
}

const sf::Vector2i &Animation::get_size() const noexcept
{
    return m_size;
}

const sf::Sprite &Animation::get_sprite() const
{
    /* Can throw error */
    return m_sprite.value();
}

sf::Sprite &Animation::get_sprite()
{
    /* Can throw error */
    return m_sprite.value();
}

void Animation::set_scale(const sf::Vector2f &s)
{
    if (m_sprite)
    {
        m_sprite->setScale(s);
    }
}

void Animation::set_origin(OriginAnchor a)
{
    if (!m_sprite)
    {
        return;
    }

    switch (a)
    {
        case OriginAnchor::Center:
        {
            m_sprite->setOrigin(sf::Vector2f{0.5f * m_size.x, 0.5f * m_size.y});
            break;
        }

        case OriginAnchor::BottomLeft:
        {
            m_sprite->setOrigin(sf::Vector2f{0.0f, static_cast<float>(m_size.y)});
            break;
        }

        default:
        {
            break;
        }
    }
}