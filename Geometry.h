#ifndef GEOMETRY_H_INCLUDED
#define GEOMETRY_H_INCLUDED

#include <vector>
#include <cmath>
#include <utility>
#include <functional>
#include <algorithm>
#include "Array2.h"
#include "Random.h"

//maybe will be renamed to Shapes and some functions will be extracted to other files
namespace Geo
{
    namespace ___internal
    {
        template <class Type, Type...>
        struct is_one_of;

        template <class Type, Type F>
        struct is_one_of<Type, F>
        {
            static constexpr bool value = false;
        };

        template <class Type, Type F, Type S, Type... T>
        struct is_one_of<Type, F, S, T...>
        {
            static constexpr bool value = F == S
                                          || is_one_of<Type, F, T...>::value;
        };

        template <class Type, Type...>
        struct are_unique;

        template <class Type>
        struct are_unique<Type>
        {
            static constexpr bool value = true;
        };

        template<class Type, Type F, Type... T>
        struct are_unique<Type, F, T...>
        {
            static constexpr bool value = are_unique<Type, T...>::value
                                          && !is_one_of<Type, F, T...>::value;
        };


        template <typename T, template <typename...> class Template>
        struct is_specialization_of
        {
            static constexpr bool value = false;
        };

        template <typename... Args, template <typename...> class Template>
        struct is_specialization_of<Template<Args...>, Template>
        {
            static constexpr bool value = true;
        };
    }
    /*   UTILITY   */
    constexpr double PI = 3.14159265359;

    inline double radiansToDegrees(const double radians)
    {
        return radians * (180.0 / PI);
    }
    inline double degreesToRadians(const double degrees)
    {
        return degrees * (PI / 180.0);
    }
    template<class T>
    T min(const T& a, const T& b)
    {
        if(a < b) return a;
        return b;
    }
    template<class T>
    T max(const T& a, const T& b)
    {
        if(a > b) return a;
        return b;
    }
    inline double clamp(int a, int mi, int ma)
    {
        return max(min(a, ma), mi);
    }
    /*   UTILITY END  */
    template <class T> class Angle;
    template <class T> class Shape;
    template <class T> class Shape2;
    template <class T> class Shape3;
    template <class T> class Shape4;
    template <class T> class Vec2;
    template <class T> class Vec3;
    template <class T> class Vec4;
    template <class T, size_t C, size_t R> class Matrix;
    template <class T> class Rectangle;
    template <class T> class Cuboid;
    template <class T> class Circle;
    template <class T> class Cylinder;
    template <class T> class LineSegment;
    template <class T> class Ray;
    template <class T> class Triangle;
    template <class T> class Polygon;
    template <class T> class Polyline;
    template <class T> class Mesh;

    class Intersections;
    template <class T> class Raycast;
    class Raycaster;

    template <class T> class RandomShapePicker;
    template <class T> class ConvexHull;
    template <class T> class Transformation2;
    template <class T> class AffineTransformation;
    template <class T> class BezierCurve;
    template <class T> class BezierPath;
    template <class T> class Matrix3x3;
    template <class T> class RapidlyExploringRandomTree;

    /*         HEADERS         */

#include "include/Angle.h"
#include "include/Shape.h"
#include "include/Shape2.h"
#include "include/Shape3.h"
#include "include/Shape4.h"
#include "include/Vec2.h"
#include "include/Vec3.h"
#include "include/Vec4.h"
#include "include/Matrix.h"
#include "include/Rectangle.h"
#include "include/Cuboid.h"
#include "include/Circle.h"
#include "include/Cylinder.h"
#include "include/LineSegment.h"
#include "include/Ray.h"
#include "include/Triangle.h"
#include "include/Polygon.h"
#include "include/Polyline.h"
#include "include/Mesh.h"

#include "include/Intersections.h"
#include "include/Raycast.h"
#include "include/Raycaster.h"

#include "include/RandomShapePicker.h"
#include "include/ConvexHull.h"
#include "include/Transformation2.h"
#include "include/AffineTransformation.h"
#include "include/BezierCurve.h"
#include "include/BezierPath.h"
#include "include/Matrix3x3.h"
#include "include/RapidlyExploringRandomTree.h"

}
#endif // GEOMETRY_H_INCLUDED
