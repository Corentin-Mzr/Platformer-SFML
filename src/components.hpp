#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Audio/Sound.hpp>
#include "animation.hpp"

/**
 * @brief Abstract base component
 */
struct Component
{
    bool exists{false};
    virtual ~Component() noexcept = 0;
};
inline Component::~Component() noexcept = default;

/**
 * @brief Transform component for position, velocity, etc.
 */
struct CTransform : public Component
{
    sf::Vector2f pos{};
    sf::Vector2f previous_pos{};
    sf::Vector2f velocity{};
    sf::Vector2f scale{1.0f, 1.0f};
    float angle{};

    /**
     * @brief Default constructor
     */
    explicit CTransform() noexcept = default;

    /**
     * @brief Create a Transform Component
     *
     * @param p Position
     */
    explicit CTransform(const sf::Vector2f &p) noexcept : pos(p)
    {
    }

    /**
     * @brief Create a Transform Component
     *
     * @param p Position
     * @param v Velocity
     * @param s Scale
     * @param a Angle
     */
    explicit CTransform(const sf::Vector2f &p, const sf::Vector2f &v, const sf::Vector2f &s, float a) noexcept
        : pos(p), previous_pos(p), velocity(v), scale(s), angle(a)
    {
    }
};

/**
 * @brief Lifespan component to know when to destroy an entity
 */
struct CLifeSpan : public Component
{
    unsigned lifespan{};
    unsigned frame_created{};

    /**
     * @brief Default constructor
     */
    explicit CLifeSpan() noexcept = default;

    /**
     * @brief Create a LifeSpan Component
     *
     * @param total_lifespan Lifespan duration
     * @param frame_create Frame when the component was created
     */
    explicit CLifeSpan(unsigned total_lifespan, unsigned frame_created) noexcept
        : lifespan(total_lifespan), frame_created(frame_created)
    {
    }
};

/**
 * @brief Input component to handle user inputs
 */
struct CInput : public Component
{
    bool up{false};
    bool down{false};
    bool left{false};
    bool right{false};
    bool shoot{false};
    bool can_shoot{true};
    bool can_jump{true};

    /**
     * @brief Default constructor
     */
    explicit CInput() noexcept = default;
};

/**
 * @brief Bounding box component to know if entities collide
 */
struct CBoundingBox : public Component
{
    sf::Vector2f size{};
    sf::Vector2f half_size{};

    /**
     * @brief Default constructor
     */
    explicit CBoundingBox() noexcept = default;

    /**
     * @brief Create a BoundingBox Component
     *
     * @param s Box size
     */
    explicit CBoundingBox(const sf::Vector2f &s) noexcept : size(s), half_size(0.5f * s)
    {
    }
};

/**
 * @brief Animation component to make an animated entity
 */
struct CAnimation : public Component
{
    Animation animation{};
    bool repeat{false};

    /**
     * @brief Default constructor
     */
    explicit CAnimation() noexcept = default;

    /**
     * @brief Create an Animation Component
     *
     * @param a Animation
     * @param r True if the animation is repeated
     */
    explicit CAnimation(Animation a, bool r) noexcept : animation(std::move(a)), repeat(r)
    {
    }
};

/**
 * @brief Gravity component to apply a downward force on an entity
 */
struct CGravity : public Component
{
    float gravity{};

    /**
     * @brief Default constructor
     */
    explicit CGravity() noexcept = default;

    /**
     * @brief Create a Gravity Component
     *
     * @param g Gravity value
     */
    explicit CGravity(float g) noexcept : gravity(g)
    {
    }
};

/**
 * @brief State component to know the current state of an entity
 */
struct CState : public Component
{
    std::string state{"idle"};
    std::string previous_state{"idle"};
    bool change_animation{false};

    /**
     * @brief Default constructor
     */
    explicit CState() noexcept = default;

    /**
     * @brief Create a State Component
     *
     * @brief s State
     */
    explicit CState(std::string s) noexcept : state(std::move(s))
    {
    }
};

/**
 * @brief Jump component to handle variable jump height 
 */
struct CJump : public Component
{
    bool jumping{false};
    unsigned start_frame{}; // Updated when input.up == true
    unsigned max_duration{};
    float initial_strength{};
    float frame_strength{};

    /**
     * @brief Default constructor
     */
    explicit CJump() noexcept = default;

    /**
     * @brief Create a Jump Component
     *
     * @param str Jump Initial Strength
     * @param dur Max jump duration in frames
     * @param str_per_frame Jump strength added each frame if up key is still pressed
     */
    explicit CJump(float init_str, unsigned dur, float str_per_frame) noexcept : max_duration(dur), initial_strength(init_str), frame_strength(str_per_frame)
    {
    }
};

/**
 * @brief Sound component to play a sound  
 */
struct CSound : public Component
{
    std::optional<sf::Sound> sound{};
    bool loop{false};
    bool played{false};

    /**
     * @brief Default constructor
     */
    explicit CSound() noexcept = default;

    /**
     * @brief Create a Sound Component
     *
     * @param buffer Sound Buffer
     * @param loop True if sound loops
     */
    explicit CSound(const sf::SoundBuffer &buffer, bool loop, float volume = 100.0f) noexcept : loop(loop)
    {
        sound.emplace(buffer);
        sound->setLooping(loop);
        sound->setVolume(volume);
    }
};

/**
 * @brief Convex Hitbox component
 * 
 * @note Points must be given in local space
 */
struct CBoundingConvex : public Component
{
    std::vector<sf::Vector2f> points{};
    sf::Vector2f scale{1.0f, 1.0f};
    size_t count{};

    /**
     * @brief Default constructor
     */
    explicit CBoundingConvex() noexcept = default;

    /**
     * @brief Create a BoundingConvex component
     * 
     * @param p Points representing the convex shape, in local space
     * @param s Scaling factor, equivalent of "size" for CBoundingBox
     */
    explicit CBoundingConvex(const std::vector<sf::Vector2f> &p, const sf::Vector2f &s) noexcept : points(std::move(p)), scale(s), count(p.size())
    {
    }
};
