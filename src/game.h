#ifndef GAME_H
#define GAME_H

#include "shared.h"
#include "camera.h"

struct Game {
    GLFWwindow* window;
    int window_width, window_height;
    float vertical_fov;
    Camera camera;
};

extern struct Game* game;

bool game_init();

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

#endif

