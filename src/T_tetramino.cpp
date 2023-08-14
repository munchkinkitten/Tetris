#include <T_tetramino.hpp>
#include <iostream>

TTetramino::TTetramino()
{
    start_position = {640 / 2 - 60, 20};
    for (int i = 0; i < 4; i++)
    {
        sf::Sprite* square = new sf::Sprite();
        square->setTexture(*different_squares[4]);
        tetramino_sprite.push_back(square);
    }

    set_start_pos(sf::Vector2u(start_position));
}

void TTetramino::set_start_pos(sf::Vector2u position)
{
    position.x -= SIZE;
    for (int i = 0; i < 3; i++)
    {
        tetramino_sprite[i]->setPosition(sf::Vector2f(position));
        position.x += SIZE;
    }

    position = {position.x - 2 * SIZE, position.y + SIZE};
    tetramino_sprite[3]->setPosition(sf::Vector2f(position));
    is_3_pos     = false;
    is_start_pos = true;
    std::clog << __PRETTY_FUNCTION__ << std::endl;
}

void TTetramino::set_1_pos(sf::Vector2u position)
{
    position.y -= 2 * SIZE;
    for (int i = 0; i < 3; i++)
    {
        tetramino_sprite[i]->setPosition(sf::Vector2f(position.x, position.y + SIZE));
        position.y += SIZE;
    }

    position.x -= SIZE;
    position.y -= SIZE;

    tetramino_sprite[3]->setPosition(sf::Vector2f(position));
    is_start_pos = false;
    is_1_pos     = true;
    std::clog << __PRETTY_FUNCTION__ << std::endl;
}

void TTetramino::set_2_pos(sf::Vector2u position)
{
    position.x -= SIZE;
    for (int i = 0; i < 3; i++)
    {
        tetramino_sprite[i]->setPosition(sf::Vector2f(position));
        position.x += SIZE;
    }

    position.y -= SIZE;
    position.x -= 2 * SIZE;

    tetramino_sprite[3]->setPosition(sf::Vector2f(position));
    is_1_pos = false;
    is_2_pos = true;
    std::clog << __PRETTY_FUNCTION__ << std::endl;
}

void TTetramino::set_3_pos(sf::Vector2u position)
{
    position.y -= 2 * SIZE;
    for (int i = 0; i < 3; i++)
    {
        tetramino_sprite[i]->setPosition(sf::Vector2f(position.x, position.y + SIZE));
        position.y += SIZE;
    }

    position.x += SIZE;
    position.y -= SIZE;

    tetramino_sprite[3]->setPosition(sf::Vector2f(position));
    is_2_pos = false;
    is_3_pos = true;
    std::clog << __PRETTY_FUNCTION__ << std::endl;
}

void TTetramino::rotate()
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

    for (auto mafing : tetramino_sprite)
    {
        std::clog << mafing->getPosition().x << " " << mafing->getPosition().y << std::endl;
    }
    std::clog << std::endl;
}

//void update(const sf::Event& event);

TTetramino::~TTetramino()
{
    for (auto i : tetramino_sprite)
    {
        delete i;
    }
}
