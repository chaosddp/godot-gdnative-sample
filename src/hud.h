#ifndef _HUD_H_
#define _HUD_H_


#include <Godot.hpp>
#include <CanvasLayer.hpp>
#include <Label.hpp>
#include <Button.hpp>
#include <Timer.hpp>
#include <String>
#include <SceneTree.hpp>
#include <SceneTreeTimer.hpp>
#include <AudioStreamPlayer2D.hpp>

using namespace godot;

class HUD : public CanvasLayer
{
    GODOT_CLASS(HUD, CanvasLayer);

    private:
        Label* _message_label = nullptr;
        Label* _score_label = nullptr;
        Timer* _message_timer = nullptr;
        Button* _start_button = nullptr;

    public:
        static void _register_methods();

        HUD();
        ~HUD();

        void _init();
        void _ready();

        void show_message(String text);
        void show_game_over();
        void update_score(int score);
        void hide_message();

        void _on_start_button_pressed();

        void _on_message_timer_timeout();

        // used as callback of a temp timer
        void _on_show_start_button();
};


#endif