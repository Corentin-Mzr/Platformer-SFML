#include <iostream>
#include "game_engine.hpp"

int main()
{
    std::cout << "Hello Game !\n";

    try
    {
        GameEngine game("../resources/config.toml");
        game.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    std::cout << "Goodbye Game !\n";

    return 0;
}