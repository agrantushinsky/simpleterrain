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

        game_render();

        glfwSwapBuffers(game->window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
 
