#pragma once

#include "scene.hpp"
#include "misc.hpp"

class SceneMenu : public Scene
{
public:
    explicit SceneMenu(GameEngine *game = nullptr);
    void system_render() noexcept override;

private:
    std::string m_title{};
    sf::Font m_menu_font{};
    std::optional<sf::Text> m_menu_text{};
    std::vector<sf::Text> m_menu_items{};
    std::vector<std::string> m_menu_strings{};
    std::vector<std::string> m_level_paths{};
    size_t m_selected_menu_index{};

    void init();
    void update() noexcept override;
    void system_do_action(const Action &action) noexcept override;
    void on_end() noexcept override;
};
