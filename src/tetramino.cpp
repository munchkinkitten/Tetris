#include <SFML/Audio.hpp>
#include <game.hpp>
#include <game_map.hpp>
#include <iostream>
#include <random>
#include <tetramino.hpp>

namespace Tetris
{

    std::vector<sf::Texture> Tetramino::m_textures;
    Tetramino* Tetramino::next_tetramino;

    void Tetramino::load_textures()
    {
        m_textures.resize(7);
        for (int i = 1; i <= 7; i++)
        {
            std::string name = std::string("resources/cubic/") + std::to_string(i) + "_sq.png";
            m_textures[i - 1].loadFromFile(name);
        }
    }

    bool Tetramino::can_rotate()
    {
        return false;
    }

    inline bool in_range(int a, int b, int c)
    {
        return a >= b && a <= c;
    }

    bool Tetramino::is_valid_position(const Position& pos)
    {
        if (!in_range(pos.x, 0, 9) || !in_range(pos.y, 0, 19))
        {
            return false;
        }
        Block* blk = Game::instance()->map()->slot_at(pos);
        if (blk && blk->tetramino() != this)
        {
            return false;
        }

        return true;
    }

    void Tetramino::initialize()
    {
        for (const BlockEntry& entry : required_start_blocks)
        {
            new_block(entry.pos, entry.index);
        }

        m_is_valid = true;
        m_is_next  = false;
        m_sprites.clear();
    }

    Block* Tetramino::new_block(const Position& pos, int index)
    {
        Block* block = new Block(this, m_sprites[index], index);
        if (index != -1)
        {
            m_blocks[index] = block;
        }
        else
        {
            for (Block*& blk : m_blocks)
            {
                if (blk == nullptr)
                {
                    blk = block;
                    break;
                }
            }
        }
        block->position(pos);
        return block;
    }

    Tetramino::Tetramino()
    {
        //std::clog << "Tetramino!" << std::endl;
        m_is_tetramino = true;
        if (m_textures.empty())
        {
            load_textures();
        }

        render_priority(1);

        std::random_device rand;
        std::uniform_int_distribution<int> distribution(0, m_textures.size() - 1);
        m_base_texture = &m_textures[distribution(rand)];

        m_blocks.resize(4, nullptr);
        m_sprites.resize(4);

        for (sf::Sprite*& sprite : m_sprites)
        {
            sprite = new sf::Sprite(*m_base_texture);
        }

        clock.restart();
        printf("CREATE: %p\n", this);
    }

    bool Tetramino::is_active() const
    {
        return m_is_active;
    }

    bool Tetramino::is_next_tetramino() const
    {
        return m_is_next;
    }

    //    void Tetramino::is_active(bool flag)
    //    {
    //        m_is_active = flag;
    //    }

    void Tetramino::remove_block(Block* block)
    {
        if (block->tetramino() == this)
        {
            for (size_t i = 0; i < m_blocks.size(); i++)
            {
                if (m_blocks[i] == block)
                {
                    m_blocks.erase(m_blocks.begin() + i);
                }
            }
            m_is_active = false;
        }
    }

    Tetramino* Tetramino::random_tetramino()
    {
        std::random_device rand;
        std::uniform_int_distribution<int> distribution(0, m_constructors.size() - 1);

        int index            = distribution(rand);
        Tetramino* tetramino = m_constructors[index]();
        return tetramino;
    }

