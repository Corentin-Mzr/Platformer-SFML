#pragma once

#include <memory>
#include <cassert>
#include <SFML/System/Vector2.hpp>
#include "entity.hpp"

namespace Physics
{
    /**
     * @brief Get the current overlap between two entities bounding boxes. Entities collide iff ox > 0 && oy > 0
     * 
     * @param a First entity
     * @param b Second entity
     */
    [[nodiscard]]
    inline sf::Vector2f get_current_overlap(const std::shared_ptr<Entity> a, const std::shared_ptr<Entity> b)
    {
        // assert(a->has<CBoundingBox>() && a->has<CTransform>() && b->has<CBoundingBox>() && b->has<CTransform>());
        const auto &a_box_half_size = a->get<CBoundingBox>().half_size;
        const auto &a_current_pos = a->get<CTransform>().pos;
        const auto &b_box_half_size = b->get<CBoundingBox>().half_size;
        const auto &b_current_pos = b->get<CTransform>().pos;

        const auto delta = absolute_vec(a_current_pos - b_current_pos);
        const float ox = a_box_half_size.x + b_box_half_size.x - delta.x;
        const float oy = a_box_half_size.y + b_box_half_size.y - delta.y;

        return {ox, oy};
    }

    /**
     * @brief Get the previous overlap between two entities bounding boxes. Entities collide iff ox > 0 && oy > 0
     * 
     * @param a First entity
     * @param b Second entity
     */
    [[nodiscard]]
    inline sf::Vector2f get_previous_overlap(const std::shared_ptr<Entity> a, const std::shared_ptr<Entity> b)
    {
        // assert(a->has<CBoundingBox>() && a->has<CTransform>() && b->has<CBoundingBox>() && b->has<CTransform>());
        const auto &a_box_half_size = a->get<CBoundingBox>().half_size;
        const auto &a_previous_pos = a->get<CTransform>().previous_pos;
        const auto &b_box_half_size = b->get<CBoundingBox>().half_size;
        const auto &b_previous_pos = b->get<CTransform>().previous_pos;

        const auto delta = absolute_vec(a_previous_pos - b_previous_pos);
        const float ox = a_box_half_size.x + b_box_half_size.x - delta.x;
        const float oy = a_box_half_size.y + b_box_half_size.y - delta.y;

        return {ox, oy};
    }

};
