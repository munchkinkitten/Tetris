#include <background.hpp>

Background::Background(const char* filepath)
{
    background_texture.loadFromFile(filepath);
    background_sprite.setTexture(background_texture);
}

void Background::set_position(const sf::Vector2f& position)
{
    background_sprite.setPosition(position);
}

const sf::Vector2f& Background::get_position()
{
    return background_sprite.getPosition();
}

void Background::render(sf::RenderWindow& window)
{
    window.draw(background_sprite);
}

Background::~Background()
{}
