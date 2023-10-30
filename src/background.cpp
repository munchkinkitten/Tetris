#include <backgroud.hpp>
#include <game.hpp>

namespace Tetris
{
    Background::Background()
    {
        m_texture.loadFromFile("resources/background.jpg");
        m_sprite.setTexture(m_texture, true);
//        auto ws = Game::instance()->window().getSize();
//        auto ts = m_texture.getSize();
//        m_sprite.setScale({static_cast<float>(ws.x) / static_cast<float>(ts.x),
//                           static_cast<float>(ws.y) / static_cast<float>(ts.y)});
    }

    void Background::render(sf::RenderWindow& window)
    {
        window.draw(m_sprite);
        sf::Vertex line[2];
        line[0].position = {Block::block_size * 10, 0};
        line[1].position = {Block::block_size * 10, static_cast<float>(m_texture.getSize().y)};
    }
}// namespace Tetris
