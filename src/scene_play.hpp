#pragma once

#include "scene.hpp"
#include "config_structs.hpp"

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
    sf::Vector2f grid_to_mid_pixel(float grid_x, float grid_y, std::shared_ptr<Entity> entity);

    /**
     * @brief Load a level using a data file
     *
     * @param path Path to the level file
     */
    void load_level(const std::string &path);

    /**
     * @brief Add player to the scene
     */
    void spawn_player();

    /**
     * @brief Add a bullet to the scene
     *
     * @param entity Bullet will spawn from this entity's position
     */
    void spawn_bullet(std::shared_ptr<Entity> entity);

    /**
     * @brief Handle player inputs
     */
    void system_movement();

    /**
     * @brief Handle entities lifespan
     */
    void system_lifespan();

    /**
     * @brief Handle collision between entities
     */
    void system_collision();

    /**
     * @brief Handle animations of entities
     */
    void system_animation();

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
    void spawn_explosion(std::shared_ptr<Entity> tile);

    /**
     * @brief Change the tile animation to debris, and add a timer before removing it. 
     * Use this when a brick is destroyed via the player collision
     * 
     * @param tile Brick tile to update
     */
    void spawn_debris(std::shared_ptr<Entity> tile);

    /**
     * @brief Spawn a spinning coin over a question mark tile
     * 
     * @param tile Question mark tile
     */
    void spawn_coin(std::shared_ptr<Entity> tile);

private:
    std::shared_ptr<Entity> m_player{};
    std::string m_level_path{};
    bool m_draw_textures{true};
    bool m_draw_collision{false};
    bool m_draw_grid{false};
    const sf::Vector2u m_grid_size{64, 64};
    sf::Font m_font{};
    std::optional<sf::Text> m_grid_text{};
};
