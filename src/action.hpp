#pragma once

#include <string>

struct Action
{
    const std::string name{"NONE"};
    const std::string type{"NONE"};

    explicit Action() noexcept = default;

    /**
     * @brief Create an action
     * 
     * @param name Action's name
     * @param type Action's type
     */
    explicit Action(std::string name, std::string type) noexcept
        : name(std::move(name)), type(std::move(type))
    {
    }
};
