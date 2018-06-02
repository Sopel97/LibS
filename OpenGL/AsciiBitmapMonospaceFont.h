#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include "Texture2.h"
#include "Font.h"

#include "../Shapes/Box2.h"
#include "../Shapes/Vec2.h"

namespace ls
{
    namespace gl
    {
        class AsciiBitmapMonospaceFont : public Font
        {
        public:
            AsciiBitmapMonospaceFont() = default;
            AsciiBitmapMonospaceFont(const char* path, int glyphWidth, int glyphHeight, int glyphsPerLine)
            {
                load(path, glyphWidth, glyphHeight, glyphsPerLine);
            }

            void load(const char* path, int glyphWidth, int glyphHeight, int glyphsPerLine)
            {
                m_texture.load(path, GL_RGBA);
                m_texture.setMagFilter(GL_NEAREST);

                // NOTE: may be better without mipmaps and just nearest, currently does not work well with transaprency
                m_texture.setMinFilter(GL_LINEAR_MIPMAP_LINEAR);
                m_texture.generateMipmaps();

                const ls::Vec2F textureSize(static_cast<float>(m_texture.width()), static_cast<float>(m_texture.height()));
                m_glyphWidth = glyphWidth / textureSize.x;
                m_glyphHeight = glyphHeight / textureSize.y;
                m_glyphsPerLine = glyphsPerLine;
            }

            ls::Box2F glyphTexBounds(char c) const override
            {
                const ls::Vec2F posNorm = glyphTexCoords(c);
                const ls::Vec2F glyphSizeNorm = ls::Vec2F(m_glyphWidth, m_glyphHeight);

                const ls::Vec2F texPosMinNorm = posNorm;
                const ls::Vec2F texPosMaxNorm = texPosMinNorm + glyphSizeNorm;

                return ls::Box2F(texPosMinNorm, texPosMaxNorm);
            }
            ls::Vec2F glyphTexCoords(char c) const override
            {
                const ls::Vec2I glyphPosOnGrid = glyphPositionOnGrid(c);
                return ls::Vec2F(glyphPosOnGrid.x * m_glyphWidth, 1.0f - ((glyphPosOnGrid.y + 1) * m_glyphHeight));
            }
            ls::Vec2F glyphTexSize(char c) const override
            {
                return ls::Vec2F(m_glyphWidth, m_glyphHeight);
            }
            void bind() const override
            {
                m_texture.bind(GL_TEXTURE0);
            }

        private:
            Texture2 m_texture;
            float m_glyphWidth;
            float m_glyphHeight;
            int m_glyphsPerLine;

            ls::Vec2I glyphPositionOnGrid(char c) const
            {
                const int index = static_cast<int>(c);
                return { index % m_glyphsPerLine, index / m_glyphsPerLine };
            }
        };
    }
}
