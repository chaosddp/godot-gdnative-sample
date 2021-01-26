#include "gdimgui.h"

int GDImGui::texture_id = 1000;


inline ImVec4 to_imgui_color(Color &color)
{
    return ImVec4(color.r, color.g, color.b, color.a);
}

inline ImVec2 to_imgui_vec2(Vector2 &v)
{
    return ImVec2(v.x, v.y);
}

inline Color to_godot_color(ImVec4 c)
{
    return Color(c.x, c.y, c.z, c.w);
}

inline Color to_godot_color(unsigned char r, unsigned char g, unsigned char b, unsigned a)
{
    return Color(r / 255, g / 255, b / 255, a / 255);
}


void GDImGui::_register_methods()
{
    register_method("_process", &GDImGui::_process);
    register_method("_input", &GDImGui::_input);

    register_property("show demo", &GDImGui::_is_show_demo, false);
}


GDImGui::GDImGui()
{

}

GDImGui::~GDImGui()
{
    ImGui::DestroyContext(_imgui_ctx);
    _imgui_ctx = nullptr;

    _textures.clear();
}

void GDImGui::_init()
{
    // init imgui
    IMGUI_CHECKVERSION();

    // TODO: share font between instance?
    _imgui_ctx = ImGui::CreateContext();

    ImGui::SetCurrentContext(_imgui_ctx);

    gen_font_tex();

    init_imgui();
}


void GDImGui::_input(Ref<InputEvent> event)
{
    ImGuiIO &io = ImGui::GetIO();

    InputEventMouseMotion *motion_evt = cast_to<InputEventMouseMotion>(event.ptr());

    SceneTree *tree = get_tree();

    if (motion_evt != nullptr)
    {
        io.MousePos = to_imgui_vec2(motion_evt->get_position());

        if (io.WantCaptureMouse)
        {
            tree->set_input_as_handled();
        }

        return;
    }

    InputEventMouseButton *mouse_btn_event = cast_to<InputEventMouseButton>(event.ptr());

    if (mouse_btn_event != nullptr)
    {
        switch (mouse_btn_event->get_button_index())
        {
        case GlobalConstants::BUTTON_LEFT: // left button
        {
            io.MouseDown[ImGuiMouseButton_Left] = mouse_btn_event->is_pressed();
            break;
        }
        case GlobalConstants::BUTTON_RIGHT: // right button
        {
            io.MouseDown[ImGuiMouseButton_Right] = mouse_btn_event->is_pressed();
            break;
        }
        case GlobalConstants::BUTTON_MIDDLE: // middle button
        {
            io.MouseDown[ImGuiMouseButton_Middle] = mouse_btn_event->is_pressed();
            break;
        }
        case GlobalConstants::BUTTON_WHEEL_UP: // wheel up
        {
            io.MouseWheel = mouse_btn_event->get_factor() * 1.0f;
            break;
        }
        case GlobalConstants::BUTTON_WHEEL_DOWN: // wheel down
        {
            io.MouseWheel = mouse_btn_event->get_factor() * -1.0f;
            break;
        }
        case GlobalConstants::BUTTON_WHEEL_LEFT: // wheel left
        {
            io.MouseWheel = mouse_btn_event->get_factor() * -1.0f;
            break;
        }
        case GlobalConstants::BUTTON_WHEEL_RIGHT: // wheel right
        {
            io.MouseWheel = mouse_btn_event->get_factor() * 1.0f;
            break;
        }
        default:
            break;
        }

        if (io.WantCaptureMouse)
        {
            tree->set_input_as_handled();
        }

        return;
    }

    InputEventKey *key_event = cast_to<InputEventKey>(event.ptr());

    if (key_event != nullptr)
    {
        int key = 0;
        int64_t lkey = key_event->get_scancode();

        if (lkey < 256)
        {
            key = (int)lkey;
        }
        else
        {
            key = 256 + (int)((unsigned int)lkey & 0xFF);
        }

        io.KeyShift = key_event->get_shift();
        io.KeyAlt = key_event->get_alt();
        io.KeyCtrl = key_event->get_control();
        io.KeySuper = key_event->get_command();

        io.KeysDown[key] = key_event->is_pressed();

        if (key_event->is_pressed())
        {
            io.AddInputCharacter(key_event->get_unicode());
        }

        if (io.WantCaptureKeyboard || io.WantTextInput)
        {
            tree->set_input_as_handled();
        }

        return;
    }
}

