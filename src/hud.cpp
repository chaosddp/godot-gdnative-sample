#include "hud.h"

void HUD::_register_methods()
{
    register_signal<HUD>("start_game", Dictionary());

    register_method("_ready", &HUD::_ready);
    register_method("_on_message_timer_timeout", &HUD::_on_message_timer_timeout);
    register_method("_on_show_start_button", &HUD::_on_show_start_button);
    register_method("_on_start_button_pressed", &HUD::_on_start_button_pressed);
    register_method("hide_message", &HUD::hide_message);
}

HUD::HUD()
{}

HUD::~HUD()
{
    _start_button = nullptr;
    _message_label = nullptr;
    _score_label = nullptr;
    _message_timer = nullptr;
}

void HUD::_init()
{

}

void HUD::_ready()
{
    _start_button = get_node<Button>("StartButton");
    _message_label = get_node<Label>("Message");
    _score_label = get_node<Label>("ScoreLabel");
    _message_timer = get_node<Timer>("MessageTimer");

    _message_timer->connect("timeout", this, "_on_message_timer_timeout");
    _start_button->connect("pressed", this, "_on_start_button_pressed");
}


void HUD::show_game_over()
{
    show_message("Game Over");

    // we have to yield in c++, so we split it into 2 parts will timeout callback
    auto timer = get_tree()->create_timer(3);

    timer->connect("timeout", this, "_on_show_start_button");
}

void HUD::show_message(String text)
{
    _message_label->set_text(text);
    _message_label->show();
    _message_timer->start();
}

void HUD::update_score(int score)
{
    _score_label->set_text(String::num_int64(score));
}

void HUD::_on_message_timer_timeout()
{
    _message_label->set_text("Dodge the\nCreeps!");
    
    auto timer = get_tree()->create_timer(1);

    timer->connect("timeout", this, "hide_message");
}

void HUD::_on_show_start_button()
{
    _start_button->show();
}

void HUD::_on_start_button_pressed()
{
    _start_button->hide();
    emit_signal("start_game");
}

void HUD::hide_message()
{
    _message_label->hide();
}