#pragma once

#include <SFML/Graphics.hpp>

#include "animation.hpp"

struct Component
{
    bool exists{false};
};

struct CTransform : public Component
{
    sf::Vector2f pos{};
    sf::Vector2f previous_pos{};
    sf::Vector2f velocity{};
    sf::Vector2f scale{1.0f, 1.0f};
    float angle{};

    explicit CTransform() noexcept = default;
    explicit CTransform(const sf::Vector2f &p) noexcept : pos(p)
    {
    }

    explicit CTransform(const sf::Vector2f &p, const sf::Vector2f &v, const sf::Vector2f &s, float a) noexcept
        : pos(p), previous_pos(p), velocity(v), scale(s), angle(a)
    {
    }
};

struct CLifeSpan : public Component
{
    int lifespan{};
    int frame_created{};

    explicit CLifeSpan() noexcept = default;
    explicit CLifeSpan(int total_lifespan, int frame_created) noexcept
        : lifespan(total_lifespan), frame_created(frame_created)
    {
    }
};

struct CInput : public Component
{
    bool up{false};
    bool down{false};
    bool left{false};
    bool right{false};
    bool shoot{false};
    bool can_shoot{true};
    bool can_jump{true};

    explicit CInput() noexcept = default;
};

struct CBoundingBox : public Component
{
    sf::Vector2f size{};
    sf::Vector2f half_size{};

    explicit CBoundingBox() noexcept = default;
    explicit CBoundingBox(const sf::Vector2f &s) noexcept : size(s), half_size(0.5f * s)
    {
    }
};

struct CAnimation : public Component
{
    Animation animation{};
    bool repeat{false};

    explicit CAnimation() noexcept = default;
    explicit CAnimation(Animation a, bool r) noexcept : animation(std::move(a)), repeat(r)
    {
    }
};

struct CGravity : public Component
{
    float gravity{};
    explicit CGravity() noexcept = default;
    explicit CGravity(float g) noexcept : gravity(g)
    {
    }
};

struct CState : public Component
{
    std::string state{"idle"};
    std::string previous_state{"idle"};
    bool change_animation{false};
    explicit CState() noexcept = default;
    explicit CState(std::string s) noexcept : state(std::move(s))
    {
    }
};
