#pragma once

#include "ShapeIntersectionsCommon.h"

#include "LibS/Shapes.h"

#include <optional>
#include <cmath>

namespace ls
{
    template <typename T>
    bool intersect(const Box2<T>& a, const Box2<T>& b)
    {
        if (a.min.x >= b.max.x) return false;
        if (a.min.y >= b.max.y) return false;
        if (a.max.x <= b.min.x) return false;
        if (a.max.y <= b.min.y) return false;
        return true;
    }
    template <typename T>
    bool intersect(const Box2<T>& a, const Circle2<T>& b)
    {
        using std::clamp;

        const Vec2<T> closestPoint(
            clamp(b.origin.x, a.min.x, a.max.x),
            clamp(b.origin.y, a.min.y, a.max.y)
        );

        const Vec2<T> diff = b.origin - closestPoint;

        return diff.lengthSquared() <= (b.radius * b.radius);
    }
    template <typename T>
    bool intersect(const Circle2<T>& a, const Box2<T>& b)
    {
        return intersect(b, a);
    }
    template <typename T>
    bool intersect(const Vec2<T>& a, const Circle2<T>& b)
    {
        return (b.origin - a).lengthSquared() <= b.radius * b.radius;
    }
    template <typename T>
    bool intersect(const Circle2<T>& a, const Vec2<T>& b)
    {
        return intersect(b, a);
    }

    template <typename T>
    bool intersect(const Vec2<T>& a, const Edge2<T>& b)
    {
        const Vec2<T> t = (b.vertices[1] - b.vertices[0]) / (a - b.vertices[0]);
        if (t.x == t.y)
        {
            if (t.x >= T(0) && t.x < T(1)) return true;
        }
        return false;
    }
    template <typename T>
    bool intersect(const Edge2<T>& a, const Vec2<T>& b)
    {
        return intersect(b, a);
    }
    template <typename T>
    bool intersect(const Vec2<T>& a, const Ray2<T>& b)
    {
        const Vec2<T> t = (b.direction - b.origin) / (a - b.origin);
        if (t.x == t.y)
        {
            if (t.x >= T(0)) return true;
        }
        return false;
    }
    template <typename T>
    bool intersect(const Ray2<T>& a, const Vec2<T>& b)
    {
        return intersect(b, a);
    }
    template <typename T>
    bool intersect(const Vec2<T>& a, const Polyline2<T>& b)
    {
        const int polySize = b.size();
        for (int i = 0; i < polySize - 1; ++i)
        {
            const Vec2<T>& thisVertex = b.vertices[i];
            const Vec2<T>& nextVertex = b.vertices[i + 1];

            if (intersect(Edge2<T>(thisVertex, nextVertex), a)) return true;
        }
        return false;
    }
    template <typename T>
    bool intersect(const Polyline2<T>& a, const Vec2<T>& b)
    {
        return intersect(b, a);
    }
    template <typename T>
    bool intersect(const Circle2<T>& a, const Circle2<T>& b)
    {
        const T radiusSum = a.radius + b.radius;
        return (a.origin - b.origin).lengthSquared() <= radiusSum * radiusSum;
    }

    template <typename T>
    bool intersect(const Edge2<T>& a, const Edge2<T>& b, Vec2<T>& intersectionPoint)
    {
        double s1_x, s1_y, s2_x, s2_y;
        s1_x = a.vertices[1].x - a.vertices[0].x;     s1_y = a.vertices[1].y - a.vertices[0].y;
        s2_x = b.vertices[1].x - b.vertices[0].x;     s2_y = b.vertices[1].y - b.vertices[0].y;

        float s, t;
        s = (-s1_y * (a.vertices[0].x - b.vertices[0].x) + s1_x * (a.vertices[0].y - b.vertices[0].y)) / (-s2_x * s1_y + s1_x * s2_y);
        t = (s2_x * (a.vertices[0].y - b.vertices[0].y) - s2_y * (a.vertices[0].x - b.vertices[0].x)) / (-s2_x * s1_y + s1_x * s2_y);

        if (s >= T(0) && s <= T(1) && t >= T(0) && t <= T(1))
        {
            // Collision detected
            intersectionPoint.x = a.vertices[0].x + (t * s1_x);
            intersectionPoint.y = a.vertices[0].y + (t * s1_y);
            return true;
        }

        return false; // No collision
    }

    template <typename T>
    bool intersect(const Edge2<T>& a, const Edge2<T>& b)
    {
        Vec2<T> intersectionPoint;
        return intersect(a, b, intersectionPoint);
    }

