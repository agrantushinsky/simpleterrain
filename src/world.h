#ifndef WORLD_H
#define WORLD_H

#include "shared.h"
#include "chunk.h"

struct {
	Chunk* chunks;
    int chunks_max;

    int loaded_chunks;
} typedef World;

void world_init(World* world);

void world_update(World* world);

void world_render(World* world);

void world_generate_chunks(World* world);

#endif
