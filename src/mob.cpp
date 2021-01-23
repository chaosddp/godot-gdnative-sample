#include "mob.h"

void Mob::_register_methods()
{
    register_property(
        "min_speed",
        &Mob::set_min_speed,
        &Mob::get_min_speed,
        150);

    register_property(
        "max_speed",
        &Mob::set_max_speed,
        &Mob::get_max_speed,
        250);

    register_method("_ready", &Mob::_ready);
    register_method("_process", &Mob::_process);

    // we should register first, or godot cannot find the callback
    register_method("_on_screen_exited", &Mob::_on_screen_exited);
}

Mob::Mob()
{
    _rand = RandomNumberGenerator::_new();
}

Mob::~Mob()
{
    _animated_sprite = nullptr;
    _collision_shape = nullptr;
    _visibility_notifier = nullptr;
    _rand = nullptr;
}

void Mob::_init()
{
    _min_speed = 150;
    _max_speed = 250;
}

void Mob::_ready()
{
    _animated_sprite = get_node<AnimatedSprite>("AnimatedSprite");
    _collision_shape = get_node<CollisionShape2D>("CollisionShape2D");
    _visibility_notifier = get_node<VisibilityNotifier2D>("VisibilityNotifier2D");

    if (_animated_sprite != nullptr)
    {
        _animated_sprite->_set_playing(true);

        auto frames = _animated_sprite->get_sprite_frames();
        auto mob_types = frames->get_animation_names();

        _animated_sprite->set_animation(mob_types[_rand->randi() % mob_types.size()]);
    }

    if (_visibility_notifier != nullptr)
    {
        _visibility_notifier->connect("screen_exited", this, "_on_screen_exited");
    }
}

void Mob::_process(float delta)
{
}

void Mob::_on_screen_exited()
{
    queue_free();
}

void Mob::set_min_speed(int value)
{
    _min_speed = value;
}
int Mob::get_min_speed()
{
    return _min_speed;
}

void Mob::set_max_speed(int value)
{
    _max_speed = value;
}
int Mob::get_max_speed()
{
    return _max_speed;
}