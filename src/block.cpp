#include <block.hpp>
#include <game.hpp>
#include <tetramino.hpp>

#include <iostream>

namespace Tetris
{

    Block::Block(Tetramino* tetramino, sf::Sprite* sp, int i) : m_tetramino(tetramino), m_sprite(sp), m_index(i)
    {}

    int Block::index() const
    {
        return m_index;
    }

    Tetramino* Block::tetramino() const
    {
        return m_tetramino;
    }

    sf::Sprite* Block::sprite() const
    {
        return m_sprite;
    }

    Block::~Block()
    {
        m_tetramino->remove_block(this);
    }

    const Position& Block::position() const
    {
        return m_position;
    }

    void Block::position(const Position& pos)
    {
        GameMap* map         = Game::instance()->map();
        Block*& current_slot = map->slot_at(m_position);

        if (current_slot != this && current_slot != nullptr)
        {
            std::clog << "Index: " << m_index << std::endl
                      << "Pos: " << m_position.x << " " << m_position.y << std::endl;
            throw std::runtime_error("Block at current slot is not valid!");
        }

        current_slot      = nullptr;
        map->slot_at(pos) = this;
        m_position        = pos;

        sf::Vector2f new_position = convert_to_screen_space(pos);
        m_sprite->setPosition(new_position);
    }

    sf::Vector2f Block::convert_to_screen_space(Position pos)
    {
        sf::Vector2f new_position = sf::Vector2f(pos.x, pos.y);
        new_position.x *= block_size;
        new_position.y *= block_size;
        return new_position;
    }

    static void change_pos(Position& pos, MoveDir dir, int value)
    {
        switch (dir)
        {
            case MoveDir::Down:
                pos.y += value;
                break;
            case MoveDir::Left:
                pos.x -= value;
                break;
            case MoveDir::Right:
                pos.x += value;
                break;
            default:
                throw std::runtime_error("Incorrect direction!");
        }
    }

    void Block::recursive_move(unsigned int blocks, MoveDir dir)
    {
        GameMap* map         = Game::instance()->map();
        Block*& current_slot = map->slot_at(m_position);

        change_pos(m_position, dir, 1);

        Block*& next_slot = map->slot_at(m_position);
        if (next_slot)
        {
            next_slot->recursive_move(blocks, dir);
        }

        current_slot = nullptr;
        next_slot    = this;

        change_pos(m_position, dir, blocks - 1);

        sf::Vector2f new_position = sf::Vector2f(m_position.x, m_position.y);
        new_position.x *= block_size;
        new_position.y *= block_size;
        m_sprite->setPosition(new_position);

        m_is_updated = true;
    }
}// namespace Tetris
