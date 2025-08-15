#pragma once

#include "scene.hpp"
#include "config_structs.hpp"
#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

/**
 * @brief Represents the main gameplay scene.
 */
class ScenePlay : public Scene
{
public:
    /**
     * @brief Create a Play Scene
     *
     * @param game Pointer to the GameEngine
     * @param level_path Path to the level file
     */
    explicit ScenePlay(GameEngine *game, const std::string &level_path);

    /**
     * @brief Update the scene
     */
    void update() noexcept override;

private:
    /**
     * @brief Initialize the scene using the given level data file
     *
     * @param path Path to the level file
     */
    void init(const std::string &path);

    /**
     * @brief Convert grid coord to pixel coord indicating where the center position of the entity should be
     *
     * @param grid_x Grid position on the x axis
     * @param grid_y Grid position on the y axis
     * @param entity Entity you want to find the center position of
     */
    [[nodiscard]]
    sf::Vector2f grid_to_mid_pixel(float grid_x, float grid_y, std::shared_ptr<Entity> entity) noexcept;

    /**
     * @brief Load a level using a data file
     *
     * @param path Path to the level file
     */
    void load_level(const std::string &path);

    /**
     * @brief Add player to the scene
     */
    void spawn_player() noexcept;

    /**
     * @brief Add a bullet to the scene
     *
     * @param entity Bullet will spawn from this entity's position
     */
    void spawn_bullet(std::shared_ptr<Entity> entity) noexcept;

    /**
     * @brief Handle player inputs
     */
    void system_movement() noexcept;

    /**
     * @brief Handle entities lifespan
     */
    void system_lifespan() noexcept;

    /**
     * @brief Handle collision between entities
     */
    void system_collision() noexcept;

    /**
     * @brief Handle animations of entities
     */
    void system_animation() noexcept;

    /**
     * @brief Handle ImGui
     */
    void system_gui();

    /**
     * @brief Handle sounds
     */
    void system_sound() noexcept;

    /**
     * @brief Render the scene
     */
    void system_render() noexcept override;

    /**
     * @brief Handle an action
     */
    void system_do_action(const Action &action) noexcept override;

    /**
     * @brief Handle the scene exit
     */
    void on_end() noexcept override;

    /**
     * @brief Change the player's state
     *
     * @param state Player's state component
     * @param new_state New player's state
     */
    void change_player_state_to(CState &state, const std::string &new_state) noexcept;

    /**
     * @brief Change the tile animation to an explosion, and add a timer before removing it. 
     * Use this when a brick is destroyed via a bullet
     * 
     * @param tile Brick tile to update
     */
    void spawn_explosion(std::shared_ptr<Entity> tile) noexcept;

    /**
     * @brief Change the tile animation to debris, and add a timer before removing it. 
     * Use this when a brick is destroyed via the player collision
     * 
     * @param tile Brick tile to update
     */
    void spawn_debris(std::shared_ptr<Entity> tile) noexcept;

    /**
     * @brief Spawn a spinning coin over a question mark tile
     * 
     * @param tile Question mark tile
     */
    void spawn_coin(std::shared_ptr<Entity> tile) noexcept;

private:
    std::shared_ptr<Entity> m_player{};
    std::string m_level_path{};
    const sf::Vector2u m_grid_size{64, 64};
    sf::Font m_font{};
    std::optional<sf::Text> m_grid_text{};

    /* Toggle some draw things */
    bool m_draw_textures{true};
    bool m_draw_collision{false};
    bool m_draw_grid{false};

    /* Toggle systems */
    bool m_movement{true};
    bool m_lifespan{true};
    bool m_collision{true};
    bool m_animation{true};
    bool m_render{true};
    bool m_action{true};
    bool m_sound{true};

    /* Count bullets to know when player can shoot */
    size_t m_bullet_count{};
};