void GDImGui::_process(float delta)
{
    ImGuiIO &io = ImGui::GetIO();

    io.DisplaySize = to_imgui_vec2(get_tree()->get_root()->get_size());

    if(delta > 0)
        io.DeltaTime = delta;

    if(_is_show_demo)
    {
        show_demo_window();
    }

    render();
}

void GDImGui::render()
{
    // https://github.com/pkdawson/imgui-godot/blob/master/addons/imgui-godot/ImGuiGD.cs

    if (!_imgui_ctx->WithinFrameScope || !is_visible())
    {
        return;
    }

    ImGui::Render();

    ImDrawData *draw_data = ImGui::GetDrawData();

    int node_number = 0;

    for (auto i = 0; i < draw_data->CmdListsCount; i++)
    {
        node_number += draw_data->CmdLists[i]->CmdBuffer.size();
    }

    while (get_child_count() < node_number)
    {
        MeshInstance2D *meshinst = MeshInstance2D::_new();
        // meshinst->set_texture(_font_tex);

        add_child(meshinst, true);
    }

    while (get_child_count() > node_number)
    {
        Node *node = get_child(get_child_count() - 1);

        remove_child(node);
    }

    int nodeN = 0;
    // VisualServer *vs = VisualServer::get_singleton();

    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        ImDrawList *cmdList = draw_data->CmdLists[n];

        int idxOffset = 0;

        int nVert = cmdList->VtxBuffer.Size;

        PoolVector2Array vertices;
        PoolColorArray colors;
        PoolVector2Array uvs;

        for (int i = 0; i < cmdList->VtxBuffer.Size; i++)
        {
            ImDrawVert vert = cmdList->VtxBuffer[i];

            vertices.append(Vector2(vert.pos.x, vert.pos.y));

            ImVec4 fc = ImGui::ColorConvertU32ToFloat4(vert.col);

            colors.append(Color(fc.x, fc.y, fc.z, fc.w));

            uvs.append(Vector2(vert.uv.x, vert.uv.y));
        }

        for (int cmdi = 0; cmdi < cmdList->CmdBuffer.Size; cmdi++, nodeN++)
        {
            ImDrawCmd &draw_cmd = cmdList->CmdBuffer[cmdi];

            PoolIntArray indices;

            for (int i = idxOffset, j = 0; i < idxOffset + draw_cmd.ElemCount; i++, j++)
            {
                indices.append(cmdList->IdxBuffer[i]);
            }

            MeshInstance2D *child = cast_to<MeshInstance2D>(get_child(nodeN));

            Ref<ArrayMesh> mesh = ArrayMesh::_new();

            Array arrays;

            arrays.resize(ArrayMesh::ArrayType::ARRAY_MAX);
            arrays[ArrayMesh::ArrayType::ARRAY_VERTEX] = vertices;
            arrays[ArrayMesh::ArrayType::ARRAY_COLOR] = colors;
            arrays[ArrayMesh::ArrayType::ARRAY_TEX_UV] = uvs;
            arrays[ArrayMesh::ArrayType::ARRAY_INDEX] = indices;

            mesh->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, arrays);

            child->set_mesh(mesh);

            Texture *tex = get_texture(draw_cmd.TextureId);

            if (tex != nullptr)
            {
                child->set_texture(tex);
            }

            // vs->canvas_item_set_clip(child->get_canvas_item(), true);
            // vs->canvas_item_set_custom_rect(child->get_canvas_item(), true, Rect2(draw_cmd.ClipRect.x, draw_cmd.ClipRect.y, draw_cmd.ClipRect.z - draw_cmd.ClipRect.x, draw_cmd.ClipRect.w - draw_cmd.ClipRect.y));

            idxOffset += draw_cmd.ElemCount;
        }
    }
}

void GDImGui::gen_font_tex()
{
    unsigned char *tex_pixels = nullptr;
    int tex_w, tex_h;
    ImGuiIO &io = ImGui::GetIO();

    io.Fonts->GetTexDataAsRGBA32(&tex_pixels, &tex_w, &tex_h);

    Ref<Image> img = Image::_new();

    img->create(tex_w, tex_h, false, Image::Format::FORMAT_RGBA8);

    img->lock();
    for (int r = 0; r < tex_w; r++)
    {
        for (int c = 0; c < tex_h; c++)
        {
            int index = c * tex_w * 4 + r * 4;
            img->set_pixel(r, c, Color::Color(tex_pixels[index] / 255, tex_pixels[index + 1] / 255, tex_pixels[index + 2] / 255, tex_pixels[index + 3] / 255));
        }
    }
    img->unlock();

    ImageTexture *img_tex = ImageTexture::_new();
    img_tex->create_from_image(img, 0);

    int tex_id = add_texture(img_tex);

    io.Fonts->SetTexID(tex_id);
    io.Fonts->ClearTexData();
}

