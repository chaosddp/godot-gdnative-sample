#ifndef _ECS_ENTITY_H_
#define _ECS_ENTITY_H_

#include <Godot.hpp>
#include <Node.hpp>
#include <NodePath.hpp>

#include "ecsworld.h"
#include "entt.hpp"

using namespace godot;

class Entity : public Node
{
    GODOT_CLASS(Entity, Node);

private:
    entt::entity _id {entt::null};
    EcsWorld *_world {nullptr};

public:
    static void _register_methods();

    Entity();
    ~Entity();

    void _init();
    void _ready();

    template <typename T, typename... Args>
    T &add_component(Args &&... args);

    template <typename T>
    T &get_component();

    template <typename T>
    void remove_component();

    template <typename T>
    bool has_component();
};

#endif