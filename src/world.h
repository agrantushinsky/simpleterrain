#ifndef WORLD_H
#define WORLD_H

#include "shared.h"
#include "chunk.h"

struct {
	Chunk* chunks;
} typedef World;

World* world_init();

#endif
