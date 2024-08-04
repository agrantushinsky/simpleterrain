#include "block.h"

FaceTextureIndex block_get_texture_from_block_face(BlockType type, FaceDirection face_direction)
{
    switch(type) 
    {
        case Air:
            return FaceInvalid;
        case Stone:
            return FaceStone;
        case Dirt:
            return FaceDirt;
        case Grass:
            if(face_direction <= PZ) {
                return FaceGrassSide;
            } else if(face_direction == NY) {
                return FaceDirt;
            } else if(face_direction == PY) {
                return FaceGrass;
            }
    }

    return FaceInvalid;
}
