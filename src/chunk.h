#ifndef CHUNK_H
#define CHUNK_H

#include "shared.h"
#include "block.h"

#define CHUNK_SIZE 16

#define TRIANGLE_SIZE 6
#define TRIANGLE_BYTES 2*sizeof(vec3)

struct {
	ivec3 position;
	Block blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];

    float* buffer;
    uint buffer_size;
    uint buffer_triangles;
} typedef Chunk;

void chunk_generate(Chunk* chunk);

void chunk_generate_mesh(Chunk* chunk);

void chunk_render(Chunk* chunk);

#endif

