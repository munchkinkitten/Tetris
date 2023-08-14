#include <iostream>
#include <tetramino.hpp>

Tetramino::Tetramino()
{
    for (int i = 1; i < 8; i++)
    {
        std::string path     = "resources/images/" + std::to_string(i) + "_sq.png";
        sf::Texture* texture = new sf::Texture();
        texture->loadFromFile(path);
        different_squares.push_back(texture);
    }

    clock.restart();
}

void Tetramino::move_down()
{
    for (int i = 0; i < 4; i++)
    {
        tetramino_sprite[i]->setPosition(
                {tetramino_sprite[i]->getPosition().x, tetramino_sprite[i]->getPosition().y + SIZE});
    }
}

void Tetramino::render(sf::RenderWindow& window)
{
    for (auto i : tetramino_sprite)
    {
        window.draw(*i);
    }
}

bool Tetramino::can_move_down()
{
    for (auto i : tetramino_sprite)
    {
        if (i->getPosition().y >= 820.f - SIZE)
        {
            return false;
        }
    }
    return true;
}

bool Tetramino::can_move_left()
{
    for (auto i : tetramino_sprite)
    {
        if (i->getPosition().x <= 20.f)
        {
            return false;
        }
    }
    return true;
}

bool Tetramino::can_move_right()
{
    for (auto i : tetramino_sprite)
    {
        if (i->getPosition().x >= 620.f - SIZE)
        {
            return false;
        }
    }
    return true;
}

void Tetramino::move(const sf::Event& event)
{
    if (event.key.code == sf::Keyboard::Key::Right && can_move_right())
    {
        move_right();
    }
    else if (event.key.code == sf::Keyboard::Key::Left && can_move_left())
    {
        move_left();
    }
    else if (event.key.code == sf::Keyboard::Key::Up && can_rotate)
    {
        rotate();
    }
    else if (event.key.code == sf::Keyboard::Key::Down && can_move_down())
    {
        move_down();
    }
    else
        return;
}

void Tetramino::move_left()
{
    for (int i = 0; i < 4; i++)
    {
        tetramino_sprite[i]->setPosition(
                {tetramino_sprite[i]->getPosition().x - SIZE, tetramino_sprite[i]->getPosition().y});
    }
}

void Tetramino::move_right()
{
    for (int i = 0; i < 4; i++)
    {
        tetramino_sprite[i]->setPosition(
                {tetramino_sprite[i]->getPosition().x + SIZE, tetramino_sprite[i]->getPosition().y});
    }
}

void Tetramino::update()
{
    if (clock.getElapsedTime() >= second && can_move_down())
    {
        move_down();
        can_rotate = true;
        clock.restart();
    }

    for (auto i : tetramino_sprite)
    {
        if (i->getPosition().x >= 640 - 20 - SIZE || i->getPosition().x <= 20)
            rotate();
    }
}

void Tetramino::set_start_pos(sf::Vector2u position)
{}

void Tetramino::process_event(const sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed && can_move_down())
        move(event);
}

Tetramino::~Tetramino()
{}

