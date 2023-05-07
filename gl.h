#include <gl/gl.h>

typedef unsigned int GLenum;
typedef unsigned char GLboolean;
typedef unsigned int GLbitfield;
typedef void GLvoid;
typedef U8 GLubyte;
typedef int GLint;
typedef unsigned int GLuint;
typedef int GLsizei;
typedef F32 GLfloat;
typedef char GLchar;
typedef U64 GLsizeiptr;

#define GLDECL WINAPI

#define GL_ARRAY_BUFFER 0x8892
#define GL_READ_ONLY 0x88B8
#define GL_WRITE_ONLY 0x88B9
#define GL_READ_WRITE 0x88BA
#define GL_BUFFER_ACCESS 0x88BB
#define GL_BUFFER_MAPPED 0x88BC
#define GL_BUFFER_MAP_POINTER 0x88BD
#define GL_STREAM_DRAW 0x88E0
#define GL_STREAM_READ 0x88E1
#define GL_STREAM_COPY 0x88E2
#define GL_STATIC_DRAW 0x88E4
#define GL_STATIC_READ 0x88E5
#define GL_STATIC_COPY 0x88E6
#define GL_DYNAMIC_DRAW 0x88E8
#define GL_DYNAMIC_READ 0x88E9
#define GL_DYNAMIC_COPY 0x88EA
#define GL_COMPILE_STATUS 0x8B81
#define GL_VERTEX_SHADER 0x8B31
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_LINK_STATUS 0x8B82
#define GL_TEXTURE_2D_ARRAY 0x8C1A
#define GL_TEXTURE0 0x84C0

typedef I64 GLintptr;

#define OPENGL_FUNCTIONS \
/* ret, name, params */ \
GLFUNC(GLuint, CreateShader, GLenum shaderType) \
GLFUNC(void, ShaderSource, GLuint shader, GLsizei count, const GLchar **string, const GLint *length) \
GLFUNC(void, CompileShader, GLuint shader) \
GLFUNC(void, GetShaderiv, GLuint shader, GLenum pname, GLint *params) \
GLFUNC(void, GetShaderInfoLog, GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog) \
GLFUNC(GLuint, CreateProgram, void) \
GLFUNC(void, AttachShader, GLuint program, GLuint shader) \
GLFUNC(void, LinkProgram, GLuint program) \
GLFUNC(void, GetProgramiv, GLuint program, GLenum pname, GLint *params) \
GLFUNC(void, GetProgramInfoLog, GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog) \
GLFUNC(void, DeleteShader, GLuint shader) \
GLFUNC(void, UseProgram, GLuint program) \
GLFUNC(void, BindVertexArray, GLuint array) \
GLFUNC(void, TexStorage2D, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height) \
GLFUNC(void, VertexAttribPointer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void * pointer) \
GLFUNC(void, VertexAttribIPointer, GLuint index, GLint size, GLenum type, GLsizei stride, const void * pointer) \
GLFUNC(void, GenVertexArrays, GLsizei n, GLuint *arrays) \
GLFUNC(void, GenBuffers, GLsizei n, GLuint * buffers) \
GLFUNC(void, BindBuffer, GLenum target, GLuint buffer) \
GLFUNC(void, BufferData, GLenum target, GLsizeiptr size, const void * data, GLenum usage) \
GLFUNC(void, BufferSubData, GLenum target, GLintptr offset, GLsizeiptr size, const void * data) \
GLFUNC(void, EnableVertexAttribArray, GLuint index) \
GLFUNC(void, ActiveTexture, GLenum texture) \
GLFUNC(void, Uniform4fv, GLint location, GLsizei count, const GLfloat *value) \
GLFUNC(void, UniformMatrix4fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) \

/* end */

// NOTE(Edvard): typedef GLuint CreateShaderproc(GLenum shaderType);
#define GLFUNC(ret, name, ...) typedef ret GLDECL name##proc(__VA_ARGS__);
OPENGL_FUNCTIONS
#undef GLFUNC


// NOTE(Edvard): CreateShaderproc* glCreateShader;
#define GLFUNC(ret, name, ...) name##proc * gl##name;
OPENGL_FUNCTIONS
typedef struct GLFunctions
{
    OPENGL_FUNCTIONS
} GLFunctions;
#undef GLFUNC

typedef B32 InitGLFunc(struct GLFunctions* functions);

inline B32 initOpenGLFunctions(InitGLFunc* initGL)  // NOTE(Edvard): temp fix
{
    GLFunctions functions = {0};
    B32 result = initGL(&functions);
    
#define GLFUNC(ret, name, ...) gl##name = functions.gl##name;
    OPENGL_FUNCTIONS
#undef GLFUNC
    
    return result;
}
