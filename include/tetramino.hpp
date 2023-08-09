#pragma once

#include <object.hpp>
#include <chrono>

class Tetramino : public Object
{
protected:
    std::vector<sf::Texture*> different_squares;
    std::vector<sf::Sprite*> tetramino_sprite;
    sf::Clock clock;
    sf::Time second = sf::seconds(1);
    bool can_rotate = false;
    virtual void set_start_pos(sf::Vector2u position);

public:
    int SIZE = 60;
    Tetramino();
    bool can_move_down();
    bool can_move_left();
    bool can_move_right();
    void move_down();
    void move(const sf::Event& event);
    void move_right();
    void move_left();
    virtual void rotate() = 0;
    void render(sf::RenderWindow& window) override;
    void update() override;
    void process_event(const sf::Event& event) override;
    ~Tetramino();
};
