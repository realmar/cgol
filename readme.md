# cgol

Convay's Game of Life programmed in C using SDL2 for rendering.

## Controls
```
t: toggle simulation (pause/continue)
c: clear screen
r: reset/randomize cells
a: simulation speed up
s: simulation speed down

Left Mouse: Revive Cell
Right Mouse: Kill cell
```

## Compilation
First SDL2 is needed on your machine:

On Windows, the easiest way is using vcpkg. On other systems your package manager might already provide an sdl2 package. If it doesn't work you should be able to use the vcpkg method too.
```
$ vcpkg install sdl2

# then you can use cmake to generate the build files using vcpkg as your toolchain
$ cmake -DCMAKE_TOOLCHAIN_FILE=[PATH_TO_VCPKG_TOOLCHAIN] CMakeLists.txt