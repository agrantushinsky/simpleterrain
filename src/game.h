#ifndef GAME_H
#define GAME_H

#include "shared.h"

struct Game {
    GLFWwindow* window;
    int window_width, window_height;
    float vertical_fov;
};

extern struct Game* game;

bool game_init();

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

#endif

