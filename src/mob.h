#ifndef _MOB_H_
#define _MOB_H_

#include <Godot.hpp>
#include <RigidBody2D.hpp>
#include <AnimatedSprite.hpp>
#include <CollisionShape2D.hpp>
#include <SpriteFrames.hpp>
#include <VisibilityNotifier2D.hpp>
#include <RandomNumberGenerator.hpp>

using namespace godot;

class Mob : public RigidBody2D
{
    GODOT_CLASS(Mob, RigidBody2D);

private:
    int _min_speed;
    int _max_speed;

    AnimatedSprite* _animated_sprite = nullptr;
    CollisionShape2D* _collision_shape = nullptr;
    VisibilityNotifier2D* _visibility_notifier = nullptr;

    RandomNumberGenerator* _rand = nullptr;
public:
    static void _register_methods();

    Mob();
    ~Mob();

    void _init();
    void _ready();
    void _process(float delta);

    void _on_screen_exited();

    void set_min_speed(int value);
    int get_min_speed();

    void set_max_speed(int value);
    int get_max_speed();
};

#endif