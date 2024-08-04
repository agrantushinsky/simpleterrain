#ifndef TEXTURE_H
#define TEXTURE_H

#include "shared.h"

struct {
    GLuint texture;
    GLenum format;
    int texture_size;
    int max_textures;
    int num_textures;
} typedef TextureArray;

GLuint texture_create(const char* texture_name, GLint internal_format, GLenum format);


void texture_array_create(TextureArray* texture_array, GLint internal_format, GLenum format, int texture_size, int max_textures);

void texture_array_add(TextureArray* texture_array, const char* texture_name);

#endif

