#pragma once

#include <string>

#include "Font.h"
#include "VertexArrayObject.h"

#include "../Shapes/Vec3.h"

namespace ls
{
    namespace gl
    {
        class Text
        {
        public:
            struct Vertex
            {
                ls::Vec3F pos;
                ls::Vec2F uv;
            };

            Text() :
                Text(nullptr, "")
            {
            }
            Text(const Font& font) :
                Text(&font, "")
            {
            }
            Text(const Font& font, std::string text) :
                Text(&font, std::move(text))
            {
            }

            void setText(std::string text)
            {
                m_text = std::move(text);
                m_isVaoUpdated = false;
                m_isSizeUpdated = false;
            }
            void setFont(const Font& font)
            {
                m_font = &font;
                m_isVaoUpdated = false;
                m_isSizeUpdated = false;
            }
            void setTabSizeInSpaces(int size)
            {
                m_tabSizeInSpaces = size;
                m_isVaoUpdated = false;
                m_isSizeUpdated = false;
            }
            void setLineSpacing(float size)
            {
                m_lineSpacing = size;
                m_isVaoUpdated = false;
                m_isSizeUpdated = false;
            }
            void setGlyphHeight(float height)
            {
                m_glyphHeight = height;
                m_isVaoUpdated = false;
                m_isSizeUpdated = false;
            }

            void draw()
            {
                m_font->bind();

                if (m_vbo == nullptr)
                {
                    m_vbo = &(m_vao.createVertexBufferObject());
                    m_vao.setVertexAttribute(*m_vbo, 0, &Vertex::pos, 3, GL_FLOAT, GL_FALSE);
                    m_vao.setVertexAttribute(*m_vbo, 1, &Vertex::uv, 2, GL_FLOAT, GL_FALSE);
                }
                if (m_ibo == nullptr)
                {
                    m_ibo = &(m_vao.createIndexBufferObject());
                }

                if (!m_isVaoUpdated)
                {
                    updateVao();
                }

                m_vao.drawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_SHORT);
            }

            const ls::Vec2F& size()
            {
                if (!m_isSizeUpdated)
                {
                    updateSize();
                }

                return m_size;
            }


        private:
            const Font* m_font;
            std::string m_text;
            VertexArrayObject m_vao;
            VertexBufferObject* m_vbo;
            IndexBufferObject* m_ibo;
            int m_numIndices;
            int m_tabSizeInSpaces;
            float m_lineSpacing;
            float m_glyphHeight;
            ls::Vec2F m_size;
            bool m_isVaoUpdated;
            bool m_isSizeUpdated;

            struct Geometry
            {
                std::vector<Vertex> vertices;
                std::vector<unsigned short> indices;
            };

            static constexpr int m_defaultTabSize = 4;
            static constexpr float m_defaultLineSpacing = 0.0f;
            static constexpr float m_defaultGlypthHeight = 1.0f;

            Text(const Font* font, std::string text) :
                m_font(font),
                m_text(std::move(text)),
                m_vbo(nullptr),
                m_ibo(nullptr),
                m_numIndices(0),
                m_tabSizeInSpaces(m_defaultTabSize),
                m_lineSpacing(m_defaultLineSpacing),
                m_glyphHeight(m_defaultGlypthHeight),
                m_isVaoUpdated(false),
                m_isSizeUpdated(false)
            {

            }

            Geometry generateGeometry() const
            {
                static constexpr int maxNumVerticesPerChar = 4;
                static constexpr int maxNumIndicesPerChar = 6;

                if (!m_text.empty() && m_font)
                {
                    std::vector<Vertex> vertices;
                    std::vector<unsigned short> indices;

                    vertices.reserve(maxNumVerticesPerChar * m_text.size());
                    indices.reserve(maxNumIndicesPerChar * m_text.size());

                    const int numLines = countLinesInText();
                    const float glyphHeight = m_glyphHeight;
                    const float lineSpacing = m_lineSpacing;
                    float x = 0.0f;
                    float y = (glyphHeight + lineSpacing) * (numLines - 1); // initial y, will go down in the loop
                    int currentFirstGlyphVertexIndex = 0;
                    for (char c : m_text)
                    {
                        float glyphScaleX = 1.0f;

                        if (c == '\n')
                        {
                            x = 0.0f;
                            y -= glyphHeight + lineSpacing;
                            continue;
                        }
                        else if (c == '\t')
                        {
                            c = ' ';
                            glyphScaleX = static_cast<float>(m_tabSizeInSpaces);
                        }

                        const ls::Box2F uvBounds = m_font->glyphTexBounds(c);

                        const ls::Vec2F min(x, y);
                        const float glyphAspect = uvBounds.width() / uvBounds.height();
                        const ls::Vec2F size(glyphHeight * glyphAspect * glyphScaleX, glyphHeight);
                        const ls::Vec2F max = min + size;

                        vertices.emplace_back(Vertex{ { min.x, min.y, 0.0f },{ uvBounds.min.x, uvBounds.min.y } });
                        vertices.emplace_back(Vertex{ { max.x, min.y, 0.0f },{ uvBounds.max.x, uvBounds.min.y } });
                        vertices.emplace_back(Vertex{ { max.x, max.y, 0.0f },{ uvBounds.max.x, uvBounds.max.y } });
                        vertices.emplace_back(Vertex{ { min.x, max.y, 0.0f },{ uvBounds.min.x, uvBounds.max.y } });

                        indices.emplace_back(currentFirstGlyphVertexIndex + 0);
                        indices.emplace_back(currentFirstGlyphVertexIndex + 1);
                        indices.emplace_back(currentFirstGlyphVertexIndex + 2);

                        indices.emplace_back(currentFirstGlyphVertexIndex + 0);
                        indices.emplace_back(currentFirstGlyphVertexIndex + 2);
                        indices.emplace_back(currentFirstGlyphVertexIndex + 3);

                        currentFirstGlyphVertexIndex += 4;
                        x += size.x;
                    }

                    return Geometry{ std::move(vertices), std::move(indices) };
                }
                else
                {
                    return Geometry{ {},{} };
                }
            }

            int countLinesInText() const
            {
                return std::count(m_text.begin(), m_text.end(), '\n') + 1;
            }

            void updateVao()
            {
                auto geometry = generateGeometry();
                m_numIndices = geometry.indices.size();

                m_vbo->reset(geometry.vertices.data(), geometry.vertices.size(), GL_STATIC_DRAW);
                m_ibo->reset(geometry.indices.data(), geometry.indices.size(), GL_STATIC_DRAW);

                m_isVaoUpdated = true;
            }

            void updateSize()
            {
                using std::max;

                m_size = ls::Vec2F::zero();

                float x = 0.0f;
                float y = 0.0f;

                for (char c : m_text)
                {
                    float glyphScaleX = 1.0f;

                    if (c == '\n')
                    {
                        x = 0.0f;
                        y += m_glyphHeight + m_lineSpacing;
                        continue;
                    }
                    else if (c == '\t')
                    {
                        c = ' ';
                        glyphScaleX = static_cast<float>(m_tabSizeInSpaces);
                    }

                    const ls::Vec2F uvSize = m_font->glyphTexSize(c);

                    const float glyphAspect = uvSize.x / uvSize.y;
                    const ls::Vec2F glyphSize(m_glyphHeight * glyphAspect * glyphScaleX, m_glyphHeight);

                    x += glyphSize.x;
                    m_size.x = max(m_size.x, x);
                    m_size.y = max(m_size.y, y + glyphSize.y);
                }

                m_isSizeUpdated = true;
            }
        };
    }
}