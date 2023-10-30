#include <SFML/Graphics.hpp>
#include <backgroud.hpp>
#include <game.hpp>
#include <iostream>
#include <score_counter.hpp>
#include <tetramino.hpp>
#include <thread>

namespace Tetris
{
    Game* Game::m_game = nullptr;

    void Game::destroy_all()
    {
        while (!Object::objects().empty())
        {
            delete (*Object::objects().begin());
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
        //sf::VideoMode mode(400, 870);
        sf::VideoMode mode(600, 870);
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
            for (Object* object : Object::objects())
            {
                if (!object->is_next_tetramino())
                    object->process_event(event);
            }
        }
    }

    Game& Game::init()
    {
        m_map = new GameMap();
        srand(time(nullptr));
        new Background();

        ScoreCounter::instance();

        start_texture.loadFromFile("resources/enter.png");

        start_sprite.setTexture(start_texture);

        start_sprite.setPosition({static_cast<float>(m_window->getSize().x - start_texture.getSize().x) / 2,
                                  static_cast<float>(m_window->getSize().y - start_texture.getSize().y) / 2});


        game_over_texture.loadFromFile("resources/game_over.png");
        game_over_sprite.setTexture(game_over_texture);
        game_over_sprite.setPosition({static_cast<float>(m_window->getSize().x - game_over_texture.getSize().x) / 2,
                                      static_cast<float>(m_window->getSize().y - game_over_texture.getSize().y) / 2});

        music.openFromFile("resources/sounds/music.wav");
        music.setVolume(60);

        sb_rotate.loadFromFile("resources/sounds/rotate.wav");
        ;
        sb_delete_row.loadFromFile("resources/sounds/row_delete.wav");
        sb_game_over.loadFromFile("resources/sounds/game_over.wav");
        sb_not.loadFromFile("resources/sounds/not.wav");

        rotate_sound.setBuffer(sb_rotate);
        delete_row_sound.setBuffer(sb_delete_row);
        game_over_sound.setBuffer(sb_game_over);
        not_sound.setBuffer(sb_not);

        return *this;
    }

    bool Game::delete_row_is_playing()
    {
        return delete_row_sound.getStatus() == sf::Sound::Playing;
    }

    static bool is_pressed_key(sf::Keyboard::Key key, const sf::Event& event)
    {
        if (event.type != sf::Event::KeyPressed)
        {
            return false;
        }

        return event.key.code == key;
    }

    void Game::process_event(const sf::Event& event)
    {
        if (event.type == sf::Event::Closed)
        {
            m_next_stage = GameStage::ForcedExit;
        }

        if ((m_stage == GameStage::GameOver || m_stage == GameStage::Launched) &&
            is_pressed_key(sf::Keyboard::Enter, event))
        {
            m_next_stage = GameStage::Playing;
        }
    }

    GameMap* Game::map() const
    {
        return m_map;
    }

    int Game::start()
    {
        init();

        m_stage      = GameStage::Launched;
        m_next_stage = GameStage::Launched;

        music.setLoop(true);
        music.play();

        while (m_stage != GameStage::ForcedExit)
        {
            process_events();
            Object::update_all();
            window().clear();

            switch (m_stage)
            {
                case GameStage::Launched:
                    render_start();
                    break;
                case GameStage::GameOver:
                    render_game_over();
                    break;
                case GameStage::Playing:
                    render_game();
                    break;
                default:
                    break;
            }

            window().display();

            m_stage = m_next_stage;
        }

        return 0;
    }

    void Game::play_G_O_sound()
    {
        //music.stop();
        game_over_sound.play();
    }

    void Game::play_rotate_sound()
    {
        rotate_sound.play();
    }

    void Game::play_delete_row_sound()
    {
        delete_row_sound.play();
    }

    void Game::play_not_sound()
    {
        not_sound.play();
    }

    Game& Game::stage(GameStage stage)
    {
        m_next_stage = stage;
        return *this;
    }

    GameStage Game::stage()
    {
        return m_stage;
    }

    sf::RenderWindow& Game::window() const
    {
        return *m_window;
    }

    void Game::render_start()
    {
        // Logic of rendering start window texture
        render_game();
        m_window->draw(start_sprite);

        if (m_next_stage == GameStage::Playing)
        {
            Tetramino::random_tetramino()->initialize();
            Tetramino::init_next_tetramino();
        }
    }

    void Game::render_game()
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

    void Game::render_game_over()
    {
        // Logic of rendering game over texture
        render_game();
        m_window->draw(game_over_sprite);

        if (m_next_stage == GameStage::Playing)
        {
            Object::clean_tetraminos();
            m_map->clean();
            Tetramino::random_tetramino()->initialize();
            ScoreCounter::instance()->reset();
        }
    }

    static void delete_row(Row& row)
    {
        for (Block*& block : row)
        {
            delete block;
            block = nullptr;
        }

        if (!Game::instance()->delete_row_is_playing())
            Game::instance()->play_delete_row_sound();
        ScoreCounter::instance()->add_point();
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