    void Tetramino::set_correct_pos_next()
    {
        auto position = required_start_blocks[0].pos;
        // if I
        if (required_start_blocks[0].pos.y == required_start_blocks[1].pos.y &&
            required_start_blocks[1].pos.y == required_start_blocks[2].pos.y &&
            required_start_blocks[2].pos.y == required_start_blocks[3].pos.y)
        {

            position.x += 9;
            position.y += 1;

            m_sprites[0]->setPosition(Block::convert_to_screen_space(position) + sf::Vector2f{15, 0});

            for (std::size_t i = 1; i < required_start_blocks.size(); i++)
            {
                position.y += 1;
                m_sprites[i]->setPosition(Block::convert_to_screen_space(position) + sf::Vector2f{15, 0});
            }
            return;
        }

        // if O
        else if (required_start_blocks[0].pos.x == required_start_blocks[1].pos.x &&
                 required_start_blocks[0].pos.y == required_start_blocks[2].pos.y)
        {
            position.x += 7;
            position.y += 1;

            m_sprites[0]->setPosition(Block::convert_to_screen_space(position) + sf::Vector2f{35, 0});
            position.y += 1;

            for (std::size_t i = 1; i < required_start_blocks.size(); i++)
            {
                m_sprites[i]->setPosition(Block::convert_to_screen_space(position) + sf::Vector2f{35, 0});
                if (i == 1)
                {
                    position.x += 1;
                    position.y -= 1;
                }
                else
                {
                    position.y += 1;
                }
            }
            return;
        }

        //if L or J
        else if (required_start_blocks[0].pos.x == required_start_blocks[1].pos.x &&
                 required_start_blocks[1].pos.x == required_start_blocks[2].pos.x)
        {
            if (required_start_blocks[3].pos.x < required_start_blocks[0].pos.x)
            {
                position.x += 7;
                //                position.y += 1;
                m_sprites[3]->setPosition(Block::convert_to_screen_space({position.x - 1, position.y + 3}) +
                                          sf::Vector2f{35, 0});
            }
            else if (required_start_blocks[3].pos.x > required_start_blocks[0].pos.x)
            {
                position.x += 6;
                //                position.y += 1;
                m_sprites[3]->setPosition(Block::convert_to_screen_space({position.x + 1, position.y + 3}) +
                                          sf::Vector2f{35, 0});
            }

            for (auto& block : required_start_blocks)
            {
                position.y += 1;

                if (block.index == 3)
                {
                    return;
                }

                m_sprites[block.index]->setPosition(Block::convert_to_screen_space(position) + sf::Vector2f{35, 0});
            }

            return;
        }

        //Else: (T S Z)
        else
        {
            for (auto& required_block : required_start_blocks)
            {
                auto pos = required_block.pos;
                pos.x += 7;
                pos.y += 1;
                sf::Vector2f screen_pos = Block::convert_to_screen_space(pos);
                screen_pos.x += 15;
                sf::Sprite* sprite = m_sprites[required_block.index];
                sprite->setPosition(screen_pos);
            }
        }
    }

    void Tetramino::render_next_tetramino(sf::RenderWindow& window)
    {
        set_correct_pos_next();
        for (auto& required_block : required_start_blocks)
        {
            window.draw(*m_sprites[required_block.index]);
        }
    }

    void Tetramino::render(sf::RenderWindow& window)
    {
        if (!m_is_valid && !is_next_tetramino())
            return;


        if (is_next_tetramino())
        {
            render_next_tetramino(window);
            return;
        }

        for (Block* block : m_blocks)
        {
            window.draw(*block->m_sprite);
        }
    }

    bool Tetramino::can_move(const Func1& f1, const Func2& f2) const
    {
        if (m_blocks.empty())
            return false;

        GameMap* map = Game::instance()->map();

        int blocks_count = 0;

        for (Block* block : m_blocks)
        {
            if (block == nullptr)
                continue;
            blocks_count += 1;
            Position pos = block->position();
            if (!f1(pos))
            {
                return false;
            }

            f2(pos);
            Block* map_block = map->slot_at(pos);
            if (map_block != nullptr && map_block->tetramino() != this)
            {
                return false;
            }
        }

        return blocks_count != 0;
    }


    bool Tetramino::can_move_down() const
    {
        static Func1 f1 = [](const Position& pos) -> bool { return pos.y < 19; };
        static Func2 f2 = [](Position& pos) { pos.y += 1; };

        return can_move(f1, f2);
    }

    bool Tetramino::can_move_left() const
    {
        static Func1 f1 = [](const Position& pos) -> bool { return pos.x > 0; };
        static Func2 f2 = [](Position& pos) { pos.x -= 1; };

        return can_move(f1, f2);
    }

    bool Tetramino::can_move_right() const
    {
        static Func1 f1 = [](const Position& pos) -> bool { return pos.x < 9; };
        static Func2 f2 = [](Position& pos) { pos.x += 1; };

        return can_move(f1, f2);
    }

    void Tetramino::move(int blocks, MoveDir dir)
    {
        //std::clog << "Move tetramino: " << blocks << " " << (int) dir << std::endl;
        for (Block* block : m_blocks)
        {
            block->m_is_updated = false;
        }

        for (Block* block : m_blocks)
        {
            if (block->m_is_updated == false)
            {
                block->recursive_move(blocks, dir);
            }
        }
    }

