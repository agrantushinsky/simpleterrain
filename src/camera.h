#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/vec3.h>
#include <cglm/mat4.h>

struct {
	vec3 position;
	float yaw;
	float pitch;

	mat4 perspective;
	mat4 view;
} typedef Camera;

void camera_init(Camera* camera, float vfov);

void camera_update_perspective(Camera* camera, float vfov, float aspect_ratio, float near, float far);

void camera_update_view(Camera* camera);

#endif