#include <SFML/Graphics.hpp>
#include <backgroud.hpp>
#include <game.hpp>
#include <tetramino.hpp>

namespace Tetris
{
    Game* Game::m_game = nullptr;

    void Game::destroy_all()
    {
        for (Object* object : Object::objects())
        {
            delete object;
        }
    }

    Game* Game::instance()
    {
        if (m_game == nullptr)
        {
            return new Game();
        }

        return m_game;
    }

    Game::Game()
    {
        m_game = this;
        sf::VideoMode mode(400, 870);
        m_window = new sf::RenderWindow(mode, "Tetris");
        m_window->setFramerateLimit(3);
        active(true);
    }

    Game::~Game()
    {
        delete m_window;
        delete m_map;
    }

    void Game::process_events()
    {
        sf::Event event;
        while (window().pollEvent(event))
        {
            for (Object* object : Object::objects())
            {
                object->process_event(event);
            }
        }
    }

    Game& Game::init()
    {
        m_map = new GameMap();
        srand(time(nullptr));
        new Background();

        Tetramino::random_tetramino();
        return *this;
    }

    void Game::process_event(const sf::Event& event)
    {
        if (event.type == sf::Event::Closed)
        {
            active(false);
        }
    }

    GameMap* Game::map() const
    {
        return m_map;
    }


    int Game::start()
    {
        while (active())
        {
            process_events();

            // Update all
            for (Object* object : Object::objects())
            {
                object->update();
            }

            window().clear();
            render_window();
            window().display();
        }

        return 0;
    }

    bool Game::active() const
    {
        return m_game_active;
    }

    Game& Game::active(bool flag)
    {
        m_game_active = flag;
        return *this;
    }

    sf::RenderWindow& Game::window() const
    {
        return *m_window;
    }

    void Game::render_window()
    {
        Priority priority     = 0;
        Priority max_priority = Object::max_priority();
        while (priority <= max_priority)
        {
            for (Object* object : Object::objects())
            {
                if (object->render_priority() == priority)
                {
                    object->render(window());
                }
            }

            priority++;
        }
    }

    static void delete_row(Row& row)
    {
        for (Block*& block : row)
        {
            delete block;
            block = nullptr;
        }
    }

    void Game::update()
    {
        int i           = m_map->map.size() - 1;
        int blocks_move = 0;
        while (i >= 0)
        {
            Row& row           = m_map->map[i];
            unsigned int count = 0;
            for (Block*& block : row)
            {
                if (block && block->tetramino()->is_active() == false)
                {
                    count++;
                }
                else
                {
                    break;
                }
            }

            if (count == 10)
            {
                delete_row(row);
                blocks_move++;
            }
            else if (blocks_move > 0)
            {
                for (Block*& block : row)
                {
                    if (block)
                    {
                        Position pos = block->position();
                        pos.y += blocks_move;
                        block->position(pos);
                    }
                }
            }

            i--;
        }
    }
}// namespace Tetris
