# godot-gdnative-sample
Sample to show how to use c++ as script instead of gdscript to implement "Dodge the Creeps!" project from Godot docs [here](https://docs.godotengine.org/en/stable/getting_started/step_by_step/your_first_game.html).


# About this branch

This branch 'mix' used to show how to mix libraries and frameworks I want to use together.


Current state (both in c++ side):

1. bind lua to support scripting: done with lua5.4
2. bind sol2 to provide godot stuff to lua side: done
3. embed imgui for runtime debuging: done
4. entt for ECS design: in progress, not find a good way to use it with current godot nodes.

## How to build

```sh

cd godot-cpp

scons platform=platform generate_bindings=yes bits=64 target=release -j4

cd ..

scons platform=platform bits=64 target=release

```