#include <O_tetramino.hpp>
#include <iostream>

OTetramino::OTetramino()
{
    start_position = {640 / 2 - 60, 20};
    for (int i = 0; i < 4; i++)
    {
        sf::Sprite* square = new sf::Sprite();
        square->setTexture(*different_squares[3]);
        tetramino_sprite.push_back(square);
    }

    set_start_pos(sf::Vector2u(start_position));
}

void OTetramino::set_start_pos(sf::Vector2u position)
{
    tetramino_sprite[0]->setPosition(sf::Vector2f(position));
    tetramino_sprite[1]->setPosition(position.x + SIZE, position.y);
    tetramino_sprite[2]->setPosition(position.x, position.y + SIZE);
    tetramino_sprite[3]->setPosition(position.x + SIZE, position.y + SIZE);
}

void OTetramino::rotate()
{}

//void update(const sf::Event& event);

OTetramino::~OTetramino()
{
    for (auto i : tetramino_sprite)
    {
        delete i;
    }
}
