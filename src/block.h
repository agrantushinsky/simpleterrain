#ifndef BLOCK_H
#define BLOCK_H

#include "shared.h"
#include <stdint.h>

enum : uint8_t {
    Air,
    Stone
} typedef BlockType;

struct {
	BlockType type;
} typedef Block;

#endif

