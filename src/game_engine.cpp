#include "game_engine.hpp"
#include "action.hpp"
#include "scene.hpp"
#include "scene_menu.hpp"
#include <iostream>
#include <imgui.h>
#include <imgui-SFML.h>

GameEngine::GameEngine(const std::string &config_file) : m_config(config_file)
{
    init();
}

GameEngine::~GameEngine()
{
    ImGui::SFML::Shutdown();
}

void GameEngine::init()
{
    /* Load assets */
    for (const auto &font : m_config.get_font_config())
    {
        std::cout << std::format("Adding font {}\n", font.name);
        m_assets.add_font(font.name, font.path);
    }

    for (const auto &texture : m_config.get_texture_config())
    {
        std::cout << std::format("Adding texture {}\n", texture.name);
        m_assets.add_texture(texture.name, texture.path);
    }

    for (const auto &animation : m_config.get_animation_config())
    {
        std::cout << std::format("Adding animation {}\n", animation.name);
        Animation anim(animation.name, m_assets.get_texture(animation.texture), animation.frames, animation.speed);
        m_assets.add_animation(animation.name, anim);
    }

    for (const auto &sound: m_config.get_sound_config())
    {
        std::cout << std::format("Adding sound {}\n", sound.name);
        m_assets.add_sound(sound.name, sound.path);
    }

    for (const auto &music: m_config.get_music_config())
    {
        std::cout << std::format("Adding music {}\n", music.name);
        m_assets.add_music(music.name, music.path);
    }

    /* Window config */
    const sf::Vector2u sizes{m_config.get_window_config().width, m_config.get_window_config().height};
    const sf::Vector2f sizes_f{static_cast<sf::Vector2f>(sizes)};
    m_window.create(sf::VideoMode(sizes), m_config.get_window_config().title);
    m_window.setMinimumSize(sizes);
    m_window.setMaximumSize(sizes);
    m_window.setFramerateLimit(m_config.get_window_config().framerate);
    m_window.setView(sf::View{{0.0f, 0.0f}, sizes_f});

    /* Init ImGui */
    if (!ImGui::SFML::Init(m_window))
    {
        throw std::runtime_error("Could not initialize ImGui");
    }

    /* Setup scene */
    change_scene("MENU", std::make_shared<SceneMenu>(this), true);
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
    std::cout << std::format("Changing from {} to {}\n", m_current_scene, name);

    if (m_scenes.contains(name) && end_current)
    {
        m_scenes.erase(m_current_scene);
    }
    m_scenes[name] = std::move(scene);
    // m_scenes.try_emplace(name, std::move(scene));
    m_current_scene = name;
}

void GameEngine::update() noexcept
{
    auto scene{get_current_scene()};
    if (scene != nullptr) [[likely]]
    {
        ImGui::SFML::Update(m_window, m_imgui_clock.restart());
        scene->update();
        ImGui::SFML::Render(m_window);
    }
}

void GameEngine::system_user_input() noexcept
{
    while (const std::optional event = m_window.pollEvent())
    {
        ImGui::SFML::ProcessEvent(m_window, *event);

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

AssetManager &GameEngine::get_assets() noexcept
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