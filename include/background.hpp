#pragma once
#include <object.hpp>

class Background : public Object
{
private:
    sf::Texture background_texture;
    sf::Sprite background_sprite;

public:
    Background(const char* filepath);
    void set_position(const sf::Vector2f& position);
    const sf::Vector2f& get_position();
    void render(sf::RenderWindow& window);
    ~Background();
};
