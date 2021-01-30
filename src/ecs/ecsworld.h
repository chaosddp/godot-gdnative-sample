#ifndef _ECS_ECS_SCENE_H_
#define _ECS_ECS_SCENE_H_

#include <functional>
#include <vector>

#include <Godot.hpp>
#include <Node.hpp>
#include <String.hpp>

#include "entt.hpp"

class Entity;

using namespace godot;

class EcsWorld : public Node
{
    GODOT_CLASS(EcsWorld, Node);

private:
    friend Entity;
 
    entt::registry _registry;

    std::vector<std::function<void(entt::registry *, float)>> _system_list;

public:
    static void _register_methods();

    EcsWorld();
    ~EcsWorld();

    void _init();
    void _ready();
    void _process(float delta);

    void add_system(std::function<void(entt::registry *, float)> sys_fn);
};

#endif