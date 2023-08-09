#pragma once
#include <tetramino.hpp>

class JTetramino : public Tetramino
{
private:
    sf::Vector2u start_position;
    bool is_start_pos, is_1_pos, is_2_pos, is_3_pos;
    void set_start_pos(sf::Vector2u position) override;
    void set_1_pos(sf::Vector2u position);
    void set_2_pos(sf::Vector2u position);
    void set_3_pos(sf::Vector2u position);
public:
    JTetramino();
    void rotate();
    //void update(const sf::Event& event);
    ~JTetramino();
};
