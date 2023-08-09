#pragma once
#include <SFML/Graphics.hpp>
#include <set>

class Object{
private:
    static std::set<Object*> m_objects;

public:
    Object();
    virtual void render(sf::RenderWindow& window);
    virtual void update();
    virtual void process_event(const sf::Event& event);

    static void destroy_all();
    static const std::set<Object*>& objects();

    virtual ~Object();
};
