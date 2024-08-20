#ifndef GAME_H
#define GAME_H

#include "shared.h"
#include "camera.h"
#include "world.h"

struct Game {
    GLFWwindow* window;
    int window_width, window_height;

    float vertical_fov;
    Camera camera;

    World world;
};

extern struct Game* game;

bool game_init();

void game_destroy();

void game_update();

void game_render();

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

#endif

