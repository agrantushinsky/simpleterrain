#include "world.h"
#include "chunk.h"
#include "shader.h"
#include "game.h"
#include "texture.h"
#include <time.h>
#include "worldgen.h"

#define STB_DS_IMPLEMENTATION
#include "stb/stb_ds.h"

int64_t i16x3_to_i64(i16x3 in)
{
    int64_t r = 0;
    r |= (int64_t)(u_int16_t)in[0] << 32;
    r |= (int64_t)(u_int16_t)in[1] << 16;
    r |= (int64_t)(u_int16_t)in[2] << 0;
    return r;
}

void i64_to_i16x3(int64_t in, i16x3 out)
{
    out[0] = (int16_t)(in >> 32);
    out[1] = (int16_t)(in >> 16);
    out[2] = (int16_t)(in);
}

void world_init(World* world) 
{
    world->chunks = 0;
    world->seed = (int)time(NULL);

    world_generate_chunks(world);

    TextureArray texture_array;
    texture_array_create(&texture_array, GL_RGBA, GL_RGB, 16, 6);
    texture_array_add(&texture_array, "../res/images/stone.png");
    texture_array_add(&texture_array, "../res/images/dirt.png");
    texture_array_add(&texture_array, "../res/images/grass_side.png");
    texture_array_add(&texture_array, "../res/images/grass.png");
    texture_array_add(&texture_array, "../res/images/sand.png");
    texture_array_add(&texture_array, "../res/images/water.png");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture_array.texture);

    glUniform1i(glGetUniformLocation(texture_array.texture, "sTexture"), 0);
}

void world_destory(World* world)
{
    for(int i = 0; i < hmlen(world->chunks); i++) 
    {
        Chunk* chunk = world->chunks[i].value;
        free(chunk);
    }
    free(world);
}

void world_update(World* world)
{
    // Eventually the loading/unloading logic should go here.
}

void world_render(World* world)
{
    // TODO: refactor "shader_programs"
    for(int i = 0; i < hmlen(world->chunks); i++) 
    {
        Chunk* chunk = world->chunks[i].value;
        i16x3 pos;
        i64_to_i16x3(world->chunks[i].key, pos);

        GLuint matrix_location = glGetUniformLocation(shader_programs[0], "matrix");
        assert(matrix_location != 0xFFFFFFFF);

        mat4 translation = GLM_MAT4_IDENTITY_INIT;
        glm_translate(translation, (vec3){
            pos[0] * CHUNK_SIZE,
            pos[1] * CHUNK_SIZE,
            pos[2] * CHUNK_SIZE,
        });

        mat4 final_matrix;
        glm_mat4_mulN((mat4* []){&game->camera.perspective, &game->camera.view, &translation}, 3, final_matrix);

        glUniformMatrix4fv(matrix_location, 1, GL_FALSE, (float*)final_matrix);

        chunk_render(chunk);
    }
}


void world_generate_chunks(World* world)
{
    int lmax = 16;
    for(int x = 0; x < lmax; x++)
    {
        for(int z = 0; z < lmax; z++)
        {
            worldgen_generate_chunk_vertical(world, (ivec2){x, z});
        }
    }

    for(int i = 0; i < hmlen(world->chunks); i++)
    {
        ChunkHash ch = world->chunks[i];
        i16x3 pos;
        i64_to_i16x3(ch.key, pos);
        // It's best not to ask why here.
        chunk_generate_mesh(ch.value, (ivec3){ pos[0], pos[1], pos[2] }, world);
    }
}

Chunk* world_get_chunk(World* world, i16x3 chunk_position)
{
    return hmget(world->chunks, i16x3_to_i64(chunk_position));
}

Chunk* world_allocate_chunk(World* world, i16x3 chunk_position)
{
    Chunk* chunk = calloc(1, sizeof(Chunk));
    ChunkHash hchunk = {
        i16x3_to_i64(chunk_position),
        chunk
    };
    hmputs(world->chunks, hchunk);

    return chunk;
}

void world_deallocate_chunk(World* world, i16x3 chunk_position)
{
    int64_t key = i16x3_to_i64(chunk_position);
    Chunk* chunk = hmget(world->chunks, key);
    free(chunk);
    hmdel(world->chunks, key);
}

