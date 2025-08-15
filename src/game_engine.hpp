#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include "config_parser.hpp"
#include "asset_manager.hpp"

class Scene;

namespace sf
{
    class View;
}

using SceneMap = std::unordered_map<std::string, std::shared_ptr<Scene>>;

/**
 * @brief Main application class, runs the game.
 * 
 * GameEngine manages the game loop, scenes, assets, config and window.
 * It serves as the entry point and controller of the application.
 * 
 * Usage:
 * 
 * - run(): Start the main game loop
 * 
 * - update(): Update the game logic
 * 
 * - quit(): Stop the game
 * 
 * - change_scene(name, scene, end_current): Switch between scenes
 * 
 * The class provides access to:
 * 
 * - AssetManager for textures, fonts, sounds, musics and animations
 * - Window and view for rendering
 * - Config data for window, levels, etc.
 * 
 * @note Non copyable, non movable
 * @note Should be constructed from a TOML config file
 * @note Holds GameSettings for volume control
 * @note Manages scene lifecycle and user inputs
 */
class GameEngine
{
public:
    /* Options */
    struct GameSettings
    {
        float m_music_volume{10.0f};
        float m_sound_volume{10.0f};
    };

    GameSettings settings{};

public:
    /**
     * @brief Default constructor
     */
    explicit GameEngine() noexcept = default;

    /**
     * @brief Custom destructor
     */
    ~GameEngine();

    /* Delete copy and move */
    GameEngine(const GameEngine &) noexcept = delete;
    GameEngine &operator=(const GameEngine &) noexcept = delete;
    GameEngine(GameEngine &&) noexcept = delete;
    GameEngine &operator=(GameEngine &&) noexcept = delete;

    /**
     * @brief Create a GameEngine from a TOML config file
     *
     * @param config_file Path to the config file
     */
    explicit GameEngine(const std::string &config_file);

    /**
     * @brief Update the game each frame
     */
    void update() noexcept;

    /**
     * @brief Run the game
     */
    void run() noexcept;

    /**
     * Quit the game
     */
    void quit() noexcept;

    /**
     * @brief Change current scene to the given scene
     *
     * @param name Scene name
     * @param scene Pointer to the next scene
     * @param end_current True to end the current scene
     */
    void change_scene(const std::string &name, std::shared_ptr<Scene> scene, bool end_current = false) noexcept;

    /**
     * @brief Return the asset manager
     */
    [[nodiscard]]
    const AssetManager &get_assets() const noexcept;

    /**
     * @brief Return the asset manager
     *
     */
    [[nodiscard]]
    AssetManager &get_assets() noexcept;

    /**
     * @brief Return the window
     */
    [[nodiscard]]
    const sf::RenderWindow &get_window() const noexcept;

    /**
     * @brief Return the window
     */
    [[nodiscard]]
    sf::RenderWindow &get_window() noexcept;

    /**
     * @brief Return the window configuration (size, title etc.)
     */
    [[nodiscard]]
    const WindowConfig &get_window_config() const noexcept;

    /**
     * @brief Return player info
     */
    [[nodiscard]]
    const PlayerConfig &get_player_config() const noexcept;

    /**
     * @brief Return bullet info
     */
    [[nodiscard]]
    const BulletConfig &get_bullet_config() const noexcept;

    /**
     * @brief Return levels info (name, data file)
     */
    [[nodiscard]]
    const std::vector<LevelConfig> &get_level_config() const noexcept;

    /**
     * @brief Return fonts info
     */
    [[nodiscard]]
    const std::vector<FontConfig> &get_font_config() const noexcept;

    /**
     * @brief Return textures info
     */
    [[nodiscard]]
    const std::vector<TextureConfig> &get_texture_config() const noexcept;

    /**
     * @brief Return animations info
     */
    [[nodiscard]]
    const std::vector<AnimationConfig> &get_animation_config() const noexcept;

    /**
     * @brief Return current window's view
     */
    [[nodiscard]]
    const sf::View &get_view() const noexcept;

    /**
     * @brief Check if game is running
     */
    [[nodiscard]]
    bool is_running() const noexcept;

private:
    /**
     * @brief Load assets, scenes etc.
     */
    void init();

    /**
     * @brief Handle user inputs
     */
    void system_user_input() noexcept;

    /**
     * @brief Return current scene
     */
    [[nodiscard]]
    std::shared_ptr<Scene> get_current_scene() const noexcept;

private:
    sf::RenderWindow m_window{};
    SceneMap m_scenes{};
    AssetManager m_assets{};
    std::string m_current_scene{"NONE"};
    unsigned m_simulation_speed{1};
    bool m_running{true};
    ConfigParser m_config{};
    sf::Clock m_imgui_clock{};
};
