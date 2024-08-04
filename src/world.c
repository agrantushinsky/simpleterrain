#include "world.h"
#include "chunk.h"
#include "shader.h"
#include "game.h"

void world_init(World* world) 
{
    world->chunks_max = 16;
    world->loaded_chunks = 0;
    world->chunks = calloc(world->chunks_max, sizeof(Chunk));
}

void world_update(World* world)
{
    // Eventually the loading/unloading logic should go here.
    static bool generated = false;
    if(generated) {
        return;
    }
    generated = true;

    // This also doesn't really make sense right now....
    world->loaded_chunks++;
    chunk_generate(&world->chunks[0]);
    chunk_generate_mesh(&world->chunks[0]);
    glm_ivec3_copy((ivec3){0.f, 0.f, 0.f}, world->chunks[0].position);
}

void world_render(World* world)
{
    // TODO: refactor "shader_programs"
    for(int i = 0; i < world->loaded_chunks; i++) 
    {
        Chunk* chunk = &world->chunks[i];
        GLuint matrix_location = glGetUniformLocation(shader_programs[0], "matrix");
        assert(matrix_location != 0xFFFFFFFF);

        mat4 translation = GLM_MAT4_IDENTITY_INIT;
        glm_translate(translation, (vec3){
            chunk->position[0] * CHUNK_SIZE,
            chunk->position[1] * CHUNK_SIZE,
            chunk->position[2] * CHUNK_SIZE,
        });

        mat4 final_matrix;
        glm_mat4_mulN((mat4* []){&game->camera.perspective, &game->camera.view, &translation}, 3, final_matrix);

        glUniformMatrix4fv(matrix_location, 1, GL_FALSE, (float*)final_matrix);

        chunk_render(chunk);
    }
}

