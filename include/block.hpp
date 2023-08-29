#pragma once
#include <SFML/Graphics.hpp>


namespace Tetris
{
    using Position = sf::Vector2i;

    class Tetramino;

    enum class MoveDir
    {
        Right,
        Left,
        Down
    };

    class Block
    {
        Tetramino* m_tetramino;
        sf::Sprite* m_sprite;
        int m_index = 0;

    private:
        Block(Tetramino* tetramino, sf::Sprite* sp, int index = 0);
        Position m_position;
        bool m_is_updated = false;

        void recursive_move(unsigned int blocks, MoveDir dir = MoveDir::Down);

    public:
        int index() const;
        static constexpr std::size_t block_size = 40;

        const Position& position() const;
        void position(const Position& pos);

        Tetramino* tetramino() const;
        sf::Sprite* sprite() const;

        ~Block();

        friend class Tetramino;
    };
}// namespace Tetris
