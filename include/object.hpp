#pragma once
#include <SFML/Graphics.hpp>
#include <set>

namespace Tetris
{

    using Priority = unsigned int;

    class Object
    {
    public:
        using ObjectsSet = std::set<Object*>;

    private:
        static ObjectsSet m_objects;
        Priority m_render_priority = 0;
        static Priority m_max_priority;

    protected:
        bool m_is_tetramino = false;

    public:
        static const ObjectsSet& objects();
        static Priority max_priority();
        static void clean_tetraminos();

        Priority render_priority() const;
        void render_priority(Priority priority);
        bool is_tetramino() const;

        Object();

        virtual void update();
        void update_all();
        virtual void render(sf::RenderWindow& window);
        virtual void process_event(const sf::Event& event);

        virtual ~Object();
    };
}// namespace Tetris
