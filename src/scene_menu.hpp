#pragma once

#include "scene.hpp"
#include "misc.hpp"

class SceneMenu : public Scene
{
public:
    /**
     * @brief Create a Menu Scene
     *
     * @param game Pointer to the GameEngine
     */
    explicit SceneMenu(GameEngine *game = nullptr);

    /**
     * @brief Render the scene
     */
    void system_render() noexcept override;

private:
    /**
     * @brief Initialize the scene (store actions, load fonts, make level selection etc.)
     */
    void init();

    /**
     * @brief Update the scene
     */
    void update() noexcept override;

    /**
     * @brief Handle an action
     */
    void system_do_action(const Action &action) noexcept override;

    /**
     * @brief Handle the scene exit
     */
    void on_end() noexcept override;

private:
    std::string m_title{};
    sf::Font m_menu_font{};
    std::optional<sf::Text> m_menu_text{};
    std::vector<sf::Text> m_menu_items{};
    std::vector<std::string> m_menu_strings{};
    std::vector<std::string> m_level_paths{};
    size_t m_selected_menu_index{};
};
