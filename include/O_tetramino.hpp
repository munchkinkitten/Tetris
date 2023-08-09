#pragma once
#include <tetramino.hpp>

class OTetramino : public Tetramino
{
private:
    sf::Vector2u start_position;
    void set_start_pos(sf::Vector2u position) override;
public:
    OTetramino();
    void rotate();
    //void update(const sf::Event& event);
    ~OTetramino();
};
