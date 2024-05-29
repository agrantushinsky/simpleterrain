#include "camera.h"
#include "game.h"
#include <cglm/cam.h>

// TODO: Refactor this code.

void camera_init(Camera* camera, float vfov)
{
	glm_vec3_copy(GLM_VEC3_ZERO, &camera->position);
	camera->yaw = -89.0f;
	camera->pitch = 0.0f;

	camera_update_perspective(camera, vfov, (float)game->window_width / (float)game->window_height, 0.01f, 1000.0f);
	camera_update_view(camera);
}

void camera_update_perspective(Camera* camera, float vfov, float aspect_ratio, float near, float far) 
{
	glm_perspective(glm_rad(vfov), aspect_ratio, near, far, &camera->perspective);
}

void camera_update_view(Camera* camera)
{
	vec3 camera_front = GLM_VEC3_ZERO_INIT;
	camera_front[0] = cosf(glm_rad(camera->yaw)) * cosf(glm_rad(camera->pitch));
	camera_front[1] = sinf(glm_rad(camera->pitch));
	camera_front[2] = sinf(glm_rad(camera->yaw)) * cosf(glm_rad(camera->pitch));
	glm_normalize(&camera_front);

    vec3 camera_up = { 0.0f, 1.0f, 0.0f };
    vec3 camera_center = GLM_VEC3_ZERO_INIT;
    glm_vec3_add(camera->position, camera_front, &camera_center);
    glm_lookat(camera->position, camera_center, camera_up, &camera->view);
}

