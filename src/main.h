#ifndef _MAIN_H_
#define _MAIN_H_

#include <Godot.hpp>
#include <Node2D.hpp>
#include <PackedScene.hpp>
#include <Timer.hpp>
#include <Position2D.hpp>
#include <Path2D.hpp>
#include <PathFollow2D.hpp>
#include <RandomNumberGenerator.hpp>

#include "player.h"
#include "mob.h"
#include "hud.h"


using namespace godot;

class Main : public Node2D
{
    GODOT_CLASS(Main, Node2D);

private:
    int _score;
    Ref<PackedScene> _mob_scene;

    Timer* _score_timer =nullptr;
    Timer* _mob_timer = nullptr;
    Timer* _start_timer = nullptr;

    // NOTE: though we can get the reference of Player from scene,
    // but maybe better to make a abstract class work as interface,
    // then here we only maintain the referent of Node2D.
    // then cast it to our Player interface when we need operations on it
    Player* _player = nullptr;

    Position2D* _start_pos = nullptr;

    PathFollow2D* _mob_spawn_location = nullptr;

    RandomNumberGenerator* _rand = nullptr;

    HUD* _hud = nullptr;

public:
    static void _register_methods();

    Main();
    ~Main();

    void _init();
    void _ready();
    void _process(float delta);

    // callback on player hit
    void game_over();

    void new_game();

    // 
    void _on_start_timer_timeout();
    void _on_score_timer_timeout();
    void _on_mob_timer_timeout();

    void set_mob_scene(Ref<PackedScene> mob_scene);
    Ref<PackedScene> get_mob_scene();
};

#endif