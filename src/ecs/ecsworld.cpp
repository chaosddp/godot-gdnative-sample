#include "ecsworld.h"

void EcsWorld::_register_methods()
{
    register_method("_ready", &EcsWorld::_ready);
    register_method("_process", &EcsWorld::_process);
}

EcsWorld::EcsWorld()
{

}

EcsWorld::~EcsWorld()
{

}

void EcsWorld::_init()
{

}

void EcsWorld::_ready()
{
}

void EcsWorld::_process(float delta)
{
    // call systems
    for(auto& fn : _system_list)
    {
        fn(&_registry, delta);
    }
}

// Entity* EcsWorld::create(String name)
// {
//     auto* entity = Entity::_new();

//     entity->set_name(name);

//     return entity;
// }