#include "game_engine.hpp"
#include "config_parser.hpp"
#include "scene.hpp"
#include "scene_menu.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

GameEngine::GameEngine(const std::string &config_file) : m_config(config_file)
{
    init();
}

void GameEngine::init()
{
    /* Load assets */
    for (const auto &font : m_config.get_font_config())
    {
        std::cout << "Adding font " + font.name + "\n";
        m_assets.add_font(font.name, font.path);
    }

    for (const auto &texture : m_config.get_texture_config())
    {
        std::cout << "Adding texture " + texture.name + "\n";
        m_assets.add_texture(texture.name, texture.path);
    }

    for (const auto &animation : m_config.get_animation_config())
    {
        std::cout << "Adding animation " + animation.name + "\n";
        Animation anim(animation.name, m_assets.get_texture(animation.texture), animation.frames, animation.speed);
        m_assets.add_animation(animation.name, anim);
    }

    /* Window config */
    const sf::Vector2u sizes{m_config.get_window_config().width, m_config.get_window_config().height};
    const sf::Vector2f sizes_f{static_cast<sf::Vector2f>(sizes)};
    m_window.create(sf::VideoMode(sizes), m_config.get_window_config().title);
    m_window.setMinimumSize(sizes);
    m_window.setMaximumSize(sizes);
    m_window.setFramerateLimit(m_config.get_window_config().framerate);
    m_window.setView(sf::View{{0.0f, 0.0f}, sizes_f});

    /* Setup scene */
    change_scene("MENU", std::make_shared<SceneMenu>(this));
}

void GameEngine::run() noexcept
{
    while (is_running())
    {
        system_user_input();
        update();
        m_window.display();
    }
}

void GameEngine::quit() noexcept
{
    m_running = false;
    m_window.close();
}

void GameEngine::change_scene(const std::string &name, std::shared_ptr<Scene> scene, bool end_current) noexcept
{
    std::cout << "Changing from " + m_current_scene + " to " + name + "\n";

    if (m_scenes.contains(name) && end_current)
    {
        m_scenes.erase(m_current_scene);
    }

    m_current_scene = name;
    m_scenes[name] = std::move(scene);
}

void GameEngine::update() noexcept
{
    auto scene{get_current_scene()};
    if (scene != nullptr) [[likely]]
    {
        scene->update();
    }
}

void GameEngine::system_user_input() noexcept
{
    while (const std::optional event = m_window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            quit();
        }

        auto scene{get_current_scene()};
        if (scene == nullptr) [[unlikely]]
        {
            continue;
        }

        if (const auto *key_pressed = event->getIf<sf::Event::KeyPressed>())
        {
            if (scene->get_action_map().find(key_pressed->scancode) == scene->get_action_map().end())
            {
                continue;
            }

            const Action action(scene->get_action_map().at(key_pressed->scancode), "START");
            scene->system_do_action(action);
        }

        if (const auto *key_released = event->getIf<sf::Event::KeyReleased>())
        {
            if (scene->get_action_map().find(key_released->scancode) == scene->get_action_map().end())
            {
                continue;
            }

            const Action action(scene->get_action_map().at(key_released->scancode), "END");
            scene->system_do_action(action);
        }
    }
}

const AssetManager &GameEngine::get_assets() const noexcept
{
    return m_assets;
}

const sf::RenderWindow &GameEngine::get_window() const noexcept
{
    return m_window;
}

sf::RenderWindow &GameEngine::get_window() noexcept
{
    return m_window;
}

std::shared_ptr<Scene> GameEngine::get_current_scene() const noexcept
{
    auto it{m_scenes.find(m_current_scene)};
    return it != m_scenes.end() ? it->second : nullptr;
}

const WindowConfig &GameEngine::get_window_config() const noexcept
{
    return m_config.get_window_config();
}

const PlayerConfig &GameEngine::get_player_config() const noexcept
{
    return m_config.get_player_config();
}

const BulletConfig &GameEngine::get_bullet_config() const noexcept
{
    return m_config.get_bullet_config();
}

const std::vector<LevelConfig> &GameEngine::get_level_config() const noexcept
{
    return m_config.get_level_config();
}

const std::vector<FontConfig> &GameEngine::get_font_config() const noexcept
{
    return m_config.get_font_config();
}

const std::vector<TextureConfig> &GameEngine::get_texture_config() const noexcept
{
    return m_config.get_texture_config();
}

const std::vector<AnimationConfig> &GameEngine::get_animation_config() const noexcept
{
    return m_config.get_animation_config();
}

const sf::View &GameEngine::get_view() const noexcept
{
    return m_window.getView();
}

bool GameEngine::is_running() const noexcept
{
    return m_running && m_window.isOpen();
}