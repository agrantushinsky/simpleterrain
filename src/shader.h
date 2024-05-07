#ifndef SHADER_H 
#define SHADER_H 

#include "shared.h" 

#define MAX_SHADERS 8

extern GLuint shader_programs[MAX_SHADERS];

// creates a shader with the type and source passed
void shader_create(GLuint* shader, GLenum shader_type, const char* source);

// compiles a given shader and returns true if successful
bool shader_compile(GLuint* shader);

// reads an entire file given the path and returns a char array
char* read_file(const char* path);

// this function will have all the code for setting up the our shaders
void shaders_init();

// this function will setup a given shader
void shader_init(const char* name, unsigned int id);

#endif

