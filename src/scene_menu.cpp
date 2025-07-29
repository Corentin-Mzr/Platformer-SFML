#include "scene_menu.hpp"
#include "game_engine.hpp"
#include "scene_play.hpp"

SceneMenu::SceneMenu(GameEngine *game) : Scene(game), m_menu_text(m_menu_font)
{
    init();
}

void SceneMenu::init()
{
    /* Allowed key inputs */
    register_action(sf::Keyboard::Key::Escape, "QUIT");
    register_action(sf::Keyboard::Key::Down, "DOWN");
    register_action(sf::Keyboard::Key::Up, "UP");
    register_action(sf::Keyboard::Key::Enter, "PLAY");
    register_action(sf::Keyboard::Key::R, "REFRESH"); 

    /* Font load */
    m_menu_font = m_game->get_assets().get_font("Consolas");

    /* Current view data */
    const sf::Vector2f &view_half_size{0.5f * m_game->get_view().getSize()};
    const sf::Vector2f &view_pos{m_game->get_view().getCenter()};

    /* Title */
    const sf::Vector2f title_offset{144.0f, 48.0f};
    const sf::Vector2f title_position{view_pos - view_half_size + title_offset};
    m_title = "MegaMario";
    m_menu_text->setFont(m_menu_font);
    m_menu_text->setString(m_title);
    m_menu_text->setCharacterSize(48);
    m_menu_text->setOutlineThickness(4.0f);
    m_menu_text->setFillColor(sf::Color::White);
    m_menu_text->setOutlineColor(sf::Color::Black);
    m_menu_text->setOrigin(0.5f * m_menu_text->getLocalBounds().size);
    m_menu_text->setPosition(title_position);

    /* Level selection */
    const auto level_config{m_game->get_level_config()};
    const sf::Vector2f level_offset{92.0f, 148.0f};
    const float spacing{108.0f};
    sf::Vector2f level_position{0.0f, 0.0f};
    sf::Text level_item{m_menu_font, "", 36};

    for (const auto &level : level_config)
    {
        m_menu_strings.emplace_back(level.name);
        m_level_paths.emplace_back(level.path);
    }
    m_menu_strings.emplace_back("Quit");

    for (size_t i = 0; i < m_menu_strings.size(); ++i)
    {
        level_position = view_pos - view_half_size + level_offset;
        level_position.y += spacing * i;

        level_item.setString(m_menu_strings[i]);
        level_item.setOutlineThickness(2.0f);
        level_item.setFillColor(sf::Color::Black);
        level_item.setOutlineColor(sf::Color::Black);
        level_item.setOrigin(0.5f * level_item.getLocalBounds().size);
        level_item.setPosition(level_position);
        m_menu_items.push_back(level_item);
    }
}

void SceneMenu::update() noexcept
{
    // m_entities.update();
    system_render();
}

void SceneMenu::system_render() noexcept
{
    const sf::Color background_color{array_to_color(m_game->get_window_config().color)};
    m_game->get_window().clear(background_color);

    /* Game title */
    m_game->get_window().draw(m_menu_text.value());

    /* Levels - Menu items */
    for (size_t i = 0; i < m_menu_items.size(); ++i)
    {
        /* Change color based on selected level */
        if (i != m_selected_menu_index)
            m_menu_items[i].setFillColor(sf::Color::Black);
        else
            m_menu_items[i].setFillColor(sf::Color::White);

        m_game->get_window().draw(m_menu_items[i]);
    }

    /* Draw help  */
    const sf::Vector2f view_half_size{0.5f * m_game->get_view().getSize()};
    const sf::Vector2f view_pos{m_game->get_view().getCenter()};
    sf::Text help_text{m_menu_font, "MOVE: ARROWS | PLAY: ENTER | QUIT/MENU: ESCAPE", 24};
    help_text.setFillColor(sf::Color::White);
    help_text.setPosition({view_pos.x - view_half_size.x + 24.0f, view_pos.y + 0.75f * view_half_size.y});
    m_game->get_window().draw(help_text);

}

void SceneMenu::on_end() noexcept
{
    m_game->quit();
}

void SceneMenu::system_do_action(const Action &action) noexcept
{
    /* Key pressed */
    if (action.type == "START")
    {
        if (action.name == "QUIT")
            on_end();

        else if (action.name == "DOWN")
            m_selected_menu_index = m_selected_menu_index == m_menu_strings.size() - 1 ? m_menu_strings.size() - 1 : m_selected_menu_index + 1;

        else if (action.name == "UP")
            m_selected_menu_index = m_selected_menu_index == 0 ? 0 : m_selected_menu_index - 1;

        else if (action.name == "PLAY")
        {
            if (m_selected_menu_index != m_menu_strings.size() - 1)
                m_game->change_scene("PLAY", std::make_shared<ScenePlay>(m_game, m_level_paths[m_selected_menu_index]));
            else
                on_end();
        }
        else if (action.name == "REFRESH")
        {
            m_game->change_scene("MENU", std::make_shared<SceneMenu>(m_game));
        }
            
    }

    /* Key released */
    else if (action.type == "END")
    {
    }
}