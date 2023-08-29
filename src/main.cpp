#include <game.hpp>
#include <iostream>


int main()
{
    auto result = Tetris::Game::instance()->init().start();
    Tetris::Game::destroy_all();
    return result;
}
