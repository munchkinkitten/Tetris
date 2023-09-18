#pragma once
#include <array>
#include <SFML/Graphics.hpp>
#include <block.hpp>

namespace Tetris
{
    using Row = std::array<Block*, 10>;
    using Map = std::array<Row, 20>;

    class GameMap
    {
    public:
        Map map;

        Block*& slot_at(const Position& pos);
        GameMap& clean();
    };
}
