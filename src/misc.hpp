#pragma once

#include <array>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/**
 * @brief Convert an uint8_t array to sf::Color
 * 
 * @param array Array to convert
 */
[[nodiscard]]
sf::Color array_to_color(const std::array<uint8_t, 4> &array) noexcept;

/**
 * @brief Show color on the standard output
 * 
 * @param color Color to print
 */
void print_color(const sf::Color &color) noexcept;

/**
 * @brief Convert a float to string
 * 
 * @param value Float value
 * @param precision Number of decimal digits
 */
[[nodiscard]]
std::string float_to_string(float value, int precision) noexcept;

/**
 * @brief Absolute value of each element of a vector
 * 
 * @param v Vector
 */
[[nodiscard]]
sf::Vector2f absolute_vec(const sf::Vector2f &v);