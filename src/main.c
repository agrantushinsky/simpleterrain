#include "game.h"
#include "shader.h"

int main(void)
{
    a();
    // if we fail to setup, return failure.
    if(!game_init())
        return -1;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(game->window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(game->window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}