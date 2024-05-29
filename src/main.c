#include "game.h"
#include "shader.h"
#include "input.h"

void handle_input() 
{
    const static float speed = 0.01f;
    float yaw = game->camera.yaw;
    float pitch = game->camera.pitch;
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

    camera_update_view(&game->camera);
}

int main(void)
{
    // if we fail to setup, return failure.
    if(!game_init())
        return -1;

    vec3 vertices[] = {
		{-0.5f, -0.5f, 0.5f},
		{0.5f, -0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f},
		{-0.5f, 0.5f, 0.5f},
		{-0.5f, -0.5f, -0.5f},
		{0.5f, -0.5f, -0.5f},
		{0.5f, 0.5f, -0.5f},
		{-0.5f, 0.5f, -0.5f}
    };

    unsigned int indices[] = {
		0, 1, 2, 2, 3, 0,
		1, 5, 6, 6, 2, 1,
		7, 6, 5, 5, 4, 7,
		4, 0, 3, 3, 7, 4,
		4, 5, 1, 1, 0, 4,
		3, 2, 6, 6, 7, 3
    };

    GLuint vertex_buffer;
    GLuint index_buffer;

    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

    glGenBuffers(1, &index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glUseProgram(shader_programs[0]);

    GLuint matrix_location = glGetUniformLocation(shader_programs[0], "matrix");
    assert(matrix_location != 0xFFFFFFFF);

	mat4 translation = GLM_MAT4_IDENTITY_INIT;

	glm_translate(translation, (vec3){0.0f, 0.0f, -3.0f});

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

    while (!glfwWindowShouldClose(game->window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mat4 rotation = GLM_MAT4_IDENTITY_INIT;
		//glm_rotate(rotation, sinf(glfwGetTime()), (vec3) { 1.0f, 1.0f, 1.0f });

		mat4 final_matrix;
		glm_mat4_mulN((mat4* []){&game->camera.perspective, &game->camera.view, &translation, &rotation},
			4, final_matrix);

        glUniformMatrix4fv(matrix_location, 1, GL_FALSE, final_matrix);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(game->window);
        glfwPollEvents();
        handle_input();
    }

    glfwTerminate();
    return 0;
}