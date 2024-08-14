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
    const float sensitivity = 0.05f;

    static double last_xpos = 0.0;
    static double last_ypos = 0.0;

    double dx = xpos - last_xpos;
    double dy = ypos - last_ypos;

    last_xpos = xpos;
    last_ypos = ypos;

    game->camera.yaw += dx * sensitivity;
    if(game->camera.yaw > 360 || game->camera.yaw < -360) 
    {
        game->camera.yaw = 0;
    }

    game->camera.pitch = glm_min(glm_max(game->camera.pitch - dy * sensitivity, -89.99f), 89.99f);

    camera_update_view(&game->camera);
}