    void Tetramino::process_event(const sf::Event& event)
    {
        if (event.type == sf::Event::KeyPressed && (m_is_active || abs(m_accept_events_count) > 0))
        {
            m_accept_events_count -= 1;
            bool active_status         = m_is_active;
            bool need_create_tetramino = m_need_create_tetramino;
            m_need_create_tetramino    = false;

            m_is_active = true;
            if (event.key.code == sf::Keyboard::Key::Right && can_move_right())
            {
                move(1, MoveDir::Right);
                //m_moves.push_back(MoveDir::Right);
            }
            else if (event.key.code == sf::Keyboard::Key::Left && can_move_left())
            {
                move(1, MoveDir::Left);
            }
            else if (event.key.code == sf::Keyboard::Up && can_rotate())
            {
                rotate();
                Game::instance()->play_rotate_sound();
                rotation_angle = (rotation_angle + 90) % 360;
            }
            else if (event.key.code == sf::Keyboard::Up && !can_rotate())
            {
                Game::instance()->play_not_sound();
            }
            else if (event.key.code == sf::Keyboard::Down && can_move_down())
            {
                move(1, MoveDir::Down);
            }
            else
            {
                m_accept_events_count += 1;
                m_is_active             = active_status;
                m_need_create_tetramino = need_create_tetramino;
            }
        }
    }

    void Tetramino::init_next_tetramino()
    {
        next_tetramino              = random_tetramino();
        next_tetramino->m_is_next   = true;
        next_tetramino->m_is_active = false;
    }

    bool Tetramino::check_game_over()
    {
        bool game_over = false;

        GameMap* game_map = Game::instance()->map();

        for (const BlockEntry& entry : required_start_blocks)
        {
            game_over = game_map->slot_at(entry.pos) != nullptr;
            if (!game_over)
            {
                Position second_pos = {entry.pos.x, entry.pos.y + 1};
                game_over           = game_map->slot_at(second_pos) != nullptr;
            }

            if (game_over)
            {
                break;
            }
        }

        if (game_over)
        {
            Game::instance()->stage(GameStage::GameOver);
            Game::instance()->play_G_O_sound();
        }
        return game_over;
    }

    void Tetramino::update()
    {
        if (Game::instance()->stage() != GameStage::Playing)
            return;

        if (m_is_active && !can_move_down())
        {
            m_is_active             = false;
            m_need_create_tetramino = true;
            m_accept_events_count   = 1;
        }

        if (clock.getElapsedTime() < second)
        {
            return;
        }

        if (m_need_create_tetramino)
        {
            Tetramino* tetramino = next_tetramino;
            tetramino->m_is_next = false;
            if (tetramino->check_game_over())
            {
                tetramino->m_is_active = false;
                return;
            }

            tetramino->m_is_active = true;
            tetramino->initialize();
            m_need_create_tetramino = false;
            m_accept_events_count   = 0;

            Tetramino::init_next_tetramino();
            return;
        }

        if (m_is_active && can_move_down())
        {
            move(1, MoveDir::Down);
            clock.restart();
        }

        if (m_blocks.empty())
        {
            delete this;
        }
    }

    Tetramino::~Tetramino()
    {
        printf("DESTROY: %p\n", this);
        if (this == next_tetramino)
        {
            next_tetramino = nullptr;
        }
        while (!m_blocks.empty())
        {
            Block* block = m_blocks.back();
            if (block)
            {
                delete block;
            }
            else
            {
                m_blocks.pop_back();
            }
        }
    }

    void Tetramino::rotate()
    {}

    ///////////////////////////////////////////////////////
    class ITetramino : public Tetramino
    {
    public:
        Position next_position(Block* block)
        {
            int index         = block->index();
            Position pos      = block->position();
            bool use_vertical = rotation_angle == 0 || rotation_angle == 180;


            Position pos_offset;

            switch (index)
            {
                case 0:
                    pos_offset.y = -2;
                    pos_offset.x = 2;
                    break;
                case 1:
                    pos_offset.y = -1;
                    pos_offset.x = 1;
                    break;

                case 3:
                    pos_offset.y = 1;
                    pos_offset.x = -1;
                    break;
                default:
                    break;
            }

            if (use_vertical)
            {
                return pos + pos_offset;
            }
            else
            {
                return pos - pos_offset;
            }
        }


        ITetramino()
        {
            //  0 1 2 3

            Position pos = {0, 0};
            for (int i = 0; i < 4; i++)
            {
                pos.x = 3 + i;
                required_start_blocks.push_back(BlockEntry(pos, i));
            }
        }

