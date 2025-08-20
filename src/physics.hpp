#pragma once

#include <memory>
#include <limits>
#include <SFML/System/Vector2.hpp>
#include "entity.hpp"

namespace Physics
{
    /**
     * @brief Get the current overlap between two entities that have a BoundingBox component.
     * 
     * Uses the Axis-Aligned Bounding Box method (AABB).
     * 
     * Entities collide iff ox > 0 && oy > 0.
     * 
     * Return (ox, oy) if entities collide, else (0, 0).
     *
     * @param a First entity
     * @param b Second entity
     */
    [[nodiscard]]
    inline sf::Vector2f get_current_overlap(const std::shared_ptr<Entity> &a, const std::shared_ptr<Entity> &b) noexcept
    {
        if (!a->has<CBoundingBox>() || !b->has<CBoundingBox>() || !a->has<CTransform>() || !b->has<CTransform>())
        {
            return sf::Vector2f{0.0f, 0.0f};
        }

        const auto &a_box_half_size{a->get<CBoundingBox>().half_size};
        const auto &a_offset{a->get<CBoundingBox>().offset};
        const auto &a_current_pos{a->get<CTransform>().pos};
        const auto &b_box_half_size{b->get<CBoundingBox>().half_size};
        const auto &b_offset{b->get<CBoundingBox>().offset};
        const auto &b_current_pos{b->get<CTransform>().pos};

        const auto delta{absolute_vec((a_current_pos + a_offset) - (b_current_pos + b_offset))};
        const float ox{a_box_half_size.x + b_box_half_size.x - delta.x};
        const float oy{a_box_half_size.y + b_box_half_size.y - delta.y};


        return (ox > 0.0f && oy > 0.0f) ? sf::Vector2f{ox, oy} : sf::Vector2f{0.0f, 0.0f};
    }

    /**
     * @brief Get the previous overlap between two entities that have a BoundingBox component.
     * 
     * Entities collide iff ox > 0 && oy > 0.
     * 
     * Return (ox, oy) if entities collide, else (0, 0).
     *
     * @param a First entity
     * @param b Second entity
     */
    [[nodiscard]]
    inline sf::Vector2f get_previous_overlap(const std::shared_ptr<Entity> &a, const std::shared_ptr<Entity> &b) noexcept
    {
        if (!a->has<CBoundingBox>() || !b->has<CBoundingBox>() || !a->has<CTransform>() || !b->has<CTransform>())
        {
            return sf::Vector2f{0.0f, 0.0f};
        }

        const auto &a_box_half_size{a->get<CBoundingBox>().half_size};
        const auto &a_offset{a->get<CBoundingBox>().offset};
        const auto &a_previous_pos{a->get<CTransform>().previous_pos};
        const auto &b_box_half_size{b->get<CBoundingBox>().half_size};
        const auto &b_offset{b->get<CBoundingBox>().offset};
        const auto &b_previous_pos{b->get<CTransform>().previous_pos};

        const auto delta{absolute_vec((a_previous_pos + a_offset) - (b_previous_pos + b_offset))};
        const float ox{a_box_half_size.x + b_box_half_size.x - delta.x};
        const float oy{a_box_half_size.y + b_box_half_size.y - delta.y};

        return ox > 0.0f && oy > 0.0f ? sf::Vector2f{ox, oy} : sf::Vector2f{0.0f, 0.0f};
    }

