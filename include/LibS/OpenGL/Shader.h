#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <string>

#include "LibS/Matrix.h"
#include "LibS/Shapes/Vec2.h"
#include "LibS/Shapes/Vec3.h"
#include "LibS/Shapes/Vec4.h"

#include "Bindable.h"
#include "LibS/FileUtil.h"

namespace ls
{
    namespace gl
    {
        struct ShaderLoadResult
        {
            bool success;
            std::string compilationLog;
        };

        struct ShaderLinkResult
        {
            bool success;
            std::string linkLog;
        };
        class ProgramUniformView
        {
            using ShaderProgramBinder = ShaderProgramBindable;
        public:
            ProgramUniformView(GLuint programId, const char* name) :
                m_program(programId),
                m_location(glGetUniformLocation(programId, name))
            {
            }

            // parent program must be in use
            void set(const ls::Matrix4x4F& matrix) const
            {
                ShaderProgramBinder::ensureBond(m_program);
                glUniformMatrix4fv(m_location, 1, GL_TRUE, matrix.data());
            }
            void set(int i) const
            {
                ShaderProgramBinder::ensureBond(m_program);
                glUniform1i(m_location, i);
            }
            void set(float f) const
            {
                ShaderProgramBinder::ensureBond(m_program);
                glUniform1f(m_location, f);
            }
            void set(const ls::Vec2F& v) const
            {
                ShaderProgramBinder::ensureBond(m_program);
                glUniform2f(m_location, v.x, v.y);
            }
            void set(const ls::Vec3F& v) const
            {
                ShaderProgramBinder::ensureBond(m_program);
                glUniform3f(m_location, v.x, v.y, v.z);
            }
            void set(const ls::Vec4F& v) const
            {
                ShaderProgramBinder::ensureBond(m_program);
                glUniform4f(m_location, v.x, v.y, v.z, v.w);
            }

        private:
            GLuint m_program;
            GLuint m_location;
        };

        template <GLenum ShaderType>
        class Shader
        {
        public:

            Shader() :
                m_id(glCreateShader(ShaderType))
            {

            }
            Shader(const Shader<ShaderType>&) = delete;
            Shader(Shader<ShaderType>&& other) noexcept :
                m_id(other.m_id)
            {
                other.m_id = m_nullId;
            }
            Shader& operator=(const Shader<ShaderType>&) = delete;
            Shader& operator=(Shader<ShaderType>&& other) noexcept
            {
                cleanup();

                m_id = other.m_id;
                other.m_id = m_nullId;

                return *this;
            }
            ~Shader()
            {
                cleanup();
            }

            void loadFromFile(const char* path, ShaderLoadResult* result = nullptr)
            {
                const std::string code = readFile(path);
                if (code.empty())
                {
                    throw std::runtime_error(std::string("Couldn't load shader from ") + path);
                }

                GLint compilationSuccess = GL_FALSE;
                int infoLogLength;

                char const* codePtr = code.c_str();
                glShaderSource(m_id, 1, &codePtr, nullptr);
                glCompileShader(m_id);

                if (result)
                {
                    glGetShaderiv(m_id, GL_COMPILE_STATUS, &compilationSuccess);
                    glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &infoLogLength);
                    if (infoLogLength > 0) {
                        result->compilationLog.resize(infoLogLength);
                        glGetShaderInfoLog(m_id, infoLogLength, nullptr, result->compilationLog.data());
                    }

                    result->success = compilationSuccess == GL_TRUE ? true : false;
                }
            }

            GLuint release()
            {
                GLuint id = m_id;
                m_id = m_nullId;
                return id;
            }
        private:
            GLuint m_id;

            static constexpr GLuint m_nullId = 0;

            void cleanup()
            {
                if (m_id != m_nullId)
                {
                    glDeleteShader(m_id);
                    m_id = m_nullId;
                }
            }
        };


        class ShaderProgram : public ShaderProgramBindable
        {
            using Binder = ShaderProgramBindable;

            friend class ShaderProgramBuilder;
        public:
            ShaderProgram() :
                m_id(m_nullId)
            {

            }
            ShaderProgram(const ShaderProgram&) = delete;
            ShaderProgram(ShaderProgram&& other) noexcept :
                m_id(other.m_id)
            {
                other.m_id = m_nullId;
            }
            ShaderProgram& operator=(const ShaderProgram&) = delete;
            ShaderProgram& operator=(ShaderProgram&& other) noexcept
            {
                cleanup();

                m_id = other.m_id;
                other.m_id = m_nullId;

                return *this;
            }
            ~ShaderProgram()
            {
                cleanup();
            }

            void bind() const
            {
                Binder::ensureBond(m_id);
            }

            GLuint id() const
            {
                return m_id;
            }

            ProgramUniformView uniformView(const char* name) const
            {
                return ProgramUniformView(m_id, name);
            }

            void clear()
            {
                cleanup();
            }

        private:
            GLuint m_id;

            static constexpr GLuint m_nullId = 0;

            ShaderProgram(GLuint id) :
                m_id(id)
            {

            }

            void cleanup()
            {
                if (m_id != m_nullId)
                {
                    Binder::deleteShaderProgram(m_id);
                    m_id = m_nullId;
                }
            }
        };

        class ShaderProgramBuilder
        {
        public:
            ShaderProgramBuilder() = default;

            ShaderProgramBuilder(const ShaderProgramBuilder&) = delete;
            ShaderProgramBuilder& operator=(const ShaderProgramBuilder&) = delete;
            ShaderProgramBuilder(ShaderProgramBuilder&& other) noexcept :
                m_shaders(std::move(other.m_shaders))
            {

            }

            ShaderProgramBuilder& operator=(ShaderProgramBuilder&& other) noexcept
            {
                cleanup();
                m_shaders = std::move(other.m_shaders);
            }

            ~ShaderProgramBuilder()
            {
                cleanup();
            }

            template <GLenum ShaderType>
            void addShaderFromFile(const char* path, ShaderLoadResult* result = nullptr)
            {
                Shader<ShaderType> shader;
                shader.loadFromFile(path, result);
                m_shaders.emplace_back(shader.release());
            }

            ShaderProgram linkAndClear(ShaderLinkResult* result = nullptr)
            {
                ShaderProgram program = link(result);

                clear();

                return program;
            }

            ShaderProgram link(ShaderLinkResult* result = nullptr)
            {
                GLint linkResult = GL_FALSE;
                int linkInfoLogLength;
                // Link the program
                GLuint programId = glCreateProgram();
                for (GLuint shaderId : m_shaders)
                {
                    glAttachShader(programId, shaderId);
                }
                glLinkProgram(programId);

                if (result)
                {
                    // Check the program
                    glGetProgramiv(programId, GL_LINK_STATUS, &linkResult);
                    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &linkInfoLogLength);
                    if (linkInfoLogLength > 0) {
                        result->linkLog.resize(linkInfoLogLength);
                        glGetProgramInfoLog(programId, linkInfoLogLength, nullptr, result->linkLog.data());
                    }

                    result->success = linkResult == GL_TRUE ? true : false;
                }

                for (GLuint shaderId : m_shaders)
                {
                    glDetachShader(programId, shaderId);
                }

                return ShaderProgram(programId);
            }

            void clear()
            {
                cleanup();
            }

        private:
            std::vector<GLuint> m_shaders;

            void cleanup()
            {
                for (GLuint id : m_shaders)
                {
                    glDeleteShader(id);
                }
            }
        };
    }
}
