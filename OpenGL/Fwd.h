#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

namespace ls
{
    namespace gl
    {
        template <GLuint Target>
        class BufferObject;

        class Camera;

        class AsciiBitmapMonospaceFont;

        class Font;

        class FramebufferObject;

        class RenderbufferObject;

        template <GLenum ShaderType>
        class Shader;
        struct ShaderLoadResult;
        struct ShaderLinkResult;
        class ProgramUniformView;
        class ShaderProgram;
        class ShaderProgramBuilder;

        class Text;

        class Texture2;

        class VertexArrayObject;

        template <class T>
        class VertexAttribute;
    }
}
