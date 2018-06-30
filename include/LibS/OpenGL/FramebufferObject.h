#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include "Bindable.h"
#include "Texture2.h"
#include "RenderbufferObject.h"

#include <list>

namespace ls
{
    namespace gl
    {
        class FramebufferObject : public FramebufferBindable
        {
            using Binder = FramebufferBindable;

        public:
            FramebufferObject(int width, int height) :
                m_width(width),
                m_height(height)
            {
                glGenFramebuffers(1, &m_id);
            }

            FramebufferObject(const FramebufferObject&) = delete;
            FramebufferObject(FramebufferObject&& other) noexcept :
                m_id(other.m_id),
                m_width(other.m_width),
                m_height(other.m_height),
                m_textures(std::move(other.m_textures)),
                m_rbos(std::move(other.m_rbos))
            {
                other.m_id = m_nullId;
            }
            FramebufferObject& operator=(const FramebufferObject&) = delete;
            FramebufferObject& operator=(FramebufferObject&& other) noexcept
            {
                cleanup();

                m_id = other.m_id;
                m_width = other.m_width;
                m_height = other.m_height;
                m_textures = std::move(other.m_textures);
                m_rbos = std::move(other.m_rbos);

                other.m_id = m_nullId;

                return *this;
            }
            ~FramebufferObject()
            {
                cleanup();
            }
            void bind() const
            {
                Binder::ensureBond(m_id);
            }
            void unbind() const
            {
                Binder::ensureBond(0);
            }

            Texture2& createTextureAttachment(GLenum attachment, GLenum internalFormat)
            {
                auto& texture = createTexture(m_width, m_height, internalFormat);
                bind();
                glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture.id(), 0);
                return texture;
            }
            RenderbufferObject& createRenderbufferAttachment(GLenum attachment, GLenum internalFormat)
            {
                auto& rbo = createRenderbuffer(m_width, m_height, internalFormat);
                bind();
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, rbo.id());
                return rbo;
            }

            GLuint id() const
            {
                return m_id;
            }

            bool isComplete() const
            {
                return (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
            }

        private:
            GLuint m_id;
            int m_width;
            int m_height;
            std::list<Texture2> m_textures;
            std::list<RenderbufferObject> m_rbos;

            static constexpr GLuint m_nullId = 0u;

            void cleanup()
            {
                if (m_id != m_nullId)
                {
                    m_textures.clear();
                    m_rbos.clear();
                    Binder::deleteFramebuffer(m_id);
                }
                m_id = m_nullId;
            }

            Texture2& createTexture(int width, int height, GLenum internalFormat)
            {
                m_textures.emplace_back(width, height, internalFormat, nullptr);
                Texture2& texture = m_textures.back();
                texture.setMagFilter(GL_NEAREST);
                texture.setMinFilter(GL_NEAREST);
                texture.setHorizontalWrap(GL_CLAMP_TO_EDGE);
                texture.setVerticalWrap(GL_CLAMP_TO_EDGE);
                return texture;
            }

            RenderbufferObject& createRenderbuffer(int width, int height, GLenum internalFormat)
            {
                m_rbos.emplace_back(width, height, internalFormat);
                RenderbufferObject& rbo = m_rbos.back();
                return rbo;
            }
        };
    }
}
