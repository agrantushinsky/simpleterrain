#include "texture.h"
#include "shared.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

GLuint texture_create(const char* texture_name, GLint internal_format, GLenum format)
{
    GLuint texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nChannels;
    unsigned char* image = stbi_load(texture_name, &width, &height, &nChannels, 0);
    if(image)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else 
    {
        printf("Failed to create texture: '%s'\n", texture_name);
    }
    stbi_image_free(image);

    return texture;
}

void texture_array_create(TextureArray* texture_array, GLint internal_format, GLenum format, int texture_size, int max_textures)
{
    texture_array->texture_size = texture_size;
    texture_array->max_textures = max_textures;
    texture_array->num_textures = 0;
    texture_array->format = format;

    glGenTextures(1, &texture_array->texture);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture_array->texture);

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, internal_format, texture_size, texture_size, max_textures, 0, format, GL_UNSIGNED_BYTE, 0);
}

void texture_array_add(TextureArray* texture_array, const char* texture_name)
{
    if(texture_array->num_textures >= texture_array->max_textures)
    {
        printf("Cannot add texture '%s', texture array is full.\n", texture_name);
        return;
    }

    int width, height, nChannels;
    unsigned char* image = stbi_load(texture_name, &width, &height, &nChannels, 0);
    if(image)
    {
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, texture_array->num_textures++, texture_array->texture_size, texture_array->texture_size, 1, texture_array->format, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
    }
    else 
    {
        printf("Failed to create sub texture: '%s'\n", texture_name);
    }

    stbi_image_free(image);
}

