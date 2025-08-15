#pragma once

#include <string>
#include <array>
#include <toml.hpp>

struct WindowConfig
{
    unsigned width{};
    unsigned height{};
    std::string title{};
    unsigned framerate{};
    std::array<uint8_t, 4> color{0, 0, 0, 255};
};
TOML11_DEFINE_CONVERSION_NON_INTRUSIVE(WindowConfig, width, height, title, framerate, color)

struct PlayerConfig
{
    float x{};
    float y{};
    float speed{};
    float jump{};
    float gravity{};
};
TOML11_DEFINE_CONVERSION_NON_INTRUSIVE(PlayerConfig, x, y, speed, jump, gravity)

struct BulletConfig
{
    float speed{};
    float radius{};
    unsigned lifespan{};
};
TOML11_DEFINE_CONVERSION_NON_INTRUSIVE(BulletConfig, speed, radius, lifespan)

struct LevelConfig
{
    std::string name{};
    std::string path{};
};
TOML11_DEFINE_CONVERSION_NON_INTRUSIVE(LevelConfig, name, path)

struct FontConfig
{
    std::string name{};
    std::string path{};
};
TOML11_DEFINE_CONVERSION_NON_INTRUSIVE(FontConfig, name, path)

struct TextureConfig
{
    std::string name{};
    std::string path{};
};
TOML11_DEFINE_CONVERSION_NON_INTRUSIVE(TextureConfig, name, path)

struct AnimationConfig
{
    std::string name{};
    std::string texture{};
    int frames{};
    int speed{};
};
TOML11_DEFINE_CONVERSION_NON_INTRUSIVE(AnimationConfig, name, texture, frames, speed)

struct SoundConfig
{
    std::string name{};
    std::string path{};
};
TOML11_DEFINE_CONVERSION_NON_INTRUSIVE(SoundConfig, name, path)

struct MusicConfig
{
    std::string name{};
    std::string path{};
};
TOML11_DEFINE_CONVERSION_NON_INTRUSIVE(MusicConfig, name, path)
