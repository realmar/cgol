# cgol

Convay's Game of Life programmed in C using SDL2 for rendering.

## Compilation
First SDL2 is needed on your machine:

On Windows, the easiest way is using vcpkg. On other systems your package manager might already provide an sdl2 package. If it doesn't work you should be able use the vcpkg method too.
```
$ vcpkg install sdl2

# don't forget to use vcpkg as your cmake toolchain:
$ cmake -DCMAKE_TOOLCHAIN_FILE=[PATH_TO_VCPKG_TOOLCHAIN]

# then you can use cmake to generate the build files
$ cmake CmakeLists.txt