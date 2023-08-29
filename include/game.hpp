#pragma once
#include <SFML/Window.hpp>
#include <object.hpp>
#include <game_map.hpp>

namespace Tetris
{
    class Game : public Object
    {
    private:
        sf::RenderWindow* m_window;
        static Game* m_game;
        bool m_game_active = false;
        GameMap* m_map;

    public:
        static Game* instance();
        static void destroy_all();
        Game();
        ~Game();

        void process_events();
        Game& init();

        int start();
        bool active() const;
        Game& active(bool flag);

        sf::RenderWindow& window() const;
        void render_window();
        void update() override;

        void process_event(const sf::Event& event) override;
        GameMap* map() const;
    };
}// namespace Tetris
