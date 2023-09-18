#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <block.hpp>
#include <functional>
#include <object.hpp>

namespace Tetris
{
    class Tetramino : public Object
    {
    protected:
        struct BlockEntry {
            Position pos;
            int index;

            BlockEntry(const Position& pos, int index = -1) : pos(pos), index(index)
            {}
        };

        static std::vector<sf::Texture> m_textures;
        static std::vector<Tetramino* (*) ()> m_constructors;
        std::vector<BlockEntry> required_start_blocks;

        using Func1 = std::function<bool(const Position&)>;
        using Func2 = std::function<void(Position&)>;

        std::vector<class Block*> m_blocks;
        std::vector<MoveDir> m_moves;

        bool m_is_active = true;
        bool m_is_valid = false;
        bool m_need_create_tetramino = false;
        int m_accept_events_count = -1;


        static void load_textures();
        bool can_move(const Func1& f1, const Func2& f2) const;
        void move(int blocks, MoveDir dir);
        bool check_game_over();

    protected:
        sf::Time second = sf::seconds(1);
        sf::Clock clock;

        int rotation_angle = 0;

        sf::Texture* m_base_texture = nullptr;

        Block* new_block(const Position& pos = {0, 0}, int index = -1);
        virtual bool can_rotate();

        bool is_valid_position(const Position& pos);

    public:
        void initialize();
        Tetramino();

        virtual void rotate();

        bool is_active() const;
        void is_active(bool flag);
        void remove_block(class Block* block);
        static Tetramino* random_tetramino();
        void render(sf::RenderWindow& window) override;
        bool can_move_down() const;
        bool can_move_left() const;
        bool can_move_right() const;
        void update() override;
        void process_event(const sf::Event& event) override;

        ~Tetramino();
    };
}// namespace Tetris