int GDImGui::add_texture(Texture *tex)
{
    int id = texture_id++;

    _textures[id] = tex;

    return id;
}

Texture *GDImGui::get_texture(int id)
{
    auto pair = _textures.find(id);

    if (pair == _textures.end())
    {
        return nullptr;
    }

    return pair->second;
}

void GDImGui::remove_texture(int id)
{
    _textures.erase(id);
}

void GDImGui::init_imgui()
{
    ImGuiIO &io = ImGui::GetIO();

    ImGui::StyleColorsDark();

    io.BackendPlatformName = "imgui_impl_godot";

    // we followed the solution from github.com/pkdawson to remap the key code less than 512
    int KEYOFFSET = 256;
    int KEY_TAB = KEYOFFSET + 0x02;
    int KEY_LEFT = KEYOFFSET + 0x0F;
    int KEY_UP = KEYOFFSET + 0x10;
    int KEY_RIGHT = KEYOFFSET + 0x11;
    int KEY_DOWN = KEYOFFSET + 0x12;
    int KEY_PAGEUP = KEYOFFSET + 0x13;
    int KEY_PAGEDOWN = KEYOFFSET + 0x14;
    int KEY_HOME = KEYOFFSET + 0x0D;
    int KEY_END = KEYOFFSET + 0x0E;
    int KEY_INSERT = KEYOFFSET + 0x07;
    int KEY_DELETE = KEYOFFSET + 0x08;
    int KEY_BACKSPACE = KEYOFFSET + 0x04;
    int KEY_ESCAPE = KEYOFFSET + 0x01;
    int KEY_ENTER = KEYOFFSET + 0x05;
    int KEY_KP_ENTER = KEYOFFSET + 0x06;

    int Key_A = 0x0041; // for text edit CTRL+A: select all
    int Key_C = 0x0043; // for text edit CTRL+C: copy
    int Key_V = 0x0056; // for text edit CTRL+V: paste
    int Key_X = 0x0058; // for text edit CTRL+X: cut
    int Key_Y = 0x0059; // for text edit CTRL+Y: redo
    int Key_Z = 0x005A; // for text edit CTRL+Z: undo
    // keyboard mapping
    io.KeyMap[ImGuiKey_Tab] = KEY_TAB;
    io.KeyMap[ImGuiKey_LeftArrow] = KEY_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = KEY_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = KEY_UP;
    io.KeyMap[ImGuiKey_DownArrow] = KEY_DOWN;
    io.KeyMap[ImGuiKey_PageUp] = KEY_PAGEUP;
    io.KeyMap[ImGuiKey_PageDown] = KEY_PAGEDOWN;
    io.KeyMap[ImGuiKey_Home] = KEY_HOME;
    io.KeyMap[ImGuiKey_End] = KEY_END;
    io.KeyMap[ImGuiKey_Insert] = KEY_INSERT;
    io.KeyMap[ImGuiKey_Delete] = KEY_DELETE;
    io.KeyMap[ImGuiKey_Backspace] = KEY_BACKSPACE;
    io.KeyMap[ImGuiKey_Enter] = KEY_ENTER;
    io.KeyMap[ImGuiKey_Escape] = KEY_ESCAPE;
    io.KeyMap[ImGuiKey_KeyPadEnter] = KEY_KP_ENTER;
    io.KeyMap[ImGuiKey_A] = Key_A;
    io.KeyMap[ImGuiKey_C] = Key_C;
    io.KeyMap[ImGuiKey_V] = Key_V;
    io.KeyMap[ImGuiKey_X] = Key_X;
    io.KeyMap[ImGuiKey_Y] = Key_Y;
    io.KeyMap[ImGuiKey_Z] = Key_Z;
}

int GDImGui::add_image(Ref<Texture> tex)
{
    return add_texture(tex.ptr());
}

void GDImGui::set_as_current_context()
{
    ImGui::SetCurrentContext(_imgui_ctx);
}

void GDImGui::show_demo_window()
{
    ImGui::NewFrame();

    ImGui::ShowDemoWindow();
}