#pragma once
#include <object.hpp>
#include <SFML/Graphics.hpp>

namespace Tetris
{
    class Background : public Object
    {
    private:
        sf::Texture m_texture;
        sf::Sprite m_sprite;

    public:
        Background();
        void render(sf::RenderWindow& window) override;
    };
}
