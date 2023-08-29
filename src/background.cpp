#include <backgroud.hpp>


namespace Tetris
{
    Background::Background()
    {
        m_texture.loadFromFile("resources/background.jpg");
        m_sprite.setTexture(m_texture, true);
    }

    void Background::render(sf::RenderWindow& window)
    {
        window.draw(m_sprite);
    }
}
