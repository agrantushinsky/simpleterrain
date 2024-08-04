#ifndef CHUNK_H
#define CHUNK_H

#include "shared.h"
#include "block.h"

#define CHUNK_SIZE 16
#define CHUNK_AREA CHUNK_SIZE*CHUNK_SIZE
#define CHUNK_VOLUME CHUNK_SIZE*CHUNK_SIZE*CHUNK_SIZE

#define CHUNK_BUFFER_POSITION_SIZE sizeof(vec3)
#define CHUNK_BUFFER_TEXTURE_SIZE sizeof(vec3)
#define CHUNK_BUFFER_FACE_SIZE 6
#define CHUNK_BUFFER_FACE_BYTES 

struct {
	ivec3 position;
	Block blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];

    float* buffer;
    uint buffer_size;
    uint buffer_usage;
    uint buffer_faces;
} typedef Chunk;

void chunk_generate(Chunk* chunk);

void chunk_generate_mesh(Chunk* chunk);

void chunk_render(Chunk* chunk);

#endif

