#!/bin/bash
eval cc src/main.c $(pkg-config --libs --cflags raylib) -o build/game
./build/game
