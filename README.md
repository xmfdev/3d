# 3D - fun in three dimensions

A software 3D graphics demo written in C.
Renders a wireframe cube with perspective projection and Y-axis rotation.
Built from scratch on top of [raylib](https://github.com/raysan5/raylib).

## Features

- Perspective projection with perspective divide
- Y-axis rotation
- Wireframe rendering with edge connectivity via face indices

## Requirements

- CMake 3.15+
- A C compiler (GCC or Clang)
- Git (for fetching raylib)

## Building

Clone the repository and run the following commands (assuming pwd is aforementioned).

```sh
cmake -S . -B build
cmake --build build
```

## Running

After building run the command below.

```sh
./build/bin
```

## License

Licensed under the MIT license. See `LICENSE` for more information.
