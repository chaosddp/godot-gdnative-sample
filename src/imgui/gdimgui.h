#ifndef _GDIMGUI_H_
#define _GDIMGUI_H_

#include <Godot.hpp>
#include <CanvasItem.hpp>
#include <Image.hpp>
#include <ImageTexture.hpp>
#include <ArrayMesh.hpp>
#include <MeshInstance2D.hpp>
#include <VisualServer.hpp>
#include <Resource.hpp>
#include <Node2D.hpp>
#include <Input.hpp>
#include <InputEvent.hpp>
#include <SceneTree.hpp>
#include <InputEventMouseMotion.hpp>
#include <InputEventMouseButton.hpp>
#include <InputEventKey.hpp>
#include <Viewport.hpp>
#include <GlobalConstants.hpp>
#include <Texture.hpp>
#include <GodotGlobal.hpp>

#include <unordered_map>

#include "imgui.h"
#include "imgui_internal.h"

using namespace godot;

class GDImGui : public Node2D
{
    GODOT_CLASS(GDImGui, Node2D);

    // imgui context for current instance
    ImGuiContext *_imgui_ctx;

    // global texture id for new added images
    static int texture_id;

    bool _is_show_demo{ false };

    // current avaiable customized images
    std::unordered_map<int, Texture *> _textures;

    void gen_font_tex();
    void init_imgui();

    // custom image management
    int add_texture(Texture *tex);
    Texture *get_texture(int id);
    void remove_texture(int id);

public:
    static void _register_methods();

    GDImGui();
    ~GDImGui();

    void _init(); // our initializer called by Godot

    void _input(Ref<InputEvent> event);

    void _process(float delta);

    void render();

    // helpers
    // add image and get an id for following usage, like imgui.image, imgui.image_button
    int add_image(Ref<Texture> tex);

    // Set context of this instance as current context for imgui
    void set_as_current_context();

    void show_demo_window();
};

#endif