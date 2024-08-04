#include "chunk.h"
#include "block.h"
#include "shared.h"
#include <GL/glext.h>
#include <stdlib.h>
#include "texture.h"

Chunk* chunk_generate()
{
    // Allocate 1 chunk, initialized to 0
    // which conveniently sets the block type to 0 (air).
    Chunk* chunk = calloc(1, sizeof(Chunk));

	for (int x = 0; x < CHUNK_SIZE; x++)
	{
		for (int y = 0; y < CHUNK_SIZE; y++)
		{
			for (int z = 0; z < CHUNK_SIZE; z++)
			{
                if(y == 0) 
                {
                    chunk->blocks[x][y][z].type = Stone;
                }
			}
		}
	}

    return chunk;
}

void chunk_generate_mesh(Chunk* chunk)
{
    static const vec3 vertices[] = {
        // -x
        { -1.f, -1.f, 1.f }, { -1.f, -1.f, -1.f }, { -1.f, 1.f, 1.f }, 
        { -1.f, 1.f, -1.f }, { -1.f, 1.f, 1.f }, { -1.f, -1.f, -1.f },

        // +x
        { 1.f, -1.f, -1.f }, { 1.f, -1.f, 1.f }, { 1.f, 1.f, -1.f }, 
        { 1.f, 1.f, 1.f }, { 1.f, 1.f, -1.f }, { 1.f, -1.f, 1.f },

        // -y
        { 1.f, -1.f, -1.f }, { 1.f, -1.f, 1.f },{ -1.f, -1.f, -1.f },
        { -1.f, -1.f, 1.f }, { -1.f, -1.f, -1.f }, { 1.f, -1.f, 1.f },

        // +y
        { 1.f, 1.f, -1.f }, { 1.f, 1.f, 1.f },{ -1.f, 1.f, -1.f },
        { -1.f, 1.f, 1.f }, { -1.f, 1.f, -1.f }, { 1.f, 1.f, 1.f },

        // -z
        { -1.f, -1.f, -1.f }, { 1.f, -1.f, -1.f }, { -1.f, 1.f, -1.f },
        { 1.f, 1.f, -1.f }, { -1.f, 1.f, -1.f }, { 1.f, -1.f, -1.f },

        // +z
        { 1.f, -1.f, 1.f }, { -1.f, -1.f, 1.f }, { 1.f, 1.f, 1.f }, 
        { -1.f, 1.f, 1.f }, { 1.f, 1.f, 1.f }, { -1.f, -1.f, 1.f },
    };

    static const vec2 tex_coords[] = {
        { 1.f, 1.f }, // top right
        { 0.f, 1.f }, // top left
        { 1.f, 0.f }, // bottom right
        { 0.f, 0.f }, // bottom left
        { 1.f, 0.f }, // bottom right
        { 0.f, 1.f }, // top left
    };

    const uint cube_size = sizeof(vertices) + (sizeof(vec3) * 36);

    chunk->buffer_size = 1;//256;
    chunk->buffer_usage = 0;
    chunk->buffer = malloc(cube_size * chunk->buffer_size);

    // This might be a little slow :) 
	for (int x = 0; x < CHUNK_SIZE; x++)
	{
		for (int y = 0; y < CHUNK_SIZE; y++)
		{
			for (int z = 0; z < CHUNK_SIZE; z++)
			{
                Block block = chunk->blocks[x][y][z];

                if(block.type == Air) continue;
                if(chunk->buffer_usage >= chunk->buffer_size)
                {
                    break;
                    chunk->buffer_size *= 2;
                    chunk->buffer = realloc(chunk->buffer, cube_size * chunk->buffer_size);
                }

                float* buffer = &chunk->buffer[chunk->buffer_usage * 36 * (3 + 3)];
                
                // TODO: Optimize by only copying the block face that is exposed to air
                // copy vertex data into the buffer
                for(int i = 0; i < 36; i++)
                {
                    // vertex positions
                    *(buffer++) = vertices[i][0] / 2 + x;
                    *(buffer++) = vertices[i][1] / 2 + y;
                    *(buffer++) = vertices[i][2] / 2 + z;

                    // texture coordinates
                    *(buffer++) = tex_coords[i % 6][0];
                    *(buffer++) = tex_coords[i % 6][1];

                    // texture index
                    *(buffer++) = 0.f;
                }
                chunk->buffer_usage++;
			}
		}
	}

    for(int i = 0; i < 36; i++)
    {
        printf("{ %f, %f, %f }, { %f, %f, %f }\n",
               chunk->buffer[i * 6],
               chunk->buffer[i * 6 + 1],
               chunk->buffer[i * 6 + 2],
               chunk->buffer[i * 6 + 3],
               chunk->buffer[i * 6 + 4],
               chunk->buffer[i * 6 + 5]);
    }

    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, cube_size * chunk->buffer_usage, chunk->buffer, GL_STATIC_DRAW);

    // vertex positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3) + sizeof(vec3), 0);
    glEnableVertexAttribArray(0);

    // texture coordinates
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vec3) + sizeof(vec3), (void*)sizeof(vec3));
    glEnableVertexAttribArray(1);

    TextureArray texture_array;
    texture_array_create(&texture_array, GL_RGBA, GL_RGB, 16, 4);
    texture_array_add(&texture_array, "../res/images/stone.png");
    texture_array_add(&texture_array, "../res/images/dirt.png");
    texture_array_add(&texture_array, "../res/images/grass_side.png");
    texture_array_add(&texture_array, "../res/images/grass.png");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture_array.texture);

    glUniform1i(glGetUniformLocation(texture_array.texture, "sTexture"), 0);   
}

void chunk_render(Chunk* chunk)
{
    glDrawArrays(GL_TRIANGLES, 0, 36 * chunk->buffer_usage);
}

