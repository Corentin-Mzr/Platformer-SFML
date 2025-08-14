#pragma once

#include "scene.hpp"
#include "misc.hpp"
#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

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
     * @brief Handle sounds
     */
    void system_sound();

    /**
     * @brief Handle changing scenes
     */
    void system_scene();

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
    size_t m_selected_menu_index_prev{};

    bool m_change_scene_next_frame{false};
    std::string m_next_scene_name{};
    std::shared_ptr<Scene> m_next_scene{};
    std::optional<sf::Sound> m_selection_sound{};
    std::optional<sf::Sound> m_confirm_sound{};
    bool m_menu_music_play{false};
};
