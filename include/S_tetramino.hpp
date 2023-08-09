#pragma once
#include <tetramino.hpp>

class STetramino : public Tetramino
{
private:
    sf::Vector2u start_position;
    bool is_horisontal = true;
    void set_start_pos(sf::Vector2u position);
public:
    STetramino();
    void rotate();
    //void update(const sf::Event& event);
    ~STetramino();

};
