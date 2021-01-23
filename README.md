# godot-gdnative-sample
Sample to show how to use c++ as script instead of gdscript to implement "Dodge the Creeps!" project from Godot docs [here](https://docs.godotengine.org/en/stable/getting_started/step_by_step/your_first_game.html).



## How to build

```sh

cd godot-cpp

scons platform=platform generate_bindings=yes bits=64 target=release -j4

cd ..

scons platform=platform bits=64 target=release

```