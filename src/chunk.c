#include "chunk.h"
#include "block.h"
#include "shared.h"
#include <GL/glext.h>
#include <stdlib.h>

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
        { -1.f, -1.f, -1.f }, { -1.f, 1.f, -1.f }, { -1.f, -1.f, 1.f },
        { -1.f, 1.f, 1.f }, { -1.f, 1.f, -1.f }, { -1.f, -1.f, 1.f },

        // +x
        { 1.f, -1.f, -1.f }, { 1.f, 1.f, -1.f }, { 1.f, -1.f, 1.f },
        { 1.f, 1.f, 1.f }, { 1.f, 1.f, -1.f }, { 1.f, -1.f, 1.f },

        // -y
        { -1.f, -1.f, -1.f }, { -1.f, -1.f, 1.f }, { 1.f, -1.f, 1.f },
        { -1.f, -1.f, -1.f }, { 1.f, -1.f, 1.f }, { 1.f, -1.f, -1.f },

        // +y
        { -1.f, 1.f, -1.f }, { -1.f, 1.f, 1.f }, { 1.f, 1.f, 1.f },
        { -1.f, 1.f, -1.f }, { 1.f, 1.f, 1.f }, { 1.f, 1.f, -1.f },

        // -z
        { -1.f, -1.f, -1.f }, { -1.f, 1.f, -1.f }, { 1.f, -1.f, -1.f },
        { 1.f, 1.f, -1.f }, { -1.f, 1.f, -1.f }, { 1.f, -1.f, -1.f },

        // +z
        { -1.f, -1.f, 1.f }, { -1.f, 1.f, 1.f }, { 1.f, -1.f, 1.f },
        { 1.f, 1.f, 1.f }, { -1.f, 1.f, 1.f }, { 1.f, -1.f, 1.f },
    };

    chunk->buffer_size = 2;
    chunk->buffer_usage = 0;
    chunk->buffer = malloc(sizeof(vertices) * chunk->buffer_size);

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
                    // TODO;
                    break;
                }

                float* buffer = &chunk->buffer[chunk->buffer_usage * 36 * 3];
                
                // copy vertex data into the buffer
                for(int i = 0; i < 36; i++)
                {
                    *(buffer++) = vertices[i][0] / 2 + x;
                    *(buffer++) = vertices[i][1] / 2 + y;
                    *(buffer++) = vertices[i][2] / 2 + z;
                }
                chunk->buffer_usage++;
			}
		}
	}

    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * chunk->buffer_usage, chunk->buffer, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
}

void chunk_render(Chunk* chunk)
{
    glDrawArrays(GL_TRIANGLES, 0, 36 * chunk->buffer_usage);
}

