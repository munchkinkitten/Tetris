#include <iostream>
#include <score_counter.hpp>

namespace Tetris
{
    ScoreCounter* ScoreCounter::m_score_counter = nullptr;

    ScoreCounter* ScoreCounter::instance()
    {
        if (m_score_counter == nullptr)
        {
            m_score_counter = new ScoreCounter();
        }

        return m_score_counter;
    }

    void ScoreCounter::add_point()
    {
        score++;
        std::clog << score << std::endl;
    }

    void ScoreCounter::reset()
    {
        score = 0;
    }

    void ScoreCounter::render(sf::RenderWindow& window)
    {
        std::string number = std::to_string(score);
        sf::Vector2f pos(20, 20);

        for(std::size_t i = 0; i < number.length(); i++)
        {
            sf::Sprite& to_draw = sprites[static_cast<int>(number[i] - '0')];
            to_draw.setPosition(pos);
            window.draw(to_draw);
            pos.x += 50;
        }

        //window.draw(sprites[0]);
    }

    int ScoreCounter::get_score() const
    {
        return score;
    }

    ScoreCounter::ScoreCounter() : score(0)
    {
        init_num_sprites();
        this->render_priority(2);
    }

    //    void ScoreCounter::invert_color(sf::Texture& texture)
    //    {
    //        sf::Image image = numbers.copyToImage();
    //        auto size       = image.getSize();
    //        for (sf::Uint32 x = 0; x < size.x; x++)
    //        {
    //            for (sf::Uint32 y = 0; y < size.y; y++)
    //            {
    //                sf::Color color = image.getPixel(x, y);
    //                if (color.a != 0)
    //                {
    //                    color.r = 255 - color.r;
    //                    color.g = 255 - color.g;
    //                    color.b = 255 - color.b;

    //                    image.setPixel(x, y, color);
    //                }
    //            }
    //        }
    //    }

    void ScoreCounter::init_num_sprites()
    {
        textures.resize(10);
        sprites.resize(10);
        for (int i = 0; i < 10; i++)
        {

            textures[i].loadFromFile("resources/numbers/" + std::to_string(i) + ".png");
            sprites[i].setTexture(textures[i]);
        }
    }

    ScoreCounter::~ScoreCounter()
    {
    }
}// namespace Tetris
