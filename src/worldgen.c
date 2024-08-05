#include "worldgen.h"
#include "chunk.h"
#include "world.h"
#include <math.h>

#define STB_PERLIN_IMPLEMENTATION
#include "stb/stb_perlin.h"

void worldgen_generate_chunk_vertical(World* world, ivec2 chunk_pos)
{
    float height_map[CHUNK_SIZE][CHUNK_SIZE];
    for(int i = 0; i < CHUNK_SIZE; i++)
    {
        for(int j = 0; j < CHUNK_SIZE; j++)
        {
            float rx = (float)i + chunk_pos[0] * 16,
                ry = 0,
                rz = (float)j + chunk_pos[1] * 16;

            float noise = stb_perlin_noise3_seed(rx / 36, ry, rz / 36, 0, 0, 0, world->seed);
            noise += 1;
            noise /= 2;
            noise *= powf(noise, 1.25);
            height_map[i][j] = noise;
        }
    }

    const int CHUNK_HEIGHT = 8;

    for(int cy = 0; cy < CHUNK_HEIGHT; cy++)
    {
        Chunk* chunk = world_allocate_chunk(world, (i16x3){ chunk_pos[0], cy, chunk_pos[1] });

        for (int x = 0; x < CHUNK_SIZE; x++)
        {
            for (int z = 0; z < CHUNK_SIZE; z++)
            {
                float height = height_map[x][z];
                int top = height * CHUNK_SIZE * CHUNK_HEIGHT;

                for (int y = 0; y < CHUNK_SIZE; y++)
                {
                    int ry = y + cy * CHUNK_SIZE;

                    if(ry == top) {
                        chunk->blocks[x][y][z].type = Grass;
                    } else if(ry < top) {
                        if(ry > top - 2) {
                            chunk->blocks[x][y][z].type = Dirt;
                        } else {
                            chunk->blocks[x][y][z].type = Stone;
                        }
                    }
                }
            }
        }
	}
}
