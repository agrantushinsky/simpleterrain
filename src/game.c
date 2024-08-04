#include <glad/glad.h>

#include "game.h"
#include "input.h"
#include "shader.h"
#include "world.h"

struct Game* game;

void APIENTRY debug_callback(GLenum source, GLenum type, GLenum id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam) 
{
    if(severity == GL_DEBUG_SEVERITY_NOTIFICATION) {
        return;
    }
    fprintf(stderr, "GL DEBUG L:%i\n%s\n", severity, message);
}

// TODO: Move this again.
void handle_input() 
{
    const static float speed = 0.0005f;
    float yaw = game->camera.yaw;
    if (held_keys[GLFW_KEY_W]) {
        game->camera.position[0] += speed * cosf(glm_rad(yaw));
        game->camera.position[2] += speed * sinf(glm_rad(yaw));
    }
    if (held_keys[GLFW_KEY_S]) {
        game->camera.position[0] -= speed * cosf(glm_rad(yaw));
        game->camera.position[2] -= speed * sinf(glm_rad(yaw));
    }
    if (held_keys[GLFW_KEY_A]) {
        game->camera.position[0] += speed * sinf(glm_rad(yaw));
        game->camera.position[2] -= speed * cosf(glm_rad(yaw));
    }
    if (held_keys[GLFW_KEY_D]) {
        game->camera.position[0] -= speed * sinf(glm_rad(yaw));
        game->camera.position[2] += speed * cosf(glm_rad(yaw));
    }
    if (held_keys[GLFW_KEY_SPACE]) {
        game->camera.position[1] += speed;
    }
    if (held_keys[GLFW_KEY_LEFT_SHIFT]) {
        game->camera.position[1] -= speed;
    }

    static bool toggle_wireframe = true;
    if(held_keys[GLFW_KEY_V]) {
        held_keys[GLFW_KEY_V] = false; // :)
        if(toggle_wireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        toggle_wireframe = !toggle_wireframe;
    }

    camera_update_view(&game->camera);
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

    // prepare game
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glUseProgram(shader_programs[0]);

    world_init(&game->world);

    // surely everything is working, right?
    return true;
}

void game_update()
{
    handle_input();
    world_update(&game->world);
}

void game_render()
{
    world_render(&game->world);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    game->window_width = width;
    game->window_height = height;
    glViewport(0, 0, width, height);
}
