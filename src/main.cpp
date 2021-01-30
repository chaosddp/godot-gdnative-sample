#include "main.h"

void Main::_register_methods()
{
    register_property(
        "mob_scene",
        &Main::set_mob_scene,
        &Main::get_mob_scene,
        Ref<PackedScene>());

    //register_method("_process", &Main::_process);
    register_method("_ready", &Main::_ready);
    register_method("game_over", &Main::game_over);
    register_method("new_game", &Main::new_game);
    register_method("_on_start_timer_timeout", &Main::_on_start_timer_timeout);
    register_method("_on_score_timer_timeout", &Main::_on_score_timer_timeout);
    register_method("_on_mob_timer_timeout", &Main::_on_mob_timer_timeout);
}

Main::Main()
{
}

Main::~Main()
{
    _start_timer = nullptr;
    _score_timer = nullptr;
    _mob_timer = nullptr;
    _rand = nullptr;
    _debug_label = nullptr;
}

void Main::_init()
{
    _score = 0;

    lua.open_libraries(sol::lib::base, sol::lib::package);


    lua.script("print('main._init')");

    // register godot String class
    lua.new_usertype<String>(
        "String",
        sol::call_constructor,
        sol::factories([](const char* s){ return String(s); })
    );

    // NOTE: we should  and disable constructing nodes from lua side
    // use a factory function instead, so we can maintain the reference in c++ side.

    // label class
    lua.new_usertype<Label>(
        "Label",
        "new", sol::no_constructor,
        "set_text", &Label::set_text
    );
    
    // current node
    lua.new_usertype<Main>(
        "Main",
        "new", sol::no_constructor,
        "debug_label", &Main::_debug_label
    );

    lua["Main"] = this;
}

void Main::_ready()
{

    _start_timer = get_node<Timer>("StartTimer");
    _score_timer = get_node<Timer>("ScoreTimer");
    _mob_timer = get_node<Timer>("MobTimer");
    _player = get_node<Player>("Player");
    _start_pos = get_node<Position2D>("StartPosition");
    _hud = get_node<HUD>("HUD");

    _mob_spawn_location = get_node<PathFollow2D>("MobPath/MobSpawnLocation");
    _rand = RandomNumberGenerator::_new();

    _rand->randomize();

    _debug_label = get_node<Label>("DebugLabel");

    // NOTE: we have to construct godot:String explicitly
    lua.script("Main.debug_label:set_text(String('Hello, again.'))");
}

void Main::_process(float delta)
{
}

void Main::game_over()
{
    get_node<AudioStreamPlayer2D>("Music")->stop();
    get_node<AudioStreamPlayer2D>("DeathSound")->play();

    _score_timer->stop();
    _mob_timer->stop();

    _hud->show_game_over();
}

void Main::new_game()
{
    get_node<AudioStreamPlayer2D>("Music")->play();

    _score = 0;

    get_tree()->call_group("mobs", "queue_free");

    _player->start(_start_pos->get_position());
    _start_timer->start();

    _hud->update_score(_score);
    _hud->show_message("Get Ready");
}

void Main::_on_score_timer_timeout()
{
    _score += 1;

    _hud->update_score(_score);
}

void Main::_on_start_timer_timeout()
{
    _mob_timer->start();
    _score_timer->start();
}

void Main::_on_mob_timer_timeout()
{
    _mob_spawn_location->set_offset(_rand->randi());

    // NOTE: we shoud make sure it is not empty in real production
    if (_mob_scene.is_valid())
    {
        auto *mob = (Mob *)_mob_scene->instance();

        // NOTE: DO call _init to initialize the new new node, all will variables will not initialized.
        mob->_init();

        add_child(mob);

        auto dir = _mob_spawn_location->get_rotation() + Math_PI / 2;

        mob->set_position(_mob_spawn_location->get_position());

        dir += _rand->randf_range(-Math_PI / 4, Math_PI / 4);
        mob->set_rotation(dir);

        auto velocity = Vector2(_rand->randi_range(mob->get_min_speed(), mob->get_max_speed()), 0);
        mob->set_linear_velocity(velocity.rotated(dir));
    }
    else
    {
        Godot::print("No mob scene asigne!");
    }
}

void Main::set_mob_scene(Ref<PackedScene> mob_scene)
{
    _mob_scene = mob_scene;
}

Ref<PackedScene> Main::get_mob_scene()
{
    return _mob_scene;
}