#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <string>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_PSD
#define STBI_NO_TGA
#define STBI_NO_GIF
#define STBI_NO_HDR
#define STBI_NO_PIC
#define STBI_NO_PNM
#define STB_IMAGE_STATIC
#include "detail/stb_image.h"

#include "Bindable.h"

namespace ls
{
    namespace gl
    {
        class Texture2 : public TextureBindable<GL_TEXTURE_2D>
        {
            using Binder = TextureBindable<GL_TEXTURE_2D>;
        public:
            Texture2()
            {
                glGenTextures(1, &m_id);
            }
            Texture2(int width, int height, GLenum internalFormat, const uint8_t* data) :
                Texture2()
            {
                bind(GL_TEXTURE0);
                glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                m_width = width;
                m_height = height;
            }
            Texture2(const char* path, GLenum internalFormat) :
                Texture2()
            {
                load(path, internalFormat);
            }
            Texture2(const Texture2&) = delete;
            Texture2(Texture2&& other) noexcept :
                m_id(other.m_id),
                m_width(other.m_width),
                m_height(other.m_height)
            {
                other.m_id = m_nullId;
            }
            Texture2& operator=(const Texture2&) = delete;
            Texture2& operator=(Texture2&& other) noexcept
            {
                cleanup();
                m_id = other.m_id;
                other.m_id = m_nullId;

                m_width = other.m_width;
                m_height = other.m_height;

                return *this;
            }
            ~Texture2()
            {
                cleanup();
            }

            void load(const char* path, GLenum internalFormat)
            {
                stbi_set_flip_vertically_on_load(true);
                int depth;
                unsigned char *data = stbi_load(path, &m_width, &m_height, &depth, 4);
                bind();
                glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
            }

            void setMinFilter(GLint filter) const
            {
                bind();
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
            }
            void setMagFilter(GLint filter) const
            {
                bind();
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
            }
            void generateMipmaps() const
            {
                bind();
                glGenerateMipmap(GL_TEXTURE_2D);
            }
            void setHorizontalWrap(GLint mode) const
            {
                bind();
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode);
            }
            void setVerticalWrap(GLint mode) const
            {
                bind();
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode);
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

            void bind(GLenum unit) const
            {
                setActiveTexture(unit);
                Binder::ensureBond(m_id);
            }
            void unbind(GLenum unit) const
            {
                setActiveTexture(unit);
                Binder::ensureBond(0);
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
                    Binder::deleteTexture(m_id);
                }
                m_id = m_nullId;
            }
        };
    }
}