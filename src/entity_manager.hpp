#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <cassert>
#include "entity.hpp"

using EntityVec = std::vector<std::shared_ptr<Entity>>;
using EntityMap = std::unordered_map<std::string, EntityVec>;

class EntityManager
{

public:
    explicit EntityManager() noexcept = default;
    EntityManager(const EntityManager &) noexcept = default;
    EntityManager &operator=(const EntityManager &) noexcept = default;
    EntityManager(EntityManager &&) noexcept = default;
    EntityManager &operator=(EntityManager &&) noexcept = default;

    /**
     * @brief Add an entity with the given tag
     * 
     * @param tag Entity's tag / name
     */
    [[nodiscard]] 
    std::shared_ptr<Entity> add_entity(const std::string &tag) noexcept;

    /**
     * @brief Return all entities
     */
    [[nodiscard]] 
    EntityVec &get_entities() noexcept;

    /**
     * @brief Return entities with the given tag
     * 
     * @param tag Entities tag
     */
    [[nodiscard]] 
    EntityVec &get_entities(const std::string &tag) noexcept;

    /**
     * @brief Return the entity map
     */
    [[nodiscard]] 
    const EntityMap &get_entity_map() const noexcept; 
    
    /**
     * @brief Update entities
     */
    void update() noexcept;

private:
    EntityVec m_entities{};
    EntityVec m_entities_to_add{};
    EntityMap m_entity_map{};
    size_t m_total_entities{};

    /**
     * @brief Remove dead entities from the entity manager
     */
    void remove_dead_entities(EntityVec &vec) noexcept;
};
