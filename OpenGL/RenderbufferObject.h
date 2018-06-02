#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include "Bindable.h"

namespace ls
{
    namespace gl
    {
        class RenderbufferObject : public RenderbufferBindable
        {
            using Binder = RenderbufferBindable;
        public:
            RenderbufferObject()
            {
                glGenRenderbuffers(1, &m_id);
            }
            RenderbufferObject(int width, int height, GLenum internalFormat) :
                RenderbufferObject()
            {
                bind();
                glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);
                m_width = width;
                m_height = height;
            }
            RenderbufferObject(const RenderbufferObject&) = delete;
            RenderbufferObject(RenderbufferObject&& other) noexcept :
                m_id(other.m_id),
                m_width(other.m_width),
                m_height(other.m_height)
            {
                other.m_id = m_nullId;
            }
            RenderbufferObject& operator=(const RenderbufferObject&) = delete;
            RenderbufferObject& operator=(RenderbufferObject&& other) noexcept
            {
                cleanup();

                m_id = other.m_id;
                m_width = other.m_width;
                m_height = other.m_height;

                other.m_id = m_nullId;

                return *this;
            }
            ~RenderbufferObject()
            {
                cleanup();
            }

            int width() const
            {
                return m_width;
            }
            int height() const
            {
                return m_height;
            }

            GLuint id() const
            {
                return m_id;
            }

            void bind() const
            {
                Binder::ensureBond(m_id);
            }

            void unbind() const
            {
                Binder::ensureBond(0);
            }

            void clear()
            {
                cleanup();
            }

        private:
            GLuint m_id;
            int m_width;
            int m_height;

            static constexpr GLuint m_nullId = 0;

            void cleanup()
            {
                if (m_id != m_nullId)
                {
                    Binder::deleteRenderbuffer(m_id);
                }
                m_id = m_nullId;
            }

        };
    }
}
