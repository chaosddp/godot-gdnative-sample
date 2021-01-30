#include "entity.h"

void Entity::_register_methods()
{
    register_method("_ready", &Entity::_ready);
    register_method("_init", &Entity::_init);
}

Entity::Entity()
{
}

Entity::~Entity()
{
}

void Entity::_init()
{
}

void Entity::_ready()
{
    _world = get_node<EcsWorld>("/root/EscWorld");
}

template <typename T, typename... Args>
T &Entity::add_component(Args &&... args)
{
    return _world->_registry.emplace<T>(_id, std::forward(args)...);
}

template <typename T>
T &Entity::get_component()
{
    return _world->_registry.get<T>(_id);
}

template <typename T>
void Entity::remove_component()
{
    _world->registry.remove<T>(_id);
}

template <typename T>
bool Entity::has_component()
{
    return _world->registry.has<T>(_id);
}