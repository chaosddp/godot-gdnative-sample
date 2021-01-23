#include "player.h"

// godot api provide some helpers methods, such as memory allocation.
// usually used for allocate/free memory, such as:
// api->godot_alloc: allocate specified size of memory
// api->godot_free: free memory allocate by godot_alloc, also can free memory for String.alloc_c_string()
// extern const godot_gdnative_core_api_struct *godot::api;

void Player::_register_methods()
{
    // register our properties, so that godot can access it
    register_property(
        "speed",                        // Name shows in editor
        &Player::_speed,                // Variable bind to
        400,                            // default value
        GODOT_METHOD_RPC_MODE_DISABLED, // RPC mode, we do not need it here
        GODOT_PROPERTY_USAGE_DEFAULT,    // Show it in editor
        GODOT_PROPERTY_HINT_RANGE,      // Hint flag in editor
        "0,1000,1");                    // Use hint text to specify hint properties

    // register method to godot to match the notification methods
    register_method("_process", &Player::_process); // same as gdscript _process, called each frame
    register_method("_ready", &Player::_ready);     // gdscript _ready
    //register_method("_input", &Player::_input);     // gdscript _input
    register_method("_on_player_body_entered", &Player::_on_player_body_entered);

    // register signal
    register_signal<Player>("hit", Dictionary());
}

Player::Player()
{
}

Player::~Player()
{
    _animated_sprite = nullptr;
    _collition_shape = nullptr;
}

void Player::_init()
{
    _speed = 400;
}

void Player::_ready()
{
    _screen_size = get_viewport_rect().size;

    _animated_sprite = get_node<AnimatedSprite>("AnimatedSprite");
    _collition_shape = get_node<CollisionShape2D>("CollisionShape2D");

    connect("body_entered", this, "_on_player_body_entered");

    hide();
}

void Player::_process(float delta)
{
    auto velocity = Vector2(0, 0);

    // different with gdscript, we should use Input::get_singleton() to get Input instance.
    auto *input = Input::get_singleton();

    if (input->is_action_pressed("ui_right"))
    {
        velocity.x += 1;
    }

    if (input->is_action_pressed("ui_left"))
    {
        velocity.x -= 1;
    }

    if (input->is_action_pressed("ui_down"))
    {
        velocity.y += 1;
    }

    if (input->is_action_pressed("ui_up"))
    {
        velocity.y -= 1;
    }

    if (velocity.length() > 0)
    {
        // this will not create a new instance of Vector2
        velocity = velocity.normalized() * _speed;

        if (_animated_sprite != nullptr)
            _animated_sprite->play();
    }
    else
    {
        if (_animated_sprite != nullptr)
            _animated_sprite->stop();
    }

    // set position
    auto pos = get_position();
    pos += velocity * delta;

    pos.x = Math::clamp<int>(pos.x, 0, _screen_size.x);
    pos.y = Math::clamp<int>(pos.y, 0, _screen_size.y);

    set_position(pos);

    if (velocity.x != 0)
    {
        _animated_sprite->set_animation("walk");
        _animated_sprite->set_flip_v(false);
        _animated_sprite->set_flip_h(velocity.x < 0);
    }
    else if (velocity.y != 0)
    {
        _animated_sprite->set_animation("up");
        _animated_sprite->set_flip_v(velocity.y > 0);
    }
}

void Player::_input(Ref<InputEvent> event)
{
}

void Player::_on_player_body_entered(Variant body)
{
    Godot::print("player hit");
    hide();
    emit_signal("hit");

    _collition_shape->set_deferred("disabled", true);
}

void Player::start(Vector2 pos)
{
    set_position(pos);
    show();

    _collition_shape->set_disabled(false);
}