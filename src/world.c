#include "world.h"
#include "chunk.h"
#include "shader.h"
#include "game.h"
#include "texture.h"

void world_init(World* world) 
{
    world->chunks_max = 16;
    world->loaded_chunks = 0;
    world->chunks = calloc(world->chunks_max, sizeof(Chunk));

    world_generate_chunks(world);

    TextureArray texture_array;
    texture_array_create(&texture_array, GL_RGBA, GL_RGB, 16, 4);
    texture_array_add(&texture_array, "../res/images/stone.png");
    texture_array_add(&texture_array, "../res/images/dirt.png");
    texture_array_add(&texture_array, "../res/images/grass_side.png");
    texture_array_add(&texture_array, "../res/images/grass.png");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture_array.texture);

    glUniform1i(glGetUniformLocation(texture_array.texture, "sTexture"), 0);   
}

void world_update(World* world)
{
    // Eventually the loading/unloading logic should go here.
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


void world_generate_chunks(World* world)
{
    // for now just generate a "flat" world
    int max = world->chunks_max;
    int lmax = (int)sqrt(world->chunks_max);
    for(int x = 0; x < lmax; x++)
    {
        for(int z = 0; z < lmax; z++)
        {
            printf("generated chunk at { %i, %i }\n", x, z);
            glm_ivec3_copy((ivec3){x, 0.f, z}, world->chunks[world->loaded_chunks].position);
            chunk_generate(&world->chunks[world->loaded_chunks]);
            chunk_generate_mesh(&world->chunks[world->loaded_chunks]);
            world->loaded_chunks++;
        }
    }
}

