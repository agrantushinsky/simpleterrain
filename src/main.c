#include "game.h"
#include "shared.h"


int main(void)
{
    // if we fail to setup, return failure.
    if(!game_init())
        return -1;

    while (!glfwWindowShouldClose(game->window))
    {
        game_update();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.11f, 0.54f, 0.76f, 1.0f);

        game_render();

        glfwSwapBuffers(game->window);
        glfwPollEvents();
    }

    game_destroy();

    glfwTerminate();
    return 0;
}
 
