#pragma once

#include "Readers.h"

#include "../Shapes/Vec2.h"
#include "../Shapes/Vec3.h"
#include "../Shapes/Vec4.h"

namespace ls
{
    namespace json
    {
        template <typename T>
        struct Reader<Vec2<T>>
        {
            static Vec2<T> fromJson(const Value& val)
            {
                return Vec2<T>(
                    fromJson<T>(val[0]),
                    fromJson<T>(val[1])
                );
            }
        };

        template <typename T>
        struct Reader<Vec3<T>>
        {
            static Vec3<T> fromJson(const Value& val)
            {
                return Vec3<T>(
                    fromJson<T>(val[0]),
                    fromJson<T>(val[1]),
                    fromJson<T>(val[2])
                );
            }
        };

        template <typename T>
        struct Reader<Vec4<T>>
        {
            static Vec4<T> fromJson(const Value& val)
            {
                return Vec4<T>(
                    fromJson<T>(val[0]),
                    fromJson<T>(val[1]),
                    fromJson<T>(val[2]),
                    fromJson<T>(val[3])
                );
            }
        };
    }
}
