#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include "Bindable.h"

#include <iostream>

namespace ls
{
    namespace gl
    {
        template <GLuint Target>
        class BufferObject : public BufferBindable<Target>
        {
            using Binder = BufferBindable<Target>;
        public:
            BufferObject()
            {
                glGenBuffers(1, &m_id);
            }
            BufferObject(const BufferObject&) = delete;
            BufferObject(BufferObject&& other) noexcept :
                m_id(other.m_id)
            {
                other.m_id = m_nullId;
            }
            BufferObject& operator=(const BufferObject&) = delete;
            BufferObject& operator=(BufferObject&& other) noexcept
            {
                cleanup();

                m_id = other.m_id;

                other.m_id = m_nullId;
            }
            ~BufferObject()
            {
                cleanup();
            }

            template <class T>
            void reserve(GLsizeiptr size, GLenum usage)
            {
                glBufferData(Target, sizeof(T) * size, nullptr, usage);
            }

            template <class T>
            void reset(const T* data, GLsizeiptr size, GLenum usage) const
            {
                bind();
                glBufferData(Target, sizeof(T) * size, data, usage);
            }

            template <class T>
            void update(const T* data, GLintptr offset, GLsizeiptr size) const
            {
                bind();
                glBufferSubData(Target, offset, sizeof(T) * size, data);
            }

            void bind() const
            {
                Binder::ensureBond(m_id);
            }
            void unbind() const
            {
                Binder::ensureBond(0);
            }

            GLuint id() const
            {
                return m_id;
            }

        private:
            GLuint m_id;

            static constexpr GLuint m_nullId = 0u;

            void cleanup()
            {
                if (m_id != m_nullId)
                {
                    Binder::deleteBuffer(m_id);
                }
                m_id = m_nullId;
            }
        };

        using VertexBufferObject = BufferObject<GL_ARRAY_BUFFER>;
        using IndexBufferObject = BufferObject<GL_ELEMENT_ARRAY_BUFFER>;
    }
}
