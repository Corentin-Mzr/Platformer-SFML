#pragma once

#include <tuple>
#include <string>
#include "components.hpp"

using ComponentTuple = std::tuple<CTransform, CLifeSpan, CInput, CBoundingBox, CAnimation, CGravity, CState, CJump>;

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
    void remove() const noexcept;

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
    explicit Entity() noexcept = default;
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
void Entity::remove() const noexcept
{
    get<T>() = T();
}
