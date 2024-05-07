#include "input.h"

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

}

