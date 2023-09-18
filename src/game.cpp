#include <SFML/Graphics.hpp>
#include <backgroud.hpp>
#include <game.hpp>
#include <iostream>
#include <tetramino.hpp>
#include <thread>

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
        m_window->setFramerateLimit(30);
        stage(GameStage::Launched);
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
            if (m_stage == GameStage::GameOver)
            {
                for (int i = 0; i < 50; i++) std::clog << i << std::endl;
                return;
            }

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

        game_over_texture.loadFromFile("resources/game_over.png");
        game_over_sprite.setTexture(game_over_texture);
        game_over_sprite.setPosition({static_cast<float>(m_window->getSize().x - game_over_texture.getSize().x) / 2,
                                      static_cast<float>(m_window->getSize().y - game_over_texture.getSize().y) / 2});

        return *this;
    }

    void Game::process_event(const sf::Event& event)
    {
        if (event.type == sf::Event::Closed)
        {
            stage(GameStage::ForcedExit);
        }
    }

    GameMap* Game::map() const
    {
        return m_map;
    }

    int Game::start()
    {
        init();

        m_stage = GameStage::Launched;

        while (start_window() != UserChoiceOnStartWindow::Exit)
        {
            Object::clean_tetraminos();
            m_map->clean();
            Tetramino::random_tetramino()->initialize();

            m_stage = GameStage::Playing;
            while (m_stage == GameStage::Playing)
            {
                process_events();
                // Update all
                Object::update_all();

                window().clear();
                render_window();
                window().display();
            }
        }

        return 0;
    }

    Game& Game::stage(GameStage stage)
    {
        m_stage = stage;
        return *this;
    }

    GameStage Game::stage()
    {
        return m_stage;
    }


    UserChoiceOnStartWindow Game::start_window()
    {
        if (m_stage == GameStage::ForcedExit)
        {
            return UserChoiceOnStartWindow::Exit;
        }

        if (m_stage == GameStage::GameOver)
        {
            game_over();
            m_window->draw(game_over_sprite);
            return UserChoiceOnStartWindow::Exit;
        }

        else if (m_stage == GameStage::Launched)
        {
        }

        return UserChoiceOnStartWindow::StartGame;
    }

    void Game::game_over()
    {
        m_stage = GameStage::GameOver;
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
