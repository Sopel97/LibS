#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

namespace ls
{
    namespace gl
    {
        namespace detail
        {
            template<typename T, typename U> constexpr size_t offsetOf(U T::*member)
            {
                return reinterpret_cast<const char*>(&(static_cast<const T*>(0)->*member)) - static_cast<const char*>(nullptr);
            }
        }

        template <class T>
        class VertexAttribute
        {
        public:
            VertexAttribute(GLuint index, GLint size, GLenum type, GLboolean isNormalized, const GLvoid* offset) :
                m_index(index),
                m_size(size),
                m_type(type),
                m_isNormalized(isNormalized),
                m_offset(offset)
            {
            }

            void apply() const
            {
                glVertexAttribPointer(
                    m_index,
                    m_size,
                    m_type,
                    m_isNormalized,
                    sizeof(T),
                    m_offset
                );
            }

            GLuint index() const
            {
                return m_index;
            }
            GLint size() const
            {
                return m_size;
            }
            GLenum type() const
            {
                return m_type;
            }
            GLboolean isNormalized() const
            {
                return m_isNormalized;
            }
            const GLvoid* offset() const
            {
                return m_offset;
            }

        private:
            GLuint m_index;
            GLint m_size;
            GLenum m_type;
            GLboolean m_isNormalized;
            const GLvoid* m_offset;
        };

        template <class VertexType, class AttrType>
        VertexAttribute<VertexType> makeVertexAttribute(GLuint index, AttrType VertexType::*attrPtr, GLint size, GLenum type, GLboolean isNormalized)
        {
            return VertexAttribute<VertexType>(index, size, type, isNormalized, static_cast<const char*>(0) + detail::offsetOf(attrPtr));
        }
    }
}
