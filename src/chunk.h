#ifndef CHUNK_H
#define CHUNK_H

#include "shared.h"
#include "block.h"

#define CHUNK_SIZE 16
#define CHUNK_AREA CHUNK_SIZE*CHUNK_SIZE
#define CHUNK_VOLUME CHUNK_SIZE*CHUNK_SIZE*CHUNK_SIZE

struct {
	ivec3 position;
	Block blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];

    float* buffer;
    uint buffer_size;
    uint buffer_usage;
} typedef Chunk;

Chunk* chunk_generate();

void chunk_generate_mesh(Chunk* chunk);

void chunk_render(Chunk* chunk);

#endif

