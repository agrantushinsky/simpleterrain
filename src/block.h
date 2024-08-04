#ifndef BLOCK_H
#define BLOCK_H

#include "shared.h"
#include <stdint.h>
#include <sys/types.h>

enum : uint8_t {
    Air,
    Stone,
    Dirt,
    Grass
} typedef BlockType;

enum : uint8_t {
    NX,
    PX,
    NZ,
    PZ,
    NY,
    PY
} typedef FaceDirection;

enum : int8_t {
    FaceInvalid = -1,
    FaceStone,
    FaceDirt,
    FaceGrassSide,
    FaceGrass
} typedef FaceTextureIndex;

static const int neighbours_len = 6;
static const ivec3 neighbours[] = {
    { -1, 0, 0 }, // -x
    { 1, 0, 0 }, // +x
    { 0, 0, -1 }, // -z
    { 0, 0, 1 }, // +z
    { 0, -1, 0 }, // -y
    { 0, 1, 0 }, // +y
};

struct {
	BlockType type;
} typedef Block;

FaceTextureIndex block_get_texture_from_block_face(BlockType type, FaceDirection face_direction);

#endif