    template <typename T>
    bool intersect(const Triangle2<T>& a, const Vec2<T>& b)
    {
        auto sign = [](const Vec2<T>& v1, const Vec2<T>& v2, const Vec2<T>& v3) -> bool {
            return (v3 - v1).cross(v3 - v2) > T(0);
        };

        bool s1 = sign(b, a.vertices[0], a.vertices[1]);
        bool s2 = sign(b, a.vertices[1], a.vertices[2]);
        bool s3 = sign(b, a.vertices[2], a.vertices[0]);

        return ((s1 == s2) && (s2 == s3));
    }
    template <typename T>
    bool intersect(const Vec2<T>& a, const Triangle2<T>& b)
    {
        return intersect(b, a);
    }
    template <typename T>
    bool intersect(const Triangle2<T>& a, const Triangle2<T>& b) //TODO: improve algorithm
    {
        int i = 0;
        while (i < 3)
        {
            if (intersect(a.vertices[i], b)) return true;
            ++i;
        }
        while (i < 3)
        {
            if (intersect(b.vertices[i], a)) return true;
            ++i;
        }
        for (i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                if (intersect(Edge2<T>(a.vertices[i], a.vertices[(i + 1) % 3]), Edge2<T>(b.vertices[j], b.vertices[(j + 1) % 3]))) return true;
            }
        }
        return false;
    }

    template <typename T>
    bool intersect(const Box2<T>& a, const Edge2<T>& b)
    {
        // Find min and max X for the segment

        T minX = b.vertices[0].x;
        T maxX = b.vertices[1].x;

        if (b.vertices[0].x > b.vertices[1].x)
        {
            minX = b.vertices[1].x;
            maxX = b.vertices[0].x;
        }

        // Find the intersect of the segment's and rectangle's x-projections

        if (maxX > a.max.x)
        {
            maxX = a.max.x;
        }

        if (minX < a.min.x)
        {
            minX = a.min.x;
        }

        if (minX > maxX) // If their projections do not intersect return false
        {
            return false;
        }

        // Find corresponding min and max Y for min and max X we found before

        T minY = b.vertices[0].y;
        T maxY = b.vertices[1].y;

        const T dx = b.vertices[1].x - b.vertices[0].x;

        if (std::abs(dx) > 0.0000001)
        {
            const T aa = (b.vertices[1].y - b.vertices[0].y) / dx;
            const T bb = b.vertices[0].y - aa * b.vertices[0].x;
            minY = aa * minX + bb;
            maxY = aa * maxX + bb;
        }

        if (minY > maxY)
        {
            const T tmp = maxY;
            maxY = minY;
            minY = tmp;
        }

        // Find the intersect of the segment's and rectangle's y-projections

        if (maxY > a.max.y)
        {
            maxY = a.max.y;
        }

        if (minY < a.min.y)
        {
            minY = a.min.y;
        }

        if (minY > maxY) // If Y-projections do not intersect return false
        {
            return false;
        }

        return true;
    }
    template <typename T>
    bool intersect(const Edge2<T>& a, const Box2<T>& b)
    {
        return intersect(b, a);
    }

    template <typename T>
    bool intersect(const Box2<T>& a, const Vec2<T>& b)
    {
        return (b.x > a.min.x && b.x < a.max.x && b.y > a.min.y && b.y < a.max.y);
    }
    template <typename T>
    bool intersect(const Vec2<T>& a, const Box2<T>& b)
    {
        return intersect(b, a);
    }
    template <typename T>
    bool intersect(const ConvexPolygon2<T>& a, const Vec2<T>& b)
    {
        auto determineQuadrant = [](const Vec2<T>& p, const Vec2<T>& c) -> int {
            if (p.x < c.x)
                return p.y < c.y ? 1 : 4;
            else
                return p.y < c.y ? 2 : 3;
        };

        const int vertexCount = a.size();
        const Vec2<T>* currentVertex = &(a.vertices[vertexCount - 1]);
        int totalQuadrantCrossDelta = 0;
        for (int i = 0; i < vertexCount; ++i)
        {
            const Vec2<T>* nextVertex = &(a.vertices[i]);
            //quadrants
            //1 2
            //4 3
            int currentVertexQuadrant = determineQuadrant(*currentVertex, b);
            int nextVertexQuadrant = determineQuadrant(*nextVertex, b);

            int quadrantCrossDelta = nextVertexQuadrant - currentVertexQuadrant;
            switch (quadrantCrossDelta)
            {
            case 2:
            case -2:
                if ((nextVertex->x - (((nextVertex->y - b.y) * (currentVertex->x - nextVertex->x)) / (currentVertex->y - nextVertex->y))) < b.x) quadrantCrossDelta = -quadrantCrossDelta;
                break;
            case 3:
                quadrantCrossDelta = -1;
                break;
            case -3:
                quadrantCrossDelta = 1;
                break;
            }

            totalQuadrantCrossDelta += quadrantCrossDelta;
            currentVertex = nextVertex;
        }
        return totalQuadrantCrossDelta == 4 || totalQuadrantCrossDelta == -4;
    }
    template <typename T>
    bool intersect(const ConvexPolygon2<T>& a, const ConvexPolygon2<T>& b)
    {
        auto projectMinMax = [](const ConvexPolygon2<T>& poly, const Vec2<T>& b) -> std::pair<T, T> {
            T minProjection;
            T maxProjection;
            minProjection = maxProjection = poly.vertices[0].dot(b);
            for (const Vec2<T>& vertex : poly.vertices)
            {
                T dotProduct = vertex.dot(b);
                if (dotProduct < minProjection) minProjection = dotProduct;
                else if (dotProduct > maxProjection) maxProjection = dotProduct;
            }
            return std::pair<T, T>(minProjection, maxProjection);
        };

        Vec2<T> lastVertex = a.vertices.back();
        for (const Vec2<T>& vertex : a.vertices)
        {
            Vec2<T> perp = (vertex - lastVertex).normal();
            std::pair<T, T> aProjection = projectMinMax(a, perp);
            std::pair<T, T> bProjection = projectMinMax(b, perp);
            if (aProjection.first > bProjection.second || bProjection.first > aProjection.second) return false;
            lastVertex = vertex;
        }

        lastVertex = b.vertices.back();
        for (const Vec2<T>& vertex : b.vertices)
        {
            Vec2<T> perp = (vertex - lastVertex).normal();
            std::pair<T, T> aProjection = projectMinMax(a, perp);
            std::pair<T, T> bProjection = projectMinMax(b, perp);
            if (aProjection.first > bProjection.second || bProjection.first > aProjection.second) return false;
            lastVertex = vertex;
        }
        return true;
    }
    template <typename T>
    bool intersect(const Vec2<T>& a, const ConvexPolygon2<T>& b)
    {
        return intersect(b, a);
    }
    template <typename T>
    bool intersect(const Edge2<T>& a, const ConvexPolygon2<T>& b)
    {
        if (intersect(a.vertices[0], b)) return true;
        if (intersect(a.vertices[1], b)) return true;
        int polySize = b.size();
        for (int i = 0; i < polySize; ++i)
        {
            const Vec2<T>& thisVertex = b.vertices[i];
            const Vec2<T>& nextVertex = b.vertices[(i + 1) % polySize];

            if (intersect(Edge2<T>(thisVertex, nextVertex), a)) return true;
        }
        return false;
    }
    template <typename T>
    bool intersect(const ConvexPolygon2<T>& a, const Edge2<T>& b)
    {
        return intersect(b, a);
    }
    template <typename T>
    bool intersect(const Edge2<T>& a, const Polyline2<T>& b)
    {
        int polySize = b.size();
        for (int i = 0; i < polySize - 1; ++i)
        {
            const Vec2<T>& thisVertex = b.vertices[i];
            const Vec2<T>& nextVertex = b.vertices[i + 1];

            if (intersect(Edge2<T>(thisVertex, nextVertex), a)) return true;
        }
        return false;
    }
    template <typename T>
    bool intersect(const Box2<T>& a, const Triangle2<T>& b)
    {
        return intersect(Polygon2<T>::fromBox(a), ConvexPolygon2<T>::fromTriangle(b));
    }
    template <typename T>
    bool intersect(const Triangle2<T>& a, const Box2<T>& b)
    {
        return intersect(b, a);
    }
    template <typename T>
    bool intersect(const Box2<T>& a, const Polyline2<T>& b)
    {
        if (intersect(a.toPolyline(), b)) return true;

        if (intersect(a, b.vertices.back())) return true; //only one check is needed because the polyline is either intersecting edge of the rectangle or is enclosed by it

        return false;
    }
    template <typename T>
    bool intersect(const Polyline2<T>& a, const Box2<T>& b)
    {
        return intersect(b, a);
    }
    template <typename T>
    bool intersect(const Polyline2<T>& a, const Triangle2<T>& b)
    {
        if (intersect(a, b.toPolyline())) return true;

        if (intersect(a.vertices.back(), b)) return true;

        return false;
    }
    template <typename T>
    bool intersect(const Triangle2<T>& a, const Polyline2<T>& b)
    {
        return intersect(b, a);
    }
    template <typename T>
    bool intersect(const Polyline2<T>& a, const Edge2<T>& b)
    {
        return intersect(b, a);
    }
    template <typename T>
    bool intersect(const Box2<T>& a, const ConvexPolygon2<T>& b)
    {
        return intersect(Polygon2<T>::fromBox(a), b);
    }
    template <typename T>
    bool intersect(const ConvexPolygon2<T>& a, const Box2<T>& b)
    {
        return intersect(b, a);
    }
    template <typename T>
    bool intersect(const Triangle2<T>& a, const ConvexPolygon2<T>& b)
    {
        return intersect(Polygon2<T>::fromTriangle(a), b);
    }
    template <typename T>
    bool intersect(const ConvexPolygon2<T>& a, const Triangle2<T>& b)
    {
        return intersect(b, a);
    }
    template <typename T>
    bool intersect(const Polyline2<T>& a, const Polyline2<T>& b)
    {
        int polySizeA = a.size();
        for (int i = 0; i < polySizeA - 1; ++i)
        {
            const Vec2<T>& thisVertex = a.vertices[i];
            const Vec2<T>& nextVertex = a.vertices[i + 1];

            if (intersect(Edge2<T>(thisVertex, nextVertex), b)) return true;
        }
        return false;
    }

    template <typename T>
    bool intersect(const Edge2<T>& a, const Triangle2<T>& b)
    {
        return intersect(a.toPolyline(), b);
    }
    template <typename T>
    bool intersect(const Triangle2<T>& a, const Edge2<T>& b)
    {
        return intersect(b, a);
    }
    template <typename T>
    bool intersect(const Polyline2<T>& a, const ConvexPolygon2<T>& b)
    {
        int polySizeA = a.size();
        for (int i = 0; i < polySizeA - 1; ++i)
        {
            const Vec2<T>& thisVertex = a.vertices[i];
            const Vec2<T>& nextVertex = a.vertices[i + 1];

            if (intersect(Edge2<T>(thisVertex, nextVertex), b)) return true;
        }

        return false;
    }
    template <typename T>
    bool intersect(const ConvexPolygon2<T>& a, const Polyline2<T>& b)
    {
        return intersect(b, a);
    }
    template <typename T>
    bool intersect(const Circle2<T>& a, const Edge2<T>& b)
    {
        return intersect(a, b.nearestPointTo(a.origin));
    }
    template <typename T>
    bool intersect(const Edge2<T>& a, const Circle2<T>& b)
    {
        return intersect(b, a);
    }
    template <typename T>
    bool intersect(const Circle2<T>& a, const Ray2<T>& b)
    {
        return intersect(a, b.nearestPointTo(a.origin));
    }
    template <typename T>
    bool intersect(const Ray2<T>& a, const Circle2<T>& b)
    {
        return intersect(b, a);
    }
    template <typename T>
    bool intersect(const Edge2<T>& a, const Ray2<T>& b)
    {
        const T& rdx = b.direction.x;
        const T& rdy = b.direction.y;
        const T& rox = b.origin.x;
        const T& roy = b.origin.y;

        const T& sbx = a.vertices[0].x;
        const T& sby = a.vertices[0].y;

        Vec2<T> sd = (a.vertices[1] - a.vertices[0]);
        const T& sdx = sd.x;
        const T& sdy = sd.y;

        /*
        v1 = ray.origin - lineSegment.vertices[0]
        v2 = lineSegment.vertices[1] - lineSegment.vertices[0]
        v3 = ray.direction.normal() //perpvertices[1]icular
        t1 = (v1 . v3) / (v2 . v3)
        */
        T t1 = (rdx * (sby - roy) + rdy * (rox - sbx)) / (sdx * rdy - sdy * rdx);
        T t0 = (sbx + sdx * t1 - rox) / rdx;
        return (t0 >= T(0) && t1 >= T(0) && t1 <= T(1));
    }
    template <typename T>
    bool intersect(const Ray2<T>& a, const Edge2<T>& b)
    {
        return intersect(b, a);
    }
    template <typename T>
    bool intersect(const Box2<T>& a, const Ray2<T>& b)
    {
        if (intersect(b.origin, a)) return true;

        if (intersect(Edge2<T>(a.min, Vec2<T>(a.min.x, a.max.y)), b)) return true;
        if (intersect(Edge2<T>(a.min, Vec2<T>(a.max.x, a.min.y)), b)) return true;
        if (intersect(Edge2<T>(a.max, Vec2<T>(a.min.x, a.max.y)), b)) return true;
        if (intersect(Edge2<T>(a.max, Vec2<T>(a.max.x, a.min.y)), b)) return true;

        return false;
    }
    template <typename T>
    bool intersect(const Ray2<T>& a, const Box2<T>& b)
    {
        return intersect(b, a);
    }
    template <typename T>
    bool intersect(const Circle2<T>& a, const ConvexPolygon2<T>& b)
    {
        if (intersect(a.origin, b)) return true;

        int polySize = b.size();
        for (int i = 0; i < polySize; ++i)
        {
            const Vec2<T>& thisVertex = b.vertices[i];
            const Vec2<T>& nextVertex = b.vertices[(i + 1) % polySize];

            if (intersect(Edge2<T>(thisVertex, nextVertex), a)) return true;
        }

        return false;
    }
    template <typename T>
    bool intersect(const ConvexPolygon2<T>& a, const Circle2<T>& b)
    {
        return intersect(b, a);
    }

    template <typename T>
    bool intersect(const Ray2<T>& a, const Ray2<T>& b)
    {
        T d1x = a.direction.x;
        T d1y = a.direction.y;
        T o1x = a.origin.x;
        T o1y = a.origin.y;

        T d2x = b.direction.x;
        T d2y = b.direction.y;
        T o2x = b.origin.x;
        T o2y = b.origin.y;

        T t1 = (d1x * o2y - d1y * o2x + o1x * d1y - o1y * d1y) / (d1y * d2x - d1x * d2y);
        T t2 = (o2x + d2x * t1 - o1x) / d1x;

        return (t1 >= T(0) && t2 >= T(0));
    }

    template <typename T>
    bool intersect(const Circle2<T>& a, const Polyline2<T>& b)
    {
        int polySize = b.size();
        for (int i = 0; i < polySize - 1; ++i)
        {
            const Vec2<T>& thisVertex = b.vertices[i];
            const Vec2<T>& nextVertex = b.vertices[i + 1];

            if (intersect(Edge2<T>(thisVertex, nextVertex), a)) return true;
        }

        return false;
    }
    template <typename T>
    bool intersect(const Polyline2<T>& a, const Circle2<T>& b)
    {
        return intersect(b, a);
    }
    template <typename T>
    bool intersect(const Circle2<T>& a, const Triangle2<T>& b)
    {
        if (intersect(a.origin, b)) return true;

        if (intersect(Edge2<T>(b.vertices[0], b.vertices[1]), b)) return true;
        if (intersect(Edge2<T>(b.vertices[1], b.vertices[2]), b)) return true;
        if (intersect(Edge2<T>(b.vertices[2], b.vertices[0]), b)) return true;

        return false;
    }
    template <typename T>
    bool intersect(const Triangle2<T>& a, const Circle2<T>& b)
    {
        return intersect(b, a);
    }
    template <typename T>
    bool intersect(const Ray2<T>& a, const Triangle2<T>& b)
    {
        if (intersect(a.origin, b)) return true;

        if (intersect(Edge2<T>(b.vertices[0], b.vertices[1]), b)) return true;
        if (intersect(Edge2<T>(b.vertices[1], b.vertices[2]), b)) return true;
        if (intersect(Edge2<T>(b.vertices[2], b.vertices[0]), b)) return true;

        return false;
    }
    template <typename T>
    bool intersect(const Triangle2<T>& a, const Ray2<T>& b)
    {
        return intersect(b, a);
    }
    template <typename T>
    bool intersect(const Ray2<T>& a, const Polyline2<T>& b)
    {
        int polySize = b.size();
        for (int i = 0; i < polySize - 1; ++i)
        {
            const Vec2<T>& thisVertex = b.vertices[i];
            const Vec2<T>& nextVertex = b.vertices[i + 1];

            if (intersect(Edge2<T>(thisVertex, nextVertex), a)) return true;
        }

        return false;
    }
    template <typename T>
    bool intersect(const Polyline2<T>& a, const Ray2<T>& b)
    {
        return intersect(b, a);
    }
    template <typename T>
    bool intersect(const Ray2<T>& a, const ConvexPolygon2<T>& b)
    {
        if (intersect(a.origin, b)) return true;

        int polySize = b.size();
        for (int i = 0; i < polySize; ++i)
        {
            const Vec2<T>& thisVertex = b.vertices[i];
            const Vec2<T>& nextVertex = b.vertices[(i + 1) % polySize];

            if (intersect(Edge2<T>(thisVertex, nextVertex), a)) return true;
        }

        return false;
    }
    template <typename T>
    bool intersect(const ConvexPolygon2<T>& a, const Ray2<T>& b)
    {
        return intersect(b, a);
    }

    //temporary
    template <typename T>
    bool intersect(const Vec2<T>& a, const Vec2<T>& b)
    {
        return Circle2<T>(a, T(5)).intersects(Circle2<T>(b, T(10)));
    }

    /*
    containing operations
    */


    //Box2
    template <typename T>
    bool contains(const Box2<T>& a, const Circle2<T>& b)
    {
        return ((a.min.x < b.origin.x - b.radius) && (a.max.x > b.origin.x + b.radius) && (a.min.y < b.origin.y - b.radius) && (a.max.y > b.origin.y + b.radius));
    }
    template <typename T>
    bool contains(const Box2<T>& a, const Edge2<T>& b)
    {
        return (intersect(a, b.vertices[0]) && intersect(a, b.vertices[1]));
    }
    template <typename T>
    bool contains(const Box2<T>& a, const ConvexPolygon2<T>& b)
    {
        for (const auto& v : b.vertices)
        {
            if (!intersect(a, v)) return false;
        }
        return true;
    }
    template <typename T>
    bool contains(const Box2<T>& a, const Polyline2<T>& b)
    {
        for (const auto& v : b.vertices)
        {
            if (!intersect(a, v)) return false;
        }
        return true;
    }
    template <typename T>
    bool contains(const Box2<T>& a, const Box2<T>& b)
    {
        return ((a.min.x < b.min.x) && (a.max.x > b.max.x) && (a.min.y < b.min.y) && (a.max.y > b.max.y));
    }
    template <typename T>
    bool contains(const Box2<T>& a, const Triangle2<T>& b)
    {
        return (intersect(a, b.vertices[0]) && intersect(a, b.vertices[1]) && intersect(a, b.vertices[2]));
    }
    template <typename T>
    bool contains(const Box2<T>& a, const Vec2<T>& b)
    {
        return intersect(a, b);
    }



    //Triangle2
    template <typename T>
    bool contains(const Triangle2<T>& a, const Circle2<T>& b)
    {
        if (!contains(a, b.origin)) return false;
        if (intersect(Edge2<T>(a.vertices[0], a.vertices[1]), b)) return false;
        if (intersect(Edge2<T>(a.vertices[1], a.vertices[2]), b)) return false;
        if (intersect(Edge2<T>(a.vertices[2], a.vertices[0]), b)) return false;
        return true;
    }
    template <typename T>
    bool contains(const Triangle2<T>& a, const Edge2<T>& b)
    {
        return (intersect(a, b.vertices[0]) && intersect(a, b.vertices[1]));
    }
    template <typename T>
    bool contains(const Triangle2<T>& a, const ConvexPolygon2<T>& b)
    {
        for (const auto& v : b.vertices)
        {
            if (!intersect(a, v)) return false;
        }
        return true;
    }
    template <typename T>
    bool contains(const Triangle2<T>& a, const Polyline2<T>& b)
    {
        for (const auto& v : b.vertices)
        {
            if (!intersect(a, v)) return false;
        }
        return true;
    }
    template <typename T>
    bool contains(const Triangle2<T>& a, const Box2<T>& b)
    {
        if (!intersect(a, b.min)) return false;
        if (!intersect(a, b.max)) return false;
        if (!intersect(a, Vec2<T>(b.min.x + b.max.y))) return false;
        if (!intersect(a, Vec2<T>(b.min.y + b.max.x))) return false;
        return true;
    }
    template <typename T>
    bool contains(const Triangle2<T>& a, const Triangle2<T>& b)
    {
        return (intersect(a, b.vertices[0]) && intersect(a, b.vertices[1]) && intersect(a, b.vertices[2]));
    }
    template <typename T>
    bool contains(const Triangle2<T>& a, const Vec2<T>& b)
    {
        return intersect(a, b);
    }

    template <typename T>
    bool contains(const Circle2<T>& a, const Circle2<T>& b)
    {
        return a.origin.distanceTo(b.origin) < a.radius - b.radius;
    }
    template <typename T>
    bool contains(const Circle2<T>& a, const Edge2<T>& b)
    {
        return contains(a, b.vertices[0]) && contains(a, b.vertices[1]);
    }
    template <typename T>
    bool contains(const Circle2<T>& a, const ConvexPolygon2<T>& b)
    {
        int polySize = b.size();
        for (int i = 0; i < polySize; ++i)
        {
            const Vec2<T>& thisVertex = b.vertices[i];
            const Vec2<T>& nextVertex = b.vertices[(i + 1) % polySize];

            if (!contains(a, Edge2<T>(thisVertex, nextVertex))) return false;
        }
        return true;
    }
    template <typename T>
    bool contains(const Circle2<T>& a, const Polyline2<T>& b)
    {
        int polySize = b.size();
        for (int i = 0; i < polySize - 1; ++i)
        {
            const Vec2<T>& thisVertex = b.vertices[i];
            const Vec2<T>& nextVertex = b.vertices[i + 1];

            if (!contains(a, Edge2<T>(thisVertex, nextVertex))) return false;
        }
        return true;
    }
    template <typename T>
    bool contains(const Circle2<T>& a, const Box2<T>& b)
    {
        return contains(a, b.min) && contains(a, b.max);
    }
    template <typename T>
    bool contains(const Circle2<T>& a, const Triangle2<T>& b)
    {
        return contains(a, b.vertices[0]) && contains(a, b.vertices[1]) && contains(a, b.vertices[2]);
    }
    template <typename T>
    bool contains(const Circle2<T>& a, const Vec2<T>& b)
    {
        return intersect(a, b);
    }


    template <typename T>
    bool contains(const ConvexPolygon2<T>& a, const Circle2<T>& b)
    {
        if (!contains(a, b.origin)) return false;

        int polySize = a.size();
        for (int i = 0; i < polySize; ++i)
        {
            const Vec2<T>& thisVertex = a.vertices[i];
            const Vec2<T>& nextVertex = a.vertices[(i + 1) % polySize];

            if (intersect(Edge2<T>(thisVertex, nextVertex), b)) return false;
        }
        return true;
    }
    template <typename T>
    bool contains(const ConvexPolygon2<T>& a, const Edge2<T>& b)
    {
        if (!contains(a, b.vertices[0]) && !contains(a, b.vertices[1])) return false;

        int polySize = a.size();
        for (int i = 0; i < polySize; ++i)
        {
            const Vec2<T>& thisVertex = a.vertices[i];
            const Vec2<T>& nextVertex = a.vertices[(i + 1) % polySize];

            if (intersect(Edge2<T>(thisVertex, nextVertex), b)) return false;
        }
        return true;
    }
    template <typename T>
    bool contains(const ConvexPolygon2<T>& a, const ConvexPolygon2<T>& b)
    {
        int polySize = b.size();
        for (int i = 0; i < polySize; ++i)
        {
            const Vec2<T>& thisVertex = b.vertices[i];
            const Vec2<T>& nextVertex = b.vertices[(i + 1) % polySize];

            if (!contains(a, Edge2<T>(thisVertex, nextVertex))) return false;
        }
        return true;
    }
    template <typename T>
    bool contains(const ConvexPolygon2<T>& a, const Polyline2<T>& b)
    {
        int polySize = b.size();
        for (int i = 0; i < polySize - 1; ++i)
        {
            const Vec2<T>& thisVertex = b.vertices[i];
            const Vec2<T>& nextVertex = b.vertices[i + 1];

            if (!contains(a, Edge2<T>(thisVertex, nextVertex))) return false;
        }
        return true;
    }
    template <typename T>
    bool contains(const ConvexPolygon2<T>& a, const Box2<T>& b)
    {
        if (!contains(a, Edge2<T>(b.min, Vec2<T>(b.min.x, b.max.y)))) return false;
        if (!contains(a, Edge2<T>(b.max, Vec2<T>(b.min.x, b.max.y)))) return false;
        if (!contains(a, Edge2<T>(b.min, Vec2<T>(b.max.x, b.min.y)))) return false;
        if (!contains(a, Edge2<T>(b.max, Vec2<T>(b.max.x, b.min.y)))) return false;
        return true;
    }
    template <typename T>
    bool contains(const ConvexPolygon2<T>& a, const Triangle2<T>& b)
    {
        if (!contains(a, Edge2<T>(b.vertices[0], b.vertices[1]))) return false;
        if (!contains(a, Edge2<T>(b.vertices[1], b.vertices[2]))) return false;
        if (!contains(a, Edge2<T>(b.vertices[2], b.vertices[0]))) return false;
        return true;
    }
    template <typename T>
    bool contains(const ConvexPolygon2<T>& a, const Vec2<T>& b)
    {
        return intersect(a, b);
    }

    template <typename A, typename B>
    bool isContained(const A& a, const B& b) { return contains(b, a); }


    /*

    continuous detection

    */

    template <typename T>
    PointNormalPair2<T> intersectionPointAndNormal(const Circle2<T>& circle, const Vec2<T>& point)
    {
        return{ point, (point - circle.origin).normalized() };
    }
    template <typename T>
    PointNormalPair2<T> intersectionPointAndNormal(const Circle2<T>& c1, const Circle2<T>& c2)
    {
        const Vec2<T> normal = (c2.origin - c1.origin).normalized();
        const Vec2<T> point = c1.origin + normal * c1.radius;
        return{ point, normal };
    }
    template <typename T>
    PointNormalPair2<T> intersectionPointAndNormal(const Circle2<T>& circle, const Box2<T>& rectangle)
    {
        using std::clamp;

        const Vec2<T> closestPoint(
            clamp(circle.origin.x, rectangle.min.x, rectangle.max.x),
            clamp(circle.origin.y, rectangle.min.y, rectangle.max.y)
        );

        return collisionPointAndNormal(circle, closestPoint);
    }


    template <typename T>
    std::optional<T> timeOfImpact(const WithVelocity<Circle2<T>>& movingCircle1, const WithVelocity<Circle2<T>>& movingCircle2)
    {
        using std::sqrt;
        using std::min;
        using std::numeric_limits;

        const Circle2<T>& c1 = movingCircle1.shape();
        const Circle2<T>& c2 = movingCircle2.shape();
        const Vec2<T>& v1 = movingCircle1.velocity();
        const Vec2<T>& v2 = movingCircle2.velocity();

        if (intersect(c1, c2)) return T(0);

        const Vec2<T> v = v1 - v2;
        const T dx = c1.origin.x - c2.origin.x;
        const T dy = c1.origin.y - c2.origin.y;
        const T r = c1.radius + c2.radius;

        const T a = (v.x*v.x) + (v.y*v.y);
        const T b = T(2)*((dx*v.x) + (dy*v.y));
        const T c = (dx*dx) + (dy*dy) - (r*r);

        const T d = (b*b) - T(4)*a*c;

        if (d < T(0)) return std::nullopt;

        const T sqrtd = sqrt(d);
        T t1 = (-b - sqrtd) / (T(2)*a);
        T t2 = (-b + sqrtd) / (T(2)*a);
        if (t1 < 0) t1 = numeric_limits<T>::max();
        if (t2 < 0) t2 = numeric_limits<T>::max();

        const T minT = min(t1, t2);

        if (minT != numeric_limits<T>::max()) return minT;
        return std::nullopt;
    }

    template <typename T>
    std::optional<T> timeOfImpact(const WithVelocity<Circle2<T>>& movingCircle, const WithVelocity<Vec2<T>>& movingPoint)
    {
        return timeOfImpact(movingCircle, moving(Circle2<T>(movingPoint.shape(), T(0)), movingPoint.velocity()));
    }

    template <typename T>
    std::optional<T> timeOfImpact(const WithVelocity<Circle2<T>>& movingCircle, const WithVelocity<Box2<T>>& movingBox)
    {
        using std::numeric_limits;
        using std::min;

        const Circle2<T>& circle = movingCircle.shape();
        const Box2<T>& rectangle = movingBox.shape();
        const Vec2<T>& v1 = movingCircle.velocity();
        const Vec2<T>& v2 = movingBox.velocity();

        if (intersect(circle, rectangle)) return T(0);

        const Vec2<T> v = v1 - v2;

        //following are >0 if the circle is directly in front of an edge
        const T signedDistToMinX = rectangle.min.x - (circle.origin.x + circle.radius);
        const T signedDistToMinY = rectangle.min.y - (circle.origin.y + circle.radius);
        const T signedDistToMaxX = (circle.origin.x - circle.radius) - rectangle.max.x;
        const T signedDistToMaxY = (circle.origin.y - circle.radius) - rectangle.max.y;

        bool canHitCornerMinMin = true;
        bool canHitCornerMinMax = true;
        bool canHitCornerMaxMin = true;
        bool canHitCornerMaxMax = true;

        T minT = numeric_limits<T>::max();

        if (signedDistToMinX > T(0) && v.x > T(0)) //crosses min.x
        {
            const T crossTime = signedDistToMinX / v.x;
            const T contactY = circle.origin.y + crossTime * v.y;
            const bool isOnEdge = contactY >= rectangle.min.y && contactY <= rectangle.max.y;

            if (isOnEdge) minT = min(minT, crossTime);

            if (contactY >= rectangle.min.y) canHitCornerMinMin = false;
            if (contactY <= rectangle.max.y) canHitCornerMinMax = false;
        }
        if (signedDistToMaxX > T(0) && v.x < T(0)) //crosses max.x
        {
            const T crossTime = signedDistToMaxX / -v.x;
            const T contactY = circle.origin.y + crossTime * v.y;
            const bool isOnEdge = contactY >= rectangle.min.y && contactY <= rectangle.max.y;

            if (isOnEdge) minT = min(minT, crossTime);

            if (contactY >= rectangle.min.y) canHitCornerMaxMin = false;
            if (contactY <= rectangle.max.y) canHitCornerMaxMax = false;
        }
        if (signedDistToMinY > T(0) && v.y > T(0)) //crosses min.y
        {
            const T crossTime = signedDistToMinY / v.y;
            const T contactX = circle.origin.x + crossTime * v.x;
            const bool isOnEdge = contactX >= rectangle.min.x && contactX <= rectangle.max.x;

            if (isOnEdge) minT = min(minT, crossTime);

            if (contactX >= rectangle.min.x) canHitCornerMinMin = false;
            if (contactX <= rectangle.max.x) canHitCornerMaxMin = false;
        }
        if (signedDistToMaxY > T(0) && v.y < T(0)) //crosses max.y
        {
            const T crossTime = signedDistToMaxY / -v.y;
            const T contactX = circle.origin.x + crossTime * v.x;
            const bool isOnEdge = contactX >= rectangle.min.x && contactX <= rectangle.max.x;

            if (isOnEdge) minT = min(minT, crossTime);

            if (contactX >= rectangle.min.x) canHitCornerMinMax = false;
            if (contactX <= rectangle.max.x) canHitCornerMaxMax = false;
        }

        if (canHitCornerMinMin)
        {
            const T t = timeOfImpact(circle, v1, rectangle.min, v2).value_or(numeric_limits<T>::max());
            minT = min(minT, t);
        }
        if (canHitCornerMinMax)
        {
            const T t = timeOfImpact(circle, v1, Vec2<T>(rectangle.min.x, rectangle.max.y), v2).value_or(numeric_limits<T>::max());
            minT = min(minT, t);
        }
        if (canHitCornerMaxMin)
        {
            const T t = timeOfImpact(circle, v1, Vec2<T>(rectangle.max.x, rectangle.min.y), v2).value_or(numeric_limits<T>::max());
            minT = min(minT, t);
        }
        if (canHitCornerMaxMax)
        {
            const T t = timeOfImpact(circle, v1, rectangle.max, v2).value_or(numeric_limits<T>::max());
            minT = min(minT, t);
        }

        if (minT != numeric_limits<T>::max()) return minT;
        return std::experimental::nullopt;
    }

    template <typename T>
    T timeOfImpactLowerBound(const WithVelocity<Circle2<T>>& movingCircle1, const WithVelocity<Circle2<T>>& movingCircle2) //timeOfImpact is always none or greater than estimated
    {
        const Circle2<T>& c1 = movingCircle1.shape();
        const Circle2<T>& c2 = movingCircle2.shape();
        const Vec2<T>& v1 = movingCircle1.velocity();
        const Vec2<T>& v2 = movingCircle2.velocity();

        const T totalSpeed = v1.length() + v2.length();
        const T dist = (c1.origin - c2.origin).length() - (c1.radius + c2.radius);

        return dist / totalSpeed;
    }

    template <typename T>
    Vec2<T> penetrationVector(const Circle2<T>& c1, const Circle2<T>& c2)
    {
        const T dist = distance(c1, c2);
        if (dist > c1.radius + c2.radius) return Vec2<T>(0, 0);
        return (c2.origin - c1.origin).normalized() * (dist - c1.radius - c2.radius);
    }

    template <typename S1, typename S2>
    void resolvePenetration(S1& s1, S2& s2, const typename S1::VectorType& pen, const typename S1::VectorType::ValueType& balance)
    {
        s1.translate(pen * -balance);
        s2.translate(pen * (1.0 - balance));
    }
}
