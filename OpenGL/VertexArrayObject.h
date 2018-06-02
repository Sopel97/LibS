#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <list>
#include <memory>

#include "Bindable.h"
#include "BufferObject.h"
#include "VertexAttribute.h"

namespace ls
{
    namespace gl
    {
        class VertexArrayObject : public VertexArrayBindable
        {
            using Binder = VertexArrayBindable;
        public:
            VertexArrayObject()
            {
                glGenVertexArrays(1, &m_id);
            }

            VertexArrayObject(const VertexArrayObject&) = delete;
            VertexArrayObject(VertexArrayObject&& other) noexcept :
                m_id(other.m_id),
                m_vbos(std::move(other.m_vbos)),
                m_ibo(std::move(other.m_ibo))
            {
                other.m_id = m_nullId;
            }
            VertexArrayObject& operator=(const VertexArrayObject&) = delete;
            VertexArrayObject& operator=(VertexArrayObject&& other) noexcept
            {
                cleanup();

                m_id = other.m_id;
                m_vbos = std::move(other.m_vbos);
                m_ibo = std::move(other.m_ibo);

                other.m_id = m_nullId;

                return *this;
            }
            ~VertexArrayObject()
            {
                cleanup();
            }

            template <class T>
            VertexBufferObject& createVertexBufferObject(const T* data, GLsizeiptr size, GLenum usage)
            {
                m_vbos.emplace_back();
                auto& vbo = m_vbos.back();

                vbo.reset(data, size, usage);

                return vbo;
            }


            template <class T>
            IndexBufferObject& createIndexBufferObject(const T* data, GLsizeiptr size, GLenum usage)
            {
                m_ibo = std::make_unique<IndexBufferObject>();
                m_ibo->reset(data, size, usage);

                return *m_ibo;
            }

            VertexBufferObject& createVertexBufferObject()
            {
                m_vbos.emplace_back();
                auto& vbo = m_vbos.back();

                return vbo;
            }
            IndexBufferObject& createIndexBufferObject()
            {
                m_ibo = std::make_unique<IndexBufferObject>();

                return *m_ibo;
            }

            void drawArrays(GLenum mode, GLint first, GLsizei count)
            {
                bind();
                glDrawArrays(mode, first, count);
            }
            void drawArraysInstanced(GLenum mode, GLint first, GLsizei count, GLsizei instCount)
            {
                bind();
                glDrawArraysInstanced(mode, first, count, instCount);
            }

            void drawElements(GLenum mode, GLsizei count, GLenum type)
            {
                bind();
                m_ibo->bind();
                glDrawElements(mode, count, type, nullptr);
            }

            template <class VertexType, class AttrType>
            void setVertexAttribute(const VertexBufferObject& vbo, GLuint index, AttrType VertexType::*attrPtr, GLint size, GLenum type, GLboolean isNormalized) const
            {
                bind();
                vbo.bind();
                makeVertexAttribute(index, attrPtr, size, type, isNormalized).apply();
                glEnableVertexAttribArray(index);
            }

            void setVertexAttribDivisor(GLuint index, GLuint divisor)
            {
                bind();
                glVertexAttribDivisor(index, divisor);
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

            void clear()
            {
                cleanup();
            }
        private:
            GLuint m_id;
            std::list<VertexBufferObject> m_vbos;
            std::unique_ptr<IndexBufferObject> m_ibo;

            static constexpr GLuint m_nullId = 0u;

            void cleanup()
            {
                if (m_id != m_nullId)
                {
                    m_vbos.clear();
                    Binder::deleteVertexArray(m_id);
                }
                m_id = m_nullId;
            }
        };
    }
}