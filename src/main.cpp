#include <game.hpp>
#include <iostream>
#include <unordered_map>

int main()
{
    auto result = Tetris::Game::instance()->start();
    Tetris::Game::destroy_all();
    return result;
}
