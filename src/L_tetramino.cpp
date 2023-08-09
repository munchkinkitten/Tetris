#include <L_tetramino.hpp>
#include <iostream>

LTetramino::LTetramino()
{
    start_position = {640 / 2 - 60, 20};
    for (int i = 0; i < 4; i++)
    {
        sf::Sprite* square = new sf::Sprite();
        square->setTexture(*different_squares[1]);
        tetramino_sprite.push_back(square);
    }

    set_start_pos(sf::Vector2u(start_position));
}

void LTetramino::set_start_pos(sf::Vector2u position)
{
    for (int i = 0; i < 3; i++)
    {
        tetramino_sprite[i]->setPosition(sf::Vector2f(position));
        position.y += SIZE;
    }

    position = {position.x + SIZE, position.y - SIZE};
    tetramino_sprite[3]->setPosition(sf::Vector2f(position));
    is_3_pos     = false;
    is_start_pos = true;
    std::clog << __PRETTY_FUNCTION__ << std::endl;
}

void LTetramino::set_1_pos(sf::Vector2u position)
{
    position.x -= SIZE;
    for (int i = 0; i < 3; i++)
    {
        tetramino_sprite[i]->setPosition(sf::Vector2f(position.x + SIZE, position.y));
        position.x += SIZE;
    }

    position.y += SIZE;

    tetramino_sprite[3]->setPosition(sf::Vector2f(position));
    is_start_pos = false;
    is_1_pos     = true;
    std::clog << __PRETTY_FUNCTION__ << std::endl;
}

void LTetramino::set_2_pos(sf::Vector2u position)
{
    position.y -= SIZE;
    for (int i = 0; i < 3; i++)
    {
        tetramino_sprite[i]->setPosition(sf::Vector2f(position.x, position.y + SIZE));
        position.y += SIZE;
    }

    position.x -= SIZE;

    tetramino_sprite[3]->setPosition(sf::Vector2f(position));
    is_1_pos = false;
    is_2_pos = true;
    std::clog << __PRETTY_FUNCTION__ << std::endl;
}

void LTetramino::set_3_pos(sf::Vector2u position)
{
    position.x += SIZE;
    for (int i = 0; i < 3; i++)
    {
        tetramino_sprite[i]->setPosition(sf::Vector2f(position.x - SIZE, position.y));
        position.x -= SIZE;
    }

    position.y += SIZE;

    tetramino_sprite[3]->setPosition(sf::Vector2f(position));
    is_2_pos = false;
    is_3_pos = true;
    std::clog << __PRETTY_FUNCTION__ << std::endl;
}

void LTetramino::rotate()
{
    if (is_start_pos && !is_1_pos && !is_2_pos && !is_3_pos)
    {
        set_1_pos(sf::Vector2u(tetramino_sprite[1]->getPosition()));
    }
    else if (!is_start_pos && is_1_pos && !is_2_pos && !is_3_pos)
    {
        set_2_pos(sf::Vector2u(tetramino_sprite[1]->getPosition()));
    }
    else if (!is_start_pos && !is_1_pos && is_2_pos && !is_3_pos)
    {
        set_3_pos(sf::Vector2u(tetramino_sprite[1]->getPosition()));
    }
    else if (!is_start_pos && !is_1_pos && !is_2_pos && is_3_pos)
    {
        set_start_pos(sf::Vector2u(tetramino_sprite[1]->getPosition()));
    }
}

//void update(const sf::Event& event);

LTetramino::~LTetramino()
{
    for (auto i : tetramino_sprite)
    {
        delete i;
    }
}
