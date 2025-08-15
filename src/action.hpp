#pragma once

#include <string>

/**
 * @brief Represents an action the user can do in the game depending on a key state
 * 
 * @note type is either START (key pressed) or END (key released)
 */
struct Action
{
    const std::string name{"NONE"};
    const std::string type{"NONE"};

    /**
     * @brief Default constructor
     */
    explicit Action() noexcept = default;

    /**
     * @brief Create an action
     * 
     * @param name Action's name
     * @param type Action's type (START when key pressed, END when key released)
     */
    explicit Action(std::string name, std::string type) noexcept
        : name(std::move(name)), type(std::move(type))
    {
    }
};
