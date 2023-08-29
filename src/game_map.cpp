#include <game_map.hpp>


namespace Tetris
{
    Block*& GameMap::slot_at(const Position& pos)
    {
        if(pos.x < 10 && pos.y < 20 && pos.x >= 0 && pos.y >= 0)
        {
            return map[pos.y][pos.x];
        }

        throw std::runtime_error("Index out of range!");
    }

}
