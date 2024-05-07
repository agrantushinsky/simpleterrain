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
    text = calloc(len, sizeof(char) + 1); // not sure if this + 1 is neccessary
    fread(text, sizeof(char), len, f);
    fclose(f);

    return text;
}

void shader_init(const char* name, unsigned int id)
{
    shader_programs[id] = glCreateProgram();

    GLuint vertex_shader;
    GLuint fragment_shader;

    char vertex_path[80], fragment_path[80];
    sprintf(vertex_path, "shaders/%s.vert", name);
    sprintf(fragment_path, "shaders/%s.frag", name);

    char* vertex_shader_source = read_file(vertex_path);
    char* fragment_shader_source = read_file(fragment_path);

    shader_create(&vertex_shader, GL_VERTEX_SHADER, vertex_shader_source);
    shader_create(&fragment_shader, GL_FRAGMENT_SHADER, fragment_shader_source);

    shader_compile(&vertex_shader);
    shader_compile(&fragment_shader);

    free(vertex_shader_source);
    free(fragment_shader_source);

    glAttachShader(shader_programs[id], vertex_shader);
    glAttachShader(shader_programs[id], fragment_shader);

    glLinkProgram(shader_programs[id]);

    // check link status from the line above
    GLint success;
    glGetProgramiv(shader_programs[id], GL_LINK_STATUS, &success);
    if (!success)
    {
        GLchar log[1024];
        glGetProgramInfoLog(shader_programs[id], sizeof(log), NULL, log);
        printf("Error linking shader %s: '%s'\n", name, log);
    }

    // TODO: check if this is neccessary
    // specifically in non-debug builds
    glValidateProgram(shader_programs[id]);

    printf("Setup %s\n", name);
}

void shaders_init()
{
    shader_init("shader", 0);
}

