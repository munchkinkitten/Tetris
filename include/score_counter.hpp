#pragma once
#include <object.hpp>

namespace Tetris
{
    class ScoreCounter : public Object
    {
    private:

        unsigned int score = 0;
        std::vector<sf::Texture> textures;
        std::vector<sf::Sprite> sprites;
        static ScoreCounter* m_score_counter;

        void invert_color(sf::Texture& texture);
        void init_num_sprites();

    public:
        static ScoreCounter* instance();
        void add_point();
        void reset();
        void render(sf::RenderWindow& window) override;
        int get_score() const;
        ScoreCounter();
        ~ScoreCounter();
    };
}// namespace Tetris
