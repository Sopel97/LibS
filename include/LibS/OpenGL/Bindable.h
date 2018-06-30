#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <array>
#include <vector>

namespace ls
{
    namespace gl
    {
        class ShaderProgramBindable
        {
            friend class ProgramUniformView;
        protected:
            static bool isBound(GLuint id)
            {
                return id == currentBond();
            }

            static void deleteShaderProgram(GLuint id)
            {
                glDeleteProgram(id);
                if (id == currentBond())
                {
                    currentBond() = 0;
                }
            }
            static void bindShaderProgram(GLuint id)
            {
                glBindRenderbuffer(GL_RENDERBUFFER, id);
                currentBond() = id;
            }
            static void ensureBond(GLuint id)
            {
                if (isBound(id)) return;

                glUseProgram(id);
            }

        private:
            static GLuint& currentBond()
            {
                static thread_local GLuint current = 0u;

                return current;
            }
        };

        class RenderbufferBindable
        {
        protected:
            static bool isBound(GLuint id)
            {
                return id == currentBond();
            }

            static void deleteRenderbuffer(GLuint id)
            {
                glDeleteRenderbuffers(1, &id);
                if (id == currentBond())
                {
                    currentBond() = 0;
                }
            }
            static void bindRenderbuffer(GLuint id)
            {
                glBindRenderbuffer(GL_RENDERBUFFER, id);
                currentBond() = id;
            }
            static void ensureBond(GLuint id)
            {
                if (isBound(id)) return;

                bindRenderbuffer(id);
            }

        private:

            static GLuint& currentBond()
            {
                static thread_local GLuint current = 0u;

                return current;
            }
        };

        class FramebufferBindable
        {
        protected:
            static bool isBound(GLuint id)
            {
                return id == currentBond();
            }

            static void deleteFramebuffer(GLuint id)
            {
                glDeleteFramebuffers(1, &id);
                if (id == currentBond())
                {
                    currentBond() = 0;
                }
            }
            static void bindFramebuffer(GLuint id)
            {
                glBindFramebuffer(GL_FRAMEBUFFER, id);
                currentBond() = id;
            }
            static void ensureBond(GLuint id)
            {
                if (isBound(id)) return;

                bindFramebuffer(id);
            }

        private:

            static GLuint& currentBond()
            {
                static thread_local GLuint current = 0u;

                return current;
            }
        };

        class VertexArrayBindable
        {
        protected:
            static bool isBound(GLuint id)
            {
                return id == currentBond();
            }

            static void deleteVertexArray(GLuint id)
            {
                glDeleteVertexArrays(1, &id);
                if (id == currentBond())
                {
                    currentBond() = 0;
                }
            }
            static void bindVertexArray(GLuint id)
            {
                glBindVertexArray(id);
                currentBond() = id;
            }
            static void ensureBond(GLuint id)
            {
                if (isBound(id)) return;

                bindVertexArray(id);
            }

        private:

            static GLuint& currentBond()
            {
                static thread_local GLuint current = 0u;

                return current;
            }
        };

        template <GLenum Target>
        class BufferBindable
        {
        protected:
            static bool isBound(GLuint id)
            {
                return id == currentBond();
            }

            static void deleteBuffer(GLuint id)
            {
                glDeleteBuffers(1, &id);
                if (currentBond() == id)
                {
                    currentBond() = 0;
                }
            }
            static void bindBuffer(GLuint id)
            {
                glBindBuffer(Target, id);
                currentBond() = id;
            }
            static void ensureBond(GLuint id)
            {                
                // TODO: somehow desyncs from opengl (at least for IBOs).
                //if (isBound(id)) return;

                bindBuffer(id);
            }

        private:
            static GLuint& currentBond()
            {
                static thread_local GLuint current = 0;

                return current;
            }
        };

        class TextureBindableBase
        {
        public:
            static GLuint& currentUnit()
            {
                static thread_local GLuint current = 0;

                return current;
            }
        };

        template <GLenum Target>
        class TextureBindable : public TextureBindableBase
        {
            friend void setActiveTexture(GLenum unit);

        protected:
            static bool isBound(GLuint id)
            {
                return id == currentUnitBond();
            }

            static void deleteTexture(GLuint id)
            {
                glDeleteTextures(1, &id);
                for (auto& c : currentBonds())
                {
                    if (c == id)
                    {
                        c = 0;
                    }
                }
            }
            static void bindTexture(GLuint id)
            {
                glBindTexture(Target, id);
                currentUnitBond() = id;
            }
            static void ensureBond(GLuint id)
            {
                if (isBound(id)) return;

                bindTexture(id);
            }

        private:
            static GLuint& currentUnitBond()
            {
                return currentBonds()[currentUnit()];
            }

            static std::vector<GLuint>& currentBonds()
            {
                static thread_local std::vector<GLuint> current = init();

                return current;
            }

            static std::vector<GLuint> init()
            {
                GLint numUnits;
                glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &numUnits);

                return std::vector<GLuint>(numUnits, 0);
            }

        };

        inline void setActiveTexture(GLenum unit)
        {
            static thread_local GLenum current = 0;

            if (unit == current) return;

            TextureBindableBase::currentUnit() = unit - GL_TEXTURE0;
            glActiveTexture(unit);
        }
    }
}
