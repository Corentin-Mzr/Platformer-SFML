#include "scene_menu.hpp"
#include "game_engine.hpp"
#include "scene_play.hpp"
#include <SFML/Audio/Sound.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

SceneMenu::SceneMenu(GameEngine *game) : Scene(game), m_menu_text(m_menu_font)
{
    init();
}

void SceneMenu::init()
{
    /* Allowed key inputs */
    register_action(Keycode::Escape, "QUIT");
    register_action(Keycode::Down, "DOWN");
    register_action(Keycode::Up, "UP");
    register_action(Keycode::Enter, "PLAY");
    register_action(Keycode::R, "REFRESH");

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

    /* Menu sounds */
    m_selection_sound.emplace(m_game->get_assets().get_sound("Selection"));
    m_selection_sound->setVolume(m_game->settings.m_sound_volume);

    m_confirm_sound.emplace(m_game->get_assets().get_sound("Confirm"));
    m_confirm_sound->setVolume(m_game->settings.m_sound_volume);

    /* Menu music */
    m_game->get_assets().get_music("Menu").setVolume(m_game->settings.m_sound_volume);
    m_game->get_assets().get_music("Menu").setLooping(true);
}

void SceneMenu::update() noexcept
{
    system_sound();
    system_scene();
    system_gui();
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

void SceneMenu::system_sound()
{
    auto &menu_music{m_game->get_assets().get_music("Menu")};

    menu_music.setVolume(m_game->settings.m_music_volume);
    m_selection_sound->setVolume(m_game->settings.m_sound_volume);
    m_confirm_sound->setVolume(m_game->settings.m_sound_volume);

    if (menu_music.getStatus() == sf::Music::Status::Stopped)
    {
        menu_music.play();
        m_menu_music_play = true;
    }

    if (m_selected_menu_index != m_selected_menu_index_prev)
    {
        m_selected_menu_index_prev = m_selected_menu_index;
        m_selection_sound->play();
    }

    if (m_change_scene_next_frame)
    {
        m_confirm_sound->play();
        // menu_music.stop();
    }
}

void SceneMenu::system_scene()
{
    if (m_change_scene_next_frame)
    {
        m_game->change_scene(m_next_scene_name, m_next_scene, m_end_current);
        m_change_scene_next_frame = false;
        m_end_current = false;
    }
}

void SceneMenu::system_gui()
{
    ImGui::Begin("Options");
    ImGui::SliderFloat("Music", &m_game->settings.m_music_volume, 0.0f, 100.0f, "%.0f");
    ImGui::SliderFloat("Sound effects", &m_game->settings.m_sound_volume, 0.0f, 100.0f, "%0.f");
    ImGui::End();
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
        {
            on_end();
        }

        else if (action.name == "DOWN")
        {
            m_selected_menu_index = m_selected_menu_index == m_menu_strings.size() - 1 ? m_menu_strings.size() - 1 : m_selected_menu_index + 1;
        }

        else if (action.name == "UP")
        {
            m_selected_menu_index = m_selected_menu_index == 0 ? 0 : m_selected_menu_index - 1;
        }

        else if (action.name == "PLAY")
        {
            if (m_selected_menu_index != m_menu_strings.size() - 1)
            {
                m_next_scene_name = "PLAY";
                m_next_scene = std::make_shared<ScenePlay>(m_game, m_level_paths[m_selected_menu_index]);
                m_change_scene_next_frame = true;
                m_end_current = true;
            }
            else
            {
                on_end();
            }
        }
        else if (action.name == "REFRESH")
        {
            m_next_scene_name = "MENU";
            m_next_scene = std::make_shared<SceneMenu>(m_game);
            m_change_scene_next_frame = true;
            m_end_current = true;
        }
    }

    /* Key released */
    else if (action.type == "END")
    {
    }
}
