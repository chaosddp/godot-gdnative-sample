#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <Godot.hpp>
#include <Node.hpp>
#include <Area2D.hpp>
#include <AnimatedSprite.hpp>
#include <CollisionShape2D.hpp>

#include <Object.hpp>
#include <Input.hpp>

// base input event
#include <InputEvent.hpp>

// specify input events
#include <InputEventMouseMotion.hpp>
#include <InputEventMouseButton.hpp>

// constants, such as key code
#include <GlobalConstants.hpp>

using namespace godot;

class Player : public Area2D
{
    GODOT_CLASS(Player, Area2D);

private:
    // properties exposed to editor
    int _speed;

    // normal properties
    Size2 _screen_size;

    AnimatedSprite* _animated_sprite = nullptr;
    CollisionShape2D* _collition_shape = nullptr;

public:
    Player();
    ~Player(); // Resource collecting if need

    static void _register_methods();

    // Automatically called by godot.
    // Do not need to register
    void _init();

    // Method that same as notifinition methods in gdscript,
    // Do need register first, so godot can call them
    void _ready();
    void _process(float delta);
    void _input(Ref<InputEvent> event);


    // signal callbacks
    void _on_player_body_entered(Variant body);


    //
    void start(Vector2 pos);
};

#endif