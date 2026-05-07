#!/bin/bash
eval cc src/pong.c $(pkg-config --libs --cflags raylib) -o game
./game
