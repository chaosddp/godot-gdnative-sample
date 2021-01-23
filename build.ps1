cd godot-cpp

# build godot-cpp
scons platform=windows generate_bindings=yes bits=64 target=release -j4

# build lua
scons -f .\lua.scons platform=windows target=release

cd ..

scons platform=windows bits=64 target=release

cp libs/win64/libnativesample.dll game/scripts/win64