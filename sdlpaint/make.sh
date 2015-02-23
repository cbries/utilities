#!/bin/bash
#
#g++ -O3 `sdl-config --cflags` `sdl-config --libs` -lSDL_gfx -lSDL_image -o paint *.cpp
#/usr/local/cross-tools/bin/i386-mingw32-g++ -mwindows -O3 `sdl-config --cflags` `sdl-config --libs` -lSDL_gfx -lSDL_image -o paint *.cpp

g++ -O3 `sdl-config --cflags` *.cpp -o sdlpaint `sdl-config --libs` -lSDL_gfx -lSDL_image