        bool can_rotate() override
        {
            for (Block* block : m_blocks)
            {
                Position next_pos = next_position(block);
                if (!is_valid_position(next_pos))
                {
                    return false;
                }
            }
            return true;
        }

        void rotate() override
        {
            for (Block* block : m_blocks)
            {
                Position next_pos = next_position(block);
                block->position(next_pos);
            }
        }
    };


    class CubeTetramino : public Tetramino
    {
    public:
        CubeTetramino()
        {
            Position pos = {0, 0};
            for (int i = 0; i < 2; i++)
            {
                for (int j = 0; j < 2; j++)
                {
                    pos.x = 4 + i;
                    pos.y = j;
                    required_start_blocks.push_back(BlockEntry(pos, i * 2 + j));
                }
            }
        }

        void rotate() override
        {
            Game::instance()->play_not_sound();
        }
    };

    class TTetramino : public Tetramino
    {

    public:
        Position next_position(Block* block)
        {
            static sf::Vector2i pos_offset[4] = {{1, -1}, {1, 1}, {-1, 1}, {-1, -1}};
            int index                         = rotation_angle / 90;
            Position pos                      = block->position();
            pos.x += pos_offset[index].x;
            pos.y += pos_offset[index].y;
            return pos;
        }

        bool can_rotate() override
        {
            Position pos = next_position(m_blocks[2]);

            //            if (!is_valid_position(pos))
            //                Game::instance()->play_not_sound();

            return is_valid_position(pos);
        }

        TTetramino()
        {
            Position pos = {0, 0};
            for (int i = 0; i < 3; i++)
            {
                pos.x = 4 + i;
                required_start_blocks.push_back(BlockEntry(pos, i + 2 > 3 ? 0 : i + 2));
            }

            required_start_blocks.push_back(BlockEntry({5, 1}, 1));
        }

        Position recursive_rotate(Block* block)
        {
            Position next;
            if (block->index() < 2)
            {
                next = recursive_rotate(m_blocks[block->index() + 1]);
            }
            else
            {
                next = next_position(block);
            }

            Position current = block->position();
            block->position(next);
            return current;
        }

        void rotate() override
        {
            recursive_rotate(m_blocks[0]);
        }
    };

    class JTetramino : public Tetramino
    {
    public:
        Position next_position(Block* block)
        {
            int index    = block->index();
            Position pos = block->position();
            if (index == 1)
            {
                return pos;
            }

            bool use_vertical = rotation_angle == 0 || rotation_angle == 180;


            Position pos_offset;

            switch (index)
            {
                case 0:
                    pos_offset.y = 1;
                    pos_offset.x = 1;
                    break;
                case 2:
                    pos_offset.y = -1;
                    pos_offset.x = -1;
                    break;
                case 3:
                    if (rotation_angle == 0)
                        pos_offset.y = -2;
                    else if (rotation_angle == 90)
                        pos_offset.x = 2;
                    else if (rotation_angle == 180)
                        pos_offset.y = 2;
                    else
                        pos_offset.x = -2;

                    return pos + pos_offset;
                    break;

                default:
                    break;
            }

            if (use_vertical)
            {
                return pos + pos_offset;
            }
            else
            {
                return pos - pos_offset;
            }
        }

        JTetramino()
        {
            Position pos = {5, 0};
            for (int i = 0; i < 3; i++)
            {
                pos.y = i;
                required_start_blocks.push_back(BlockEntry(pos, i));
            }

            required_start_blocks.push_back(BlockEntry({4, 2}, 3));
        }

        bool can_rotate() override
        {
            return is_valid_position(next_position(m_blocks[0])) && is_valid_position(next_position(m_blocks[2])) &&
                   is_valid_position(next_position(m_blocks[3]));
        }

        void rotate() override
        {
            for (auto i : m_blocks)
            {
                i->position(next_position(i));
            }
        }
    };

    class LTetramino : public Tetramino
    {
    public:
        Position next_position(Block* block)
        {
            int index    = block->index();
            Position pos = block->position();
            if (index == 1)
            {
                return pos;
            }

            bool use_vertical = rotation_angle == 0 || rotation_angle == 180;


            Position pos_offset;

            switch (index)
            {
                case 0:
                    pos_offset.y = 1;
                    pos_offset.x = 1;
                    break;
                case 2:
                    pos_offset.y = -1;
                    pos_offset.x = -1;
                    break;
                case 3:
                    if (rotation_angle == 0)
                        pos_offset.x = -2;
                    else if (rotation_angle == 90)
                        pos_offset.y = -2;
                    else if (rotation_angle == 180)
                        pos_offset.x = 2;
                    else
                        pos_offset.y = 2;

                    return pos + pos_offset;
                    break;

                default:
                    break;
            }

            if (use_vertical)
            {
                return pos + pos_offset;
            }
            else
            {
                return pos - pos_offset;
            }
        }

