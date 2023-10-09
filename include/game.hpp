#pragma once
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
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

    class Game : public Object
    {
    private:
        sf::RenderWindow* m_window;
        static Game* m_game;
        GameMap* m_map;
        GameStage m_stage;
        GameStage m_next_stage;

        sf::Texture start_texture;
        sf::Sprite start_sprite;

        sf::Texture game_over_texture;
        sf::Sprite game_over_sprite;

    //public:
        sf::Music music;

        sf::SoundBuffer sb_rotate;
        sf::SoundBuffer sb_delete_row;
        sf::SoundBuffer sb_game_over;
        sf::SoundBuffer sb_not;

        sf::Sound rotate_sound;
        sf::Sound delete_row_sound;
        sf::Sound game_over_sound;
        sf::Sound not_sound;

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

        sf::RenderWindow& window() const;
        void render_start();
        void render_game();
        void render_game_over();
        void update() override;

        bool delete_row_is_playing();
        void play_G_O_sound();
        void play_rotate_sound();
        void play_delete_row_sound();
        void play_not_sound();

        void game_over();

        void process_event(const sf::Event& event) override;
        GameMap* map() const;
    };
}// namespace Tetris
