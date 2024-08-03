#include "chunk.h"
#include "game.h"
#include "shader.h"
#include "input.h"
#include "shared.h"

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

int main(void)
{
    // if we fail to setup, return failure.
    if(!game_init())
        return -1;

    glUseProgram(shader_programs[0]);

    GLuint matrix_location = glGetUniformLocation(shader_programs[0], "matrix");
    assert(matrix_location != 0xFFFFFFFF);

	mat4 translation = GLM_MAT4_IDENTITY_INIT;

	glm_translate(translation, (vec3){0.0f, 0.0f, -3.0f});

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);


    Chunk* chunk = chunk_generate();

    chunk_generate_mesh(chunk);

    while (!glfwWindowShouldClose(game->window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mat4 rotation = GLM_MAT4_IDENTITY_INIT;

		mat4 final_matrix;
		glm_mat4_mulN((mat4* []){&game->camera.perspective, &game->camera.view, &translation, &rotation},
			4, final_matrix);

        glUniformMatrix4fv(matrix_location, 1, GL_FALSE, (float*)final_matrix);

        chunk_render(chunk);

        glfwSwapBuffers(game->window);
        glfwPollEvents();
        handle_input();
    }

    glfwTerminate();
    return 0;
}
 
