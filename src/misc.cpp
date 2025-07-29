#include "misc.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>


sf::Color array_to_color(const std::array<uint8_t, 4> &array) noexcept
{
    return sf::Color(array[0], array[1], array[2], array[3]);
}

void print_color(const sf::Color &color) noexcept
{
    std::string str = "Color(r=" + std::to_string(color.r) + ", g=" + std::to_string(color.g) + ", b=" + std::to_string(color.b) + ", a=" + std::to_string(color.a) + ")\n";
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