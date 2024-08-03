#include <glad/glad.h>

#include "game.h"
#include "input.h"
#include "shader.h"

struct Game* game;

void APIENTRY debug_callback(GLenum source, GLenum type, GLenum id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam) 
{
    fprintf(stderr, "GL DEBUG:\n%s\n", message);
}

bool game_init()
{
    if(!glfwInit())
    {
        printf("glfwInit failed\n");
        return false;
    }

    game = malloc(sizeof(struct Game));
    if (!game) {
        printf("The world mightve ended\n");
        return false;
    }

    game->window_width = 1280;
    game->window_height = 720;
    game->vertical_fov = 90.0f;

    game->window = glfwCreateWindow(game->window_width, game->window_height, "simpleterrain", NULL, NULL);
    if (!game->window)
    {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(game->window);

    if(!gladLoadGL())
    {
        printf("gladLoadGL failed\n");
        return false;
    }

    // useful: https://www.glfw.org/docs/latest/input_guide.html
    // setup glfw callbacks
    glfwSetFramebufferSizeCallback(game->window, framebuffer_size_callback);
    glfwSetKeyCallback(game->window, keyboard_callback);
    glfwSetCursorPosCallback(game->window, mouse_callback);

    // disable the cursor
    glfwSetInputMode(game->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // disables vsync(??)
    glfwSwapInterval(0);

    // log errors
    glEnable(GL_DEBUG_OUTPUT);
    glad_glDebugMessageCallback((void*)debug_callback, NULL);

    shaders_init();
    camera_init(&game->camera, game->vertical_fov);
    
    // surely everything is working, right?
    return true;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    game->window_width = width;
    game->window_height = height;
    glViewport(0, 0, width, height);
}
