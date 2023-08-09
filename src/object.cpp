#include <object.hpp>

std::set<Object*> Object::m_objects;

Object::Object()
{
    m_objects.insert(this);
}

void Object::update()
{}

void Object::render(sf::RenderWindow& window)
{

}

void Object::process_event(const sf::Event& event)
{

}

const std::set<Object*>& Object::objects()
{
    return m_objects;
}

void Object::destroy_all()
{
    for(Object* object : objects())
    {
        delete object;
    }
}

Object::~Object()
{
    m_objects.erase(this);
}
