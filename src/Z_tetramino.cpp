#include <Z_tetramino.hpp>
#include <iostream>

ZTetramino::ZTetramino()
{
    start_position = {640 / 2 - 60, 20};
    for (int i = 0; i < 4; i++)
    {
        sf::Sprite* square = new sf::Sprite();
        square->setTexture(*different_squares[5]);
        tetramino_sprite.push_back(square);
    }

    set_start_pos(start_position);
}

void ZTetramino::set_start_pos(sf::Vector2u position)
{
    position.x -= SIZE;
    for (int i = 0; i < 4; i++)
    {
        tetramino_sprite[i]->setPosition(sf::Vector2f(position));
        position.x += SIZE;
        if (i % 2 == 1)
        {
            position.y += SIZE;
            position.x -= SIZE;
        }
    }

    is_horisontal = true;
}

void ZTetramino::rotate()
{
    sf::Vector2f start_point;
    if (is_horisontal)
    {
        start_point = {tetramino_sprite[0]->getPosition().x + 2 * SIZE, tetramino_sprite[0]->getPosition().y - SIZE};

        for (int i = 0; i < 4; i++)
        {
            tetramino_sprite[i]->setPosition(start_point);
            start_point.y += SIZE;
            if (i % 2 == 1)
            {
                start_point.x -= SIZE;
                start_point.y -= SIZE;
            }
        }

        is_horisontal = false;
        return;
    }

    start_point = {tetramino_sprite[0]->getPosition().x - SIZE, tetramino_sprite[0]->getPosition().y + SIZE};
    std::clog << start_point.x << "   " << start_point.y << std::endl;
    set_start_pos(sf::Vector2u(start_point));
}

ZTetramino::~ZTetramino()
{
    for (auto i : tetramino_sprite)
    {
        delete i;
    }
}
