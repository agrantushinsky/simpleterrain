#include "shader.h"

GLuint shader_programs[MAX_SHADERS];

void shader_create(GLuint* shader, GLenum shader_type, const char* source)
{
    *shader = glCreateShader(shader_type);

    const GLchar* gl_source[1];
    gl_source[0] = source;

    GLint gl_length[1];
    gl_length[0] = strlen(source);

    glShaderSource(*shader, 1, gl_source, gl_length);
}

bool shader_compile(GLuint* shader)
{
    GLint success;
    // first compile the shader
    glCompileShader(*shader); // then, check if the compilation was successful GLint success;
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &success);

    // if the compilation failed, log it
    if (!success)
    {
        GLchar log[1024];
        glGetShaderInfoLog(*shader, sizeof(log), NULL, log);
        printf("Error compiling shader: %s", log);
    }

    return success;
}

char* read_file(const char* path)
{
    FILE* f;
    char* text;
    long len;
    f = fopen(path, "r");
    if (f == NULL) {
        fprintf(stderr, "error loading shader at %s\n", path);
        exit(1);
    }

    fseek(f, 0, SEEK_END);
    len = ftell(f);
    fseek(f, 0, SEEK_SET);
    text = calloc(len + 1, sizeof(char)); // not sure if this + 1 is neccessary
    fread(text, sizeof(char), len, f);
    fclose(f);

    return text;
}

void shader_init(const char* file_name, unsigned int id, GLenum shader_type)
{
    shader_programs[id] = glCreateProgram();

    GLuint shader;

    const char* shader_path_fmt = "../res/%s";
    char shader_path[_MAX_PATH];
    sprintf(&shader_path, shader_path_fmt, file_name);

    char* shader_source = read_file(shader_path);

    shader_create(&shader, shader_type, shader_source);

    shader_compile(&shader);

    free(shader_source);

    glAttachShader(shader_programs[id], shader);

    glLinkProgram(shader_programs[id]);

    // check link status from the line above
    GLint success;
    glGetProgramiv(shader_programs[id], GL_LINK_STATUS, &success);
    if (!success)
    {
        GLchar log[1024];
        glGetProgramInfoLog(shader_programs[id], sizeof(log), NULL, log);
        printf("Error linking shader %s: '%s'\n", file_name, log);
    }

    // TODO: check if this is neccessary
    // specifically in non-debug builds
    glValidateProgram(shader_programs[id]);

    printf("Setup shader: %s\n", file_name);
}

void shaders_init()
{
    shader_init("shader.frag", 0, GL_FRAGMENT_SHADER);
    shader_init("shader.vert", 1, GL_VERTEX_SHADER);
}