        LTetramino()
        {
            Position pos = {5, 0};
            for (int i = 0; i < 3; i++)
            {
                pos.y = i;
                required_start_blocks.push_back(BlockEntry(pos, i));
            }

            required_start_blocks.push_back(BlockEntry({6, 2}, 3));
        }

        bool can_rotate() override
        {
            return is_valid_position(next_position(m_blocks[0])) && is_valid_position(next_position(m_blocks[2])) &&
                   is_valid_position(next_position(m_blocks[3]));
        }

        void rotate() override
        {
            for (auto i : m_blocks)
            {
                i->position(next_position(i));
            }
        }
    };

    class STetramino : public Tetramino
    {
    public:
        Position next_position(Block* block)
        {
            int index    = block->index();
            Position pos = block->position();
            if (index < 2)
            {
                return pos;
            }

            bool use_horisontal = rotation_angle == 0 || rotation_angle == 180;


            Position pos_offset;

            switch (index)
            {
                case 2:
                    pos_offset.y = -2;
                    pos_offset.x = 1;
                    break;
                case 3:
                    pos_offset.y = 0;
                    pos_offset.x = 1;
                    break;
                default:
                    break;
            }

            if (use_horisontal)
            {
                return pos + pos_offset;
            }
            else
            {
                return pos - pos_offset;
            }
        }


        STetramino()
        {
            Position pos = {0, 0};
            for (int i = 0; i < 2; i++)
            {
                pos.x = 5 + i;
                required_start_blocks.push_back(BlockEntry(pos, i));
            }

            pos.y = 1;
            for (int i = 0; i < 2; i++)
            {
                pos.x = 4 + i;
                required_start_blocks.push_back(BlockEntry(pos, i + 2));
            }
        }

        bool can_rotate() override
        {
            return is_valid_position(next_position(m_blocks[3])) && is_valid_position(next_position(m_blocks[2]));
        }

        void rotate() override
        {
            for (size_t i = 2; i < 4; i++)
            {
                m_blocks[i]->position(next_position(m_blocks[i]));
            }
        }
    };

    class ZTetramino : public Tetramino
    {
    public:
        Position next_position(Block* block)
        {
            int index    = block->index();
            Position pos = block->position();
            if (index == 1 || index == 2)
            {
                return pos;
            }

            bool use_horisontal = rotation_angle == 0 || rotation_angle == 180;


            Position pos_offset;

            switch (index)
            {
                case 0:
                    pos_offset.y = -1;
                    pos_offset.x = 2;
                    break;
                case 3:
                    pos_offset.y = -1;
                    pos_offset.x = 0;
                    break;
                default:
                    break;
            }

            if (use_horisontal)
            {
                return pos + pos_offset;
            }
            else
            {
                return pos - pos_offset;
            }
        }

        ZTetramino()
        {
            Position pos = {0, 0};
            for (int i = 0; i < 2; i++)
            {
                pos.x = 4 + i;
                required_start_blocks.push_back(BlockEntry(pos, i));
            }

            pos.y = 1;
            for (int i = 0; i < 2; i++)
            {
                pos.x = 5 + i;
                required_start_blocks.push_back(BlockEntry(pos, i + 2));
            }
        }

        bool can_rotate() override
        {
            return is_valid_position(next_position(m_blocks[0])) && is_valid_position(next_position(m_blocks[3]));
        }

        void rotate() override
        {
            m_blocks[0]->position(next_position(m_blocks[0]));
            m_blocks[3]->position(next_position(m_blocks[3]));
        }
    };

    std::vector<Tetramino* (*) ()> Tetramino::m_constructors = {
            []() -> Tetramino* { return new ITetramino(); },   //
            []() -> Tetramino* { return new CubeTetramino(); },//
            []() -> Tetramino* { return new TTetramino(); },//
            []() -> Tetramino* { return new JTetramino(); },   //
            []() -> Tetramino* { return new LTetramino(); },   //
            []() -> Tetramino* { return new STetramino(); },//
            []() -> Tetramino* { return new ZTetramino(); },
    };
}// namespace Tetris
