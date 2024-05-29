#include "input.h"
#include "game.h"

bool held_keys[MAX_KEYS];

void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
    
    // there are 3 key states, release, press and repeating 
    // if it's not release, we must be holding down the key
    // TODO: This might catch fire, but I am too lazy to implement anything better atm.
    held_keys[key] = action == GLFW_RELEASE ? false : true;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    static double last_xpos = 0.0;
    static double last_ypos = 0.0;

    double dx = xpos - last_xpos;
    double dy = ypos - last_ypos;

    last_xpos = xpos;
    last_ypos = ypos;

    game->camera.yaw += dx * 0.05f;
    game->camera.pitch -= dy * 0.05f;
    printf("yaw: %f, pitch: %f\n", game->camera.yaw, game->camera.pitch);
    camera_update_view(&game->camera);
}

