#pragma once

#include "../Shapes/Vec2.h"
#include "../Shapes/Box2.h"

namespace ls
{
    namespace gl
    {
        class Font
        {
        public:
            virtual ls::Box2F glyphTexBounds(char c) const = 0;
            virtual ls::Vec2F glyphTexCoords(char c) const = 0;
            virtual ls::Vec2F glyphTexSize(char c) const = 0;

            virtual void bind() const = 0;
        };
    }
}
