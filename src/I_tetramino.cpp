#include <I_tetramino.hpp>
#include <background.hpp>
#include <iostream>

ITetramino::ITetramino()
{
    start_position = {640 / 2 - 2 * 60, 20};
    for (int i = 0; i < 4; i++)
    {
        sf::Sprite* square = new sf::Sprite();
        square->setTexture(*different_squares[0]);
        tetramino_sprite.push_back(square);
    }

    set_start_pos(start_position);
}

void ITetramino::set_start_pos(sf::Vector2u position)
{
    for (auto i : tetramino_sprite)
    {
        i->setPosition(sf::Vector2f(position));
        position.x += SIZE;
        std::clog << position.x << " " << position.y << std::endl;
    }

    is_horisontal = true;
}

void ITetramino::rotate()
{
    sf::Vector2f rotation_point;

    if (is_horisontal)
    {
        rotation_point = {tetramino_sprite[1]->getPosition().x, tetramino_sprite[1]->getPosition().y - SIZE};

        for (auto i : tetramino_sprite)
        {
            i->setPosition(rotation_point);
            rotation_point.y += SIZE;
        }

        is_horisontal = false;
        return;
    }

    rotation_point = {tetramino_sprite[0]->getPosition().x - SIZE, tetramino_sprite[0]->getPosition().y + SIZE};
    std::clog << rotation_point.x << "   " << rotation_point.y << std::endl;
    set_start_pos(sf::Vector2u(rotation_point));
}


ITetramino::~ITetramino()
{
    for (auto i : tetramino_sprite)
    {
        delete i;
    }
}
