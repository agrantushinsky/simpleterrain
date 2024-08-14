#include "worldgen.h"
#include "chunk.h"
#include "world.h"
#include <math.h>

#define STB_PERLIN_IMPLEMENTATION
#include "stb/stb_perlin.h"

/*
 * Steps:
 * 1. Determine which blocks are solid (e.g.: stone or air)
 *  - Combine different noise functions, each with their own amplitudes and octaves. (e.g.: continentalness, erosion, and valleys)
 *  and:
 *  - By using 3d noise:
 *  - f(x, y, z) => density (> 0 solid, < 0 air)
 *  - use a squashing factor, and a height offset
 *
 * 2. Fill in ocean/lakes/rivers (i.e.: below a certain y-level (?))
 * 3. Apply the surface layer
 * 4. Decorations
*/

void worldgen_generate_chunk_vertical(World* world, ivec2 chunk_pos)
{
    const int WORLD_HEIGHT = CHUNK_SIZE * 4;
    const int SEA_HEIGHT = 12;

    int terrain_height[CHUNK_SIZE][CHUNK_SIZE];
    for(int i = 0; i < CHUNK_SIZE; i++)
    {
        for(int j = 0; j < CHUNK_SIZE; j++)
        {
            float rx = (float)i + chunk_pos[0] * CHUNK_SIZE,
                ry = 0,
                rz = (float)j + chunk_pos[1] * CHUNK_SIZE;

            // I don't even know.
            float continentalness = stb_perlin_fbm_noise3(rx, ry, rz, 0.05f, 0.4f, 4);
            float erosion = stb_perlin_fbm_noise3(rx, ry, rz, 0.02f, 0.7f, 2);
            float valleys = stb_perlin_fbm_noise3(rx / 16, ry / 16, rz / 16, 0.5f, 0.5f, 2);

            float noise = continentalness + (erosion * valleys * 4);

            terrain_height[i][j] = (float)WORLD_HEIGHT / 2 + (noise * WORLD_HEIGHT / 2);
        }
    }

    for(int cy = 0; cy < WORLD_HEIGHT / CHUNK_SIZE; cy++)
    {
        Chunk* chunk = world_allocate_chunk(world, (i16x3){ chunk_pos[0], cy, chunk_pos[1] });

        for(int x = 0; x < CHUNK_SIZE; x++)
        {
            for(int z = 0; z < CHUNK_SIZE; z++)
            {
                for(int ly = 0; ly < CHUNK_SIZE; ly++)
                {
                    int y = ly + cy * CHUNK_SIZE;
                    
                    BlockType type = Air;
                    int height = terrain_height[x][z];
                    if(y <= SEA_HEIGHT && y > height) {
                        type = Water;
                    }
                    else if(y <= SEA_HEIGHT && y == height) {
                        type = Sand;
                    }
                    else if(y == height) {
                        type = Grass;
                    }
                    else if(y > height - 3 && y < height) {
                        type = Dirt;
                    }
                    else if(y < height){
                        type = Stone;
                    }

                    chunk->blocks[x][ly][z].type = type;
                }
            }
        }
    }
}

