#include <object.hpp>

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

    Object::Object()
    {
        m_objects.insert(this);
    }

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