    /**
     * @brief Get the current overlap between two entities that have a BoundingConvex component.
     * 
     * Uses the Separated-Axis-Theorem (SAT).
     *
     * Entities collide iff ox > 0 && oy > 0.
     * 
     * Return (ox, oy) if entities collide, else (0, 0).
     *
     * @param a First entity
     * @param b Second entity
     */
    [[nodiscard]]
    inline sf::Vector2f get_convex_current_overlap(const std::shared_ptr<Entity> &a, const std::shared_ptr<Entity> &b) noexcept
    {
        if (!a->has<CBoundingConvex>() || !b->has<CBoundingConvex>() || !a->has<CTransform>() || !b->has<CTransform>())
        {
            return sf::Vector2f{0.0f, 0.0f};
        }

        const auto &convex_a{a->get<CBoundingConvex>()};
        const auto &convex_b{b->get<CBoundingConvex>()};
        const auto &transform_a{a->get<CTransform>()};
        const auto &transform_b{b->get<CTransform>()};

        // Transform points to world space
        auto get_world_points = [](const CBoundingConvex &convex, const CTransform &transform)
        {
            std::vector<sf::Vector2f> world_points;
            world_points.reserve(convex.count);

            for (const auto &point : convex.points)
            {
                // Apply scale then position
                sf::Vector2f scaled_point{point * convex.scale};

                world_points.emplace_back(scaled_point + transform.pos);
            }
            return world_points;
        };

        auto points_a{get_world_points(convex_a, transform_a)};
        auto points_b{get_world_points(convex_b, transform_b)};

        // Get all unique normals (perpendicular to edges)
        auto get_normals = [](const std::vector<sf::Vector2f> &points)
        {
            std::vector<sf::Vector2f> normals{};
            normals.reserve(points.size());

            for (size_t i = 0; i < points.size(); ++i)
            {
                size_t next{(i + 1) % points.size()};

                // Edge vector
                sf::Vector2f edge{points[next] - points[i]};

                // Normal (perpendicular to edge)
                sf::Vector2f normal{-edge.y, edge.x};

                // Normalize
                if (normal.lengthSquared() > 0.0f)
                {
                    normals.push_back(normal.normalized());
                }
            }
            return normals;
        };

        // Project polygon onto axis
        auto project = [](const std::vector<sf::Vector2f> &points, const sf::Vector2f &axis)
        {
            float min_proj{points[0].dot(axis)};
            float max_proj{min_proj};

            for (size_t i = 1; i < points.size(); ++i)
            {
                float proj{points[i].dot(axis)};
                min_proj = std::min(min_proj, proj);
                max_proj = std::max(max_proj, proj);
            }

            return std::make_pair(min_proj, max_proj);
        };

        // Get all axes to test
        auto normals_a{get_normals(points_a)};
        auto normals_b{get_normals(points_b)};

        float min_overlap{std::numeric_limits<float>::max()};
        sf::Vector2f collision_axis{0.0f, 0.0f};

        // Test all axes
        auto test_axes = [&](const std::vector<sf::Vector2f> &axes)
        {
            for (const auto &axis : axes)
            {
                auto [min_a, max_a]{project(points_a, axis)};
                auto [min_b, max_b]{project(points_b, axis)};

                // Check for separation
                if (max_a < min_b || max_b < min_a)
                {
                    return false; // Separated on this axis
                }

                // Calculate overlap
                float overlap{std::min(max_a - min_b, max_b - min_a)};

                if (overlap < min_overlap)
                {
                    min_overlap = overlap;
                    collision_axis = axis;
                }
            }
            return true; // No separation found
        };

        // Test normals from both polygons
        if (!test_axes(normals_a) || !test_axes(normals_b))
        {
            return {0.0f, 0.0f}; // No collision
        }

        // Ensure the collision axis points from A to B
        sf::Vector2f center_a{0.0f, 0.0f};
        sf::Vector2f center_b{0.0f, 0.0f};

        for (const auto &point : points_a)
        {
            center_a += point;
        }
        center_a /= static_cast<float>(points_a.size());

        for (const auto &point : points_b)
        {
            center_b += point;
        }
        center_b /= static_cast<float>(points_b.size());

        sf::Vector2f center_diff{center_b - center_a};

        // If collision axis points in opposite direction, flip it
        float dot{collision_axis.dot(center_diff)};
        if (dot < 0.0f)
        {
            collision_axis.x = -collision_axis.x;
            collision_axis.y = -collision_axis.y;
        }

        // Return overlap vector
        return {
            collision_axis.x * min_overlap,
            collision_axis.y * min_overlap};
    }

    /**
     * @brief Get the current overlap between two entities, one has a BoundingConvex component, the other has a BoundingBox component.
     *
     * Uses the Separated-Axis-Theorem (SAT).
     *
     * Entities collide iff ox > 0 && oy > 0.
     * 
     * Return (ox, oy) if entities collide, else (0, 0).
     *
     * @param a First entity with the BoundingConvex component
     * @param b Second entity with the BoundingBox component
     */
    [[nodiscard]]
    inline sf::Vector2f get_current_overlap_between_convex_and_box(const std::shared_ptr<Entity> &a, const std::shared_ptr<Entity> &b) noexcept
    {
        /* Convert the box to a BoundingConvex component and remove it after computation */
        static const std::vector<sf::Vector2f> box_local{
            {sf::Vector2f{-0.5f, -0.5f}, sf::Vector2f{-0.5f, 0.5f}, sf::Vector2f{0.5f, 0.5f}, sf::Vector2f{0.5f, -0.5f}}};

        b->add<CBoundingConvex>(box_local, b->get<CBoundingBox>().size);
        auto result{get_convex_current_overlap(a, b)};
        b->remove<CBoundingConvex>();
        return result;
    }

};
