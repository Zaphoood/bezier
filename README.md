# Bezier curves

Draw bezier curves using nodes.

## Setup

You'll need `gcc` and `make` for compiling, as well as the [SDL2](https://github.com/libsdl-org/SDL) and [SDL2_ttf](https://github.com/libsdl-org/SDL_ttf) libraries.

On Ubuntu, SDL2 and SDL2_ttf can be installed using `apt`:
```shell
sudo apt install libsdl2-2.0-0 libsdl2-dev libsdl2-ttf-2.0-0 libsdl2-ttf-dev
```

Compile by running

```shell
cmake -B./build
cd build
make
```

## Usage

After compiling, run `./build/bezier` to start the application.

You can move nodes (as well as their arms) by clicking and dragging.
To add a new node, use hold control while clicking.
