#pragma once

#include <tuple>
#include "components.hpp"

using ComponentTuple = std::tuple<CTransform, CLifeSpan, CInput, CBoundingBox, CAnimation, CGravity, CState, CJump, CSound, CBoundingConvex>;

/**
 * @brief Represents a single entity.
 * 
 * Each entity can have multiple components which define its data and behavior.
 * 
 * The class provides methods to add, remove and access components.
 * It also stored a unique id, a tag and an alive/dead status.
 * 
 * Entites are managed by the EntityManager class and are non-movable and non-copyable.
 *
 * Usage:
 * 
 * - add<T>(...) to attach a component of type T
 * 
 * - has<T>() to check component
 * 
 * - get<T>() to access a component 
 * 
 * - destroy() to mark the entity as dead
 */
class Entity
{
    friend class EntityManager;

public:
    /**
     * @brief Add a component to the entity
     */
    template <typename T, typename... Args>
    void add(Args &&...args);

    /**
     * @brief Return the entity component
     */
    template <typename T>
    [[nodiscard]]
    T &get() noexcept;

    /**
     * @brief Return the entity component
     */
    template <typename T>
    [[nodiscard]]
    const T &get() const noexcept;

    /**
     * @brief Check if the entity has the given component type
     */
    template <typename T>
    [[nodiscard]]
    bool has() const noexcept;

    /**
     * @brief Remove a component
     */
    template <typename T>
    void remove() noexcept;

    /**
     * @brief Return entity's id
     */
    [[nodiscard]]
    size_t id() const noexcept;

    /**
     * @brief Check if entity is alive
     */
    [[nodiscard]]
    bool is_alive() const noexcept;

    /**
     * @brief Return entity's tag
     */
    [[nodiscard]]
    const std::string &tag() const noexcept;

    /**
     * @brief Kill / Destroy the entity
     */
    void destroy() noexcept;

private:
    /**
     * @brief Default constructor
     */
    explicit Entity() noexcept = default;

    /**
     * @brief Create a new entity
     * 
     * @param tag Entity's tag
     * @param id  Entity's id
     */
    explicit Entity(std::string tag, size_t id) noexcept;

    /* Delete move and copy */
    Entity(const Entity &) noexcept = delete;
    Entity &operator=(const Entity &) noexcept = delete;
    Entity(Entity &&) noexcept = delete;
    Entity &operator=(Entity &&) noexcept = delete;

private:
    ComponentTuple m_components{};
    bool m_alive{true};
    const std::string m_tag{"default"};
    const size_t m_id{};
};

/* TEMPLATE FUNCTIONS HERE */

template <typename T, typename... Args>
void Entity::add(Args &&...args)
{
    auto &component = get<T>();
    component = T(std::forward<Args>(args)...);
    component.exists = true;
    std::get<T>(m_components) = component;
}

template <typename T>
T &Entity::get() noexcept
{
    return std::get<T>(m_components);
}

template <typename T>
const T &Entity::get() const noexcept
{
    return std::get<T>(m_components);
}

template <typename T>
bool Entity::has() const noexcept
{
    return get<T>().exists;
}

template <typename T>
void Entity::remove() noexcept
{
    get<T>() = T();
}
