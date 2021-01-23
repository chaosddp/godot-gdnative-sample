cd godot-cpp

scons platform="windows" generate_bindings=yes bits=64 target=release -j4

cd ..

scons platform="windows" bits=64 target=release