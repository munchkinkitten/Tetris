#pragma once
#include <SFML/Window.hpp>
#include <game_map.hpp>
#include <object.hpp>

namespace Tetris
{
    enum class GameStage : unsigned int
    {
        Launched   = 0,
        Playing    = 1,
        GameOver   = 2,
        ForcedExit = 3,
    };

    enum class UserChoiceOnStartWindow
    {
        Exit,
        StartGame,
    };

    class Game : public Object
    {
    private:
        sf::RenderWindow* m_window;
        static Game* m_game;
        GameMap* m_map;
        GameStage m_stage;

        sf::Texture game_over_texture;
        sf::Sprite game_over_sprite;

    public:
        static Game* instance();
        static void destroy_all();
        Game();
        ~Game();

        void process_events();
        Game& init();

        int start();
        Game& stage(GameStage stage);
        GameStage stage();
        UserChoiceOnStartWindow start_window();

        sf::RenderWindow& window() const;
        void render_window();
        void update() override;

        void game_over();

        void process_event(const sf::Event& event) override;
        GameMap* map() const;
    };
}// namespace Tetris
