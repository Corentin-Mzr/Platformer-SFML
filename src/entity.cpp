#include "entity.hpp"

Entity::Entity(std::string tag, size_t id) noexcept
    : m_tag(std::move(tag)), m_id(id)
{
}

size_t Entity::id() const noexcept
{
    return m_id;
}

bool Entity::is_alive() const noexcept
{
    return m_alive;
}

const std::string &Entity::tag() const noexcept
{
    return m_tag;
}

void Entity::destroy() noexcept
{
    m_alive = false;
}