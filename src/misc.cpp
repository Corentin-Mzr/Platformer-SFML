#include "misc.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <format>


sf::Color array_to_color(const std::array<uint8_t, 4> &array) noexcept
{
    return sf::Color(array[0], array[1], array[2], array[3]);
}

void print_color(const sf::Color &color) noexcept
{
    std::string str = std::format("Color(r={}, g={}, b={})\n", color.r, color.g, color.b);
    std::cout << str;
}

std::string float_to_string(float value, int precision) noexcept
{
    std::ostringstream out;
    out << std::fixed << std::setprecision(precision) << value;
    return out.str();
}

sf::Vector2f absolute_vec(const sf::Vector2f &v)
{
    return {std::abs(v.x), std::abs(v.y)};
}