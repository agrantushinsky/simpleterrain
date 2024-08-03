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
        { -1.f, -1.f, 1.f }, { -1.f, 1.f, 1.f }, { -1.f, -1.f, -1.f },
        { -1.f, 1.f, -1.f }, { -1.f, 1.f, 1.f }, { -1.f, -1.f, -1.f },

        // +x
        { 1.f, -1.f, -1.f }, { 1.f, 1.f, -1.f }, { 1.f, -1.f, 1.f },
        { 1.f, 1.f, 1.f }, { 1.f, 1.f, -1.f }, { 1.f, -1.f, 1.f },

        // -y
        { 1.f, -1.f, -1.f }, { 1.f, -1.f, 1.f },{ -1.f, -1.f, -1.f },
        { -1.f, -1.f, 1.f }, { -1.f, -1.f, -1.f }, { 1.f, -1.f, 1.f },

        // +y
        { 1.f, 1.f, -1.f }, { 1.f, 1.f, 1.f },{ -1.f, 1.f, -1.f },
        { -1.f, 1.f, 1.f }, { -1.f, 1.f, -1.f }, { 1.f, 1.f, 1.f },

        // -z
        { -1.f, -1.f, -1.f }, { -1.f, 1.f, -1.f }, { 1.f, -1.f, -1.f },
        { 1.f, 1.f, -1.f }, { -1.f, 1.f, -1.f }, { 1.f, -1.f, -1.f },

        // +z
        { 1.f, -1.f, 1.f }, { 1.f, 1.f, 1.f }, { -1.f, -1.f, 1.f },
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

    const uint cube_size = sizeof(vertices) + (sizeof(vec2) * 36);

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

                float* buffer = &chunk->buffer[chunk->buffer_usage * 36 * (3 + 2)];
                
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
                }
                chunk->buffer_usage++;
			}
		}
	}

    for(int i = 0; i < 36; i++)
    {
        printf("{ %f, %f, %f }, { %f, %f }\n",
               chunk->buffer[i * 5],
               chunk->buffer[i * 5 + 1],
               chunk->buffer[i * 5 + 2],
               chunk->buffer[i * 5 + 3],
               chunk->buffer[i * 5 + 4]);
    }

    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, cube_size * chunk->buffer_usage, chunk->buffer, GL_STATIC_DRAW);

    // vertex positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3) + sizeof(vec2), 0);
    glEnableVertexAttribArray(0);

    // texture coordinates
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vec3) + sizeof(vec2), (void*)sizeof(vec3));
    glEnableVertexAttribArray(1);

    GLuint texture = texture_create("../res/images/blocks.png", GL_RGBA, GL_RGB);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glUniform1i(glGetUniformLocation(texture, "sTexture"), 0);   
}

void chunk_render(Chunk* chunk)
{
    glDrawArrays(GL_TRIANGLES, 0, 36 * chunk->buffer_usage);
}

