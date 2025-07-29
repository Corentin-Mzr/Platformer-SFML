#pragma once

#include <string>

struct Action
{
    const std::string name{"NONE"};
    const std::string type{"NONE"};

    explicit Action() noexcept = default;
    explicit Action(std::string name, std::string type) noexcept
        : name(std::move(name)), type(std::move(type))
    {
    }
};
