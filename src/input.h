#ifndef INPUT_H
#define INPUT_H

#include "shared.h"

// TODO: This number is completely random :)
#define MAX_KEYS 512
extern bool held_keys[MAX_KEYS];

void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);

#endif

