#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include "entity.hpp"

using EntityVec = std::vector<std::shared_ptr<Entity>>;
using EntityMap = std::unordered_map<std::string, EntityVec>;

/**
 * @brief Manages all entities within a scene.
 * 
 * The EntityManager is responsible for creating, storing, updating and removing entities.
 * 
 * The Manager keeps track of: 
 * 
 * - active entities
 * 
 * - entities pending addition
 * 
 * - a map for lookup by tag or id
 * 
 * Usage:
 * 
 * - add_entity(tag): Create and store a new entity with a unique id, and give it a tag for fast retrieval of entities of the same type
 * 
 * - get_entities(): Access entities, can be filtered by tag
 * 
 * - update(): Update entities and clean up dead ones
 * 
 * 
 * @note Each scene owns it own EntityManager, copy or move this class between scenes is not possible.
 */
class EntityManager
{

public:
    /**
     * @brief Default constructor
     */
    explicit EntityManager() noexcept = default;

    /* Each scene will have its own manager, so no copy or move between scenes */
    EntityManager(const EntityManager &) noexcept = delete;
    EntityManager &operator=(const EntityManager &) noexcept = delete;
    EntityManager(EntityManager &&) noexcept = delete;
    EntityManager &operator=(EntityManager &&) noexcept = delete;

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
    /**
     * @brief Remove dead entities from the entity manager
     * 
     * @param vec Entities to remove
     */
    void remove_dead_entities(EntityVec &vec) noexcept;

private:
    EntityVec m_entities{};
    EntityVec m_entities_to_add{};
    EntityMap m_entity_map{};
    size_t m_total_entities{};
};
