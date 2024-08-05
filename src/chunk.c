#include "chunk.h"
#include "world.h"

void chunk_generate(Chunk* chunk)
{
	for (int x = 0; x < CHUNK_SIZE; x++)
	{
		for (int y = 0; y < CHUNK_SIZE; y++)
		{
			for (int z = 0; z < CHUNK_SIZE; z++)
			{
                chunk->blocks[x][y][z].type = Grass;
			}
		}
	}
}

void chunk_generate_mesh(Chunk* chunk, ivec3 chunk_pos, void* world)
{
    static const vec3 vertices[] = {
        // -x
        { -1.f, -1.f, 1.f }, { -1.f, -1.f, -1.f }, { -1.f, 1.f, 1.f }, 
        { -1.f, 1.f, -1.f }, { -1.f, 1.f, 1.f }, { -1.f, -1.f, -1.f },

        // +x
        { 1.f, -1.f, -1.f }, { 1.f, -1.f, 1.f }, { 1.f, 1.f, -1.f }, 
        { 1.f, 1.f, 1.f }, { 1.f, 1.f, -1.f }, { 1.f, -1.f, 1.f },

        // -z
        { -1.f, -1.f, -1.f }, { 1.f, -1.f, -1.f }, { -1.f, 1.f, -1.f },
        { 1.f, 1.f, -1.f }, { -1.f, 1.f, -1.f }, { 1.f, -1.f, -1.f },

        // +z
        { 1.f, -1.f, 1.f }, { -1.f, -1.f, 1.f }, { 1.f, 1.f, 1.f }, 
        { -1.f, 1.f, 1.f }, { 1.f, 1.f, 1.f }, { -1.f, -1.f, 1.f },

        // -y
        { 1.f, -1.f, -1.f }, { 1.f, -1.f, 1.f },{ -1.f, -1.f, -1.f },
        { -1.f, -1.f, 1.f }, { -1.f, -1.f, -1.f }, { 1.f, -1.f, 1.f },

        // +y
        { 1.f, 1.f, -1.f }, { 1.f, 1.f, 1.f },{ -1.f, 1.f, -1.f },
        { -1.f, 1.f, 1.f }, { -1.f, 1.f, -1.f }, { 1.f, 1.f, 1.f },
    };

    static const vec2 tex_coords[] = {
        { 1.f, 1.f }, // top right
        { 0.f, 1.f }, // top left
        { 1.f, 0.f }, // bottom right
        { 0.f, 0.f }, // bottom left
        { 1.f, 0.f }, // bottom right
        { 0.f, 1.f }, // top left
    };


    chunk->buffer_size = TRIANGLES_PER_BLOCK * 16;
    chunk->buffer_triangles = 0;
    chunk->buffer = malloc(TRIANGLE_BYTES * chunk->buffer_size);

    // This might be a little slow :) 
	for (int x = 0; x < CHUNK_SIZE; x++)
	{
		for (int y = 0; y < CHUNK_SIZE; y++)
		{
			for (int z = 0; z < CHUNK_SIZE; z++)
			{
                Block block = chunk->blocks[x][y][z];

                if(block.type == Air) continue;
                if(chunk->buffer_triangles >= chunk->buffer_size - TRIANGLES_PER_BLOCK)
                {
                    chunk->buffer_size *= 2;
                    chunk->buffer = realloc(chunk->buffer, TRIANGLE_BYTES * chunk->buffer_size);
                }

                float* buffer = &chunk->buffer[chunk->buffer_triangles * TRIANGLE_SIZE];

                for(int i = 0; i < neighbours_len; i++)
                {
                    int dx = neighbours[i][0];
                    int dy = neighbours[i][1];
                    int dz = neighbours[i][2];
                    int nx = x + dx;
                    int ny = y + dy;
                    int nz = z + dz;

                    if(nx >= 0 && nx < CHUNK_SIZE && 
                        ny >= 0 && ny < CHUNK_SIZE && 
                        nz >= 0 && nz < CHUNK_SIZE) 
                    {
                        if(chunk->blocks[nx][ny][nz].type != Air) {
                            continue;
                        }
                    }
                    else 
                    {
                        nx = nx < 0 ? CHUNK_SIZE - 1 : nx % CHUNK_SIZE;
                        ny = ny < 0 ? CHUNK_SIZE - 1 : ny % CHUNK_SIZE;
                        nz = nz < 0 ? CHUNK_SIZE - 1 : nz % CHUNK_SIZE;

                        Chunk* neighbouring_chunk = world_get_chunk(
                            world,
                            (i16x3) {
                                chunk_pos[0] + dx,
                                chunk_pos[1] + dy,
                                chunk_pos[2] + dz
                            }
                        );

                        printf("[0x%p] { %i, %i, %i }, from { %i, %i, %i }, block: { %i, %i, %i } type: %i\n",
                               neighbouring_chunk,
                               chunk_pos[0] + dx,
                               chunk_pos[1] + dy,
                               chunk_pos[2] + dz,
                               chunk_pos[0],
                               chunk_pos[1],
                               chunk_pos[2],
                               dx, dy, dz,
                               /*neighbouring_chunk->blocks[nx][ny][nz].type*/ 0);

                        if(neighbouring_chunk && neighbouring_chunk->blocks[nx][ny][nz].type != Air) {
                            continue;
                        }
                    }

                    for(int j = 0; j < 6; j++)
                    {
                        // vertex positions
                        *(buffer++) = vertices[i * 6 + j][0] / 2 + x;
                        *(buffer++) = vertices[i * 6 + j][1] / 2 + y;
                        *(buffer++) = vertices[i * 6 + j][2] / 2 + z;

                        // texture coordinates
                        *(buffer++) = tex_coords[j % 6][0];
                        *(buffer++) = tex_coords[j % 6][1];

                        // texture index
                        *(buffer++) = block_get_texture_from_block_face(block.type, (FaceDirection)i);

                        chunk->buffer_triangles++;
                    }
                }
			}
		}
	}

    for(int i = 0; i < 36; i++)
    {
        break;
        printf("{ %f, %f, %f }, { %f, %f, %f }\n",
               chunk->buffer[i * TRIANGLE_SIZE],
               chunk->buffer[i * TRIANGLE_SIZE + 1],
               chunk->buffer[i * TRIANGLE_SIZE + 2],
               chunk->buffer[i * TRIANGLE_SIZE + 3],
               chunk->buffer[i * TRIANGLE_SIZE + 4],
               chunk->buffer[i * TRIANGLE_SIZE + 5]);
    }
}

void chunk_render(Chunk* chunk)
{
    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, chunk->buffer_triangles * TRIANGLE_BYTES, chunk->buffer, GL_STATIC_DRAW);

    // vertex positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, TRIANGLE_BYTES, 0);
    glEnableVertexAttribArray(0);

    // texture coordinates (offset by vertex positions)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, TRIANGLE_BYTES, (void*)sizeof(vec3));
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLES, 0, chunk->buffer_triangles);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glDeleteBuffers(1, &vertex_buffer);
}

