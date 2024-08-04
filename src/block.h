#ifndef BLOCK_H
#define BLOCK_H

#include "shared.h"
#include <stdint.h>

enum : uint8_t {
    Air,
    Stone,
    Dirt,
    Grass
} typedef BlockType;

struct {
	BlockType type;
} typedef Block;

int block_get_texture_from_block_face(BlockType type, vec3 norm_face);

#endif

