#pragma once

#include <unordered_map>
#include <memory>
#include "entity_manager.hpp"
#include "action.hpp"

class GameEngine;

using Keycode = sf::Keyboard::Scancode;
using ActionMap = std::unordered_map<Keycode, std::string>;

class Scene
{

public:
    virtual ~Scene() noexcept = default;
    explicit Scene() noexcept = default;

    /**
     * @brief Create a scene
     * 
     * @param game Pointer to the GameEngine
     */
    explicit Scene(GameEngine *game) noexcept;

    /**
     * @brief Update the scene
     */
    virtual void update() noexcept = 0;

    /**
     * @brief Handle an action
     */
    virtual void system_do_action(const Action &action) noexcept = 0;

    /**
     * @brief Render the scene
     */
    virtual void system_render() noexcept = 0;

    /**
     * @brief Do the given action
     */
    virtual void do_action(const Action &action) noexcept;

    /**
     * @brief Simulate the scene over multiple frames
     */
    void simulate(size_t frames) noexcept;

    /**
     * @brief Register an action
     */
    void register_action(const Keycode &input_key, const std::string &action_name) noexcept;

    /**
     * @brief Draw a line on the screen
     */
    void draw_line(const sf::Vector2f &p1, const sf::Vector2f &p2) noexcept;

    /**
     * @brief Return window width
     */
    [[nodiscard]]
    unsigned get_width() const noexcept;

    /**
     * @brief Return window height
     */
    [[nodiscard]]
    unsigned get_height() const noexcept;

    /**
     * @brief Return current frame
     */
    [[nodiscard]]
    size_t get_current_frame() const noexcept;

    /**
     * @brief Check if scene has ended
     */
    [[nodiscard]]
    bool has_ended() const noexcept;

    /**
     * @brief Return the scene action map
     */
    [[nodiscard]]
    const ActionMap &get_action_map() const noexcept;

protected:
    /**
     * @brief Handle the scene exit
     */
    virtual void on_end() noexcept = 0;

    /**
     * @brief Pause or unpause the scene
     */
    void set_paused(bool paused) noexcept;

protected:
    GameEngine *m_game{};
    EntityManager m_entities{};
    ActionMap m_action_map{};
    size_t m_current_frame{};
    bool m_paused{false};
    bool m_has_ended{false};
};
