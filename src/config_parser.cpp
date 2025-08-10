#include "config_parser.hpp"
#include <iostream>

////////////////////////////////////////// HELPER FUNCTIONS //////////////////////////////////////////

template <typename T>
[[nodiscard]]
static T parse_section(const toml::value &data, const std::string &section_name)
{
    if (data.contains(section_name))
    {
        return toml::find<T>(data, section_name);
    }
    std::cerr << std::format("Could not find {} section in the config file\n", section_name);
    return T{};
}

template <typename T>
[[nodiscard]]
static std::vector<T> parse_section_with_subsections(const toml::value &data, const std::string &section_name)
{
    const auto v = parse_section<toml::table>(data, section_name);
    std::vector<T> subdata{};
    subdata.reserve(v.size());
    for (const auto &[key, value] : v)
    {
        subdata.push_back(toml::get<T>(value));
    }
    return subdata;
}

[[nodiscard]]
static WindowConfig parse_window(const toml::value &data)
{
    return parse_section<WindowConfig>(data, "window");
}

[[nodiscard]]
static PlayerConfig parse_player(const toml::value &data)
{
    return parse_section<PlayerConfig>(data, "player");
}

[[nodiscard]]
static BulletConfig parse_bullet(const toml::value &data)
{
    return parse_section<BulletConfig>(data, "bullet");
}

[[nodiscard]]
static std::vector<LevelConfig> parse_level(const toml::value &data)
{
    auto levels{parse_section_with_subsections<LevelConfig>(data, "level")};
    std::reverse(levels.begin(), levels.end());
    return levels;
}

[[nodiscard]]
static std::vector<FontConfig> parse_font(const toml::value &data)
{
    auto fonts{parse_section_with_subsections<FontConfig>(data, "font")};
    return fonts;
}

[[nodiscard]]
static std::vector<TextureConfig> parse_texture(const toml::value &data)
{
    auto textures{parse_section_with_subsections<TextureConfig>(data, "texture")};
    return textures;
}

[[nodiscard]]
static std::vector<AnimationConfig> parse_animation(const toml::value &data)
{
    auto animations{parse_section_with_subsections<AnimationConfig>(data, "animation")};
    return animations;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

ConfigParser::ConfigParser(const std::filesystem::path &config_filepath) : m_filepath(config_filepath)
{
    const auto data = toml::parse(m_filepath);
    m_window_config = parse_window(data);
    m_player_config = parse_player(data);
    m_bullet_config = parse_bullet(data);
    m_level_configs = parse_level(data);
    m_font_configs = parse_font(data);
    m_texture_configs = parse_texture(data);
    m_animation_configs = parse_animation(data);
}

const WindowConfig &ConfigParser::get_window_config() const noexcept
{
    return m_window_config;
}

const PlayerConfig &ConfigParser::get_player_config() const noexcept
{
    return m_player_config;
}

const BulletConfig &ConfigParser::get_bullet_config() const noexcept
{
    return m_bullet_config;
}

const std::vector<LevelConfig> &ConfigParser::get_level_config() const noexcept
{
    return m_level_configs;
}

const std::vector<FontConfig> &ConfigParser::get_font_config() const noexcept
{
    return m_font_configs;
}

const std::vector<TextureConfig> &ConfigParser::get_texture_config() const noexcept
{
    return m_texture_configs;
}

const std::vector<AnimationConfig> &ConfigParser::get_animation_config() const noexcept
{
    return m_animation_configs;
}
