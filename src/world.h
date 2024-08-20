#ifndef WORLD_H
#define WORLD_H

#include "shared.h"
#include "chunk.h"
#include <sys/types.h>

typedef int16_t i16x3[3];

int64_t i16x3_to_i64(i16x3 in);

void i64_to_i16x3(int64_t in, i16x3 out);

struct {
    int64_t key;
    Chunk* value;
} typedef ChunkHash;

struct {
	ChunkHash* chunks;
    int seed;
} typedef World;

void world_init(World* world);

void world_destory(World* world);

void world_update(World* world);

void world_render(World* world);

void world_generate_chunks(World* world);

Chunk* world_get_chunk(World* world, i16x3 chunk_position);

Chunk* world_allocate_chunk(World* world, i16x3 chunk_position);

void world_deallocate_chunk(World* world, i16x3 chunk_position);

#endif
