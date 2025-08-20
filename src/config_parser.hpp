#pragma once

#include "config_structs.hpp"

/**
 * @brief Loads and stores game config data from a TOML file.
 * 
 * The ConfigParser reads a TOML config file and extracts structured data
 * such as window settings, bullet parameters, level infos and assets references.
 * 
 * Once loaded, the config values can be accessed through dedicated getters,
 * allowing other systems to retrieve settings without having to parse the file again.
 * 
 * @note This class provides read-only access to the config data, 
 * it does not support runtime modification or saving.
 * 
 */
class ConfigParser
{
public:
    /**
     * @brief Default constructor
     */
    explicit ConfigParser() noexcept = default;

    /**
     * @brief Store the config data from a TOML config file
     *
     * @param config_filepath Path to the config file
     */
    explicit ConfigParser(const std::filesystem::path &config_filepath);

    /**
     * @brief Return window configuration (size, title etc.)
     */
    [[nodiscard]]
    const WindowConfig &get_window_config() const noexcept;

    // /**
    //  * @brief Return player stats (speed etc.)
    //  */
    // [[nodiscard]]
    // const PlayerConfig &get_player_config() const noexcept;

    /**
     * @brief Return bullets stats (speed, lifespan etc.)
     */
    [[nodiscard]]
    const BulletConfig &get_bullet_config() const noexcept;

    /**
     * @brief Return levels info (level name, data file)
     */
    [[nodiscard]]
    const std::vector<LevelConfig> &get_level_config() const noexcept;

    /**
     * @brief Return fonts info (font name, file)
     */
    [[nodiscard]]
    const std::vector<FontConfig> &get_font_config() const noexcept;

    /**
     * @brief Return textures info (texture name, file)
     */
    [[nodiscard]]
    const std::vector<TextureConfig> &get_texture_config() const noexcept;

    /**
     * @brief Return animation info (anim name, frames, speed)
     */
    [[nodiscard]]
    const std::vector<AnimationConfig> &get_animation_config() const noexcept;

    /**
     * @brief Return sounds info (sound name, file)
     */
    [[nodiscard]]
    const std::vector<SoundConfig> &get_sound_config() const noexcept;

    /**
     * @brief Return musics info (sound name, file)
     */
    [[nodiscard]]
    const std::vector<MusicConfig> &get_music_config() const noexcept;

private:
    std::filesystem::path m_filepath{};
    WindowConfig m_window_config{};
    // PlayerConfig m_player_config{};
    BulletConfig m_bullet_config{};
    std::vector<LevelConfig> m_level_configs{};
    std::vector<FontConfig> m_font_configs{};
    std::vector<TextureConfig> m_texture_configs{};
    std::vector<AnimationConfig> m_animation_configs{};
    std::vector<SoundConfig> m_sound_configs{};
    std::vector<MusicConfig> m_music_configs{};
};
