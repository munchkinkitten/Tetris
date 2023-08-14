#include <SFML/Graphics.hpp>
#include <background.hpp>
#include <iostream>
#include <chrono>
#include <S_tetramino.hpp>


inline auto current_time()
{
    return std::chrono::steady_clock::now();
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(640, 940), "Tetris");
    window.setPosition({window.getPosition().x, window.getPosition().y - 40});
    window.setFramerateLimit(30);

    Background background("resources/images/background.jpg");
    background.set_position({0, 0});

    STetramino z_tetramino;

    while (window.isOpen())
    {
        auto begin = current_time();
        sf::Event event;
        while (window.pollEvent(event))
       {
            if (event.type == sf::Event::Closed)
                window.close();

            for(Object* object : Object::objects())
            {
                object->process_event(event);
            }
        }

        for(Object* object : Object::objects())
        {
            object->update();
        }

        window.clear();

        background.render(window);
        z_tetramino.render(window);


        window.display();

        auto end = current_time();
        auto time = float(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) / 1000.f;
        float FPS = 1000.f / time;

        window.setTitle(std::to_string(FPS));
    }

    return 0;
}
