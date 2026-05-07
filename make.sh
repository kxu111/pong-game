#!/bin/bash
eval cc pong.c $(pkg-config --libs --cflags raylib) -o game
./game
