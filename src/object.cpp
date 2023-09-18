#include <object.hpp>
#include <list>

namespace Tetris
{
    Object::ObjectsSet Object::m_objects;
    Priority Object::m_max_priority;

    const Object::ObjectsSet& Object::objects()
    {
        return m_objects;
    }

    Priority Object::max_priority()
    {
        return m_max_priority;
    }

    void Object::clean_tetraminos()
    {
        std::list<Object*> tetraminos;
        for(Object* object : m_objects)
        {
            if(object->is_tetramino())
            {
                tetraminos.push_back(object);
            }
        }

        for(Object* tetramino : tetraminos)
        {
            delete tetramino;
        }
    }

    Priority Object::render_priority() const
    {
        return m_render_priority;
    }

    void Object::render_priority(Priority priority)
    {
        m_render_priority = priority;
        if (priority > m_max_priority)
        {
            m_max_priority = priority;
        }
    }

    bool Object::is_tetramino() const
    {
        return m_is_tetramino;
    }

    Object::Object()
    {
        m_objects.insert(this);
    }

    void Object::update_all()
    {
        for (Object* object : Object::objects())
        {
            object->update();
        }
    }

//    void Object::set_update_status(bool active)
//    {
//        m_update_status = active;
//    }

    void Object::update()
    {}

    void Object::render(sf::RenderWindow& window)
    {}

    void Object::process_event(const sf::Event& event)
    {}

    Object::~Object()
    {
        m_objects.erase(this);
    }
}// namespace Tetris
