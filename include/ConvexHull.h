#ifndef CONVEXHULL_H_INCLUDED
#define CONVEXHULL_H_INCLUDED

#include "Vec2.h"
#include "Polygon.h"

template <class T>
class ConvexHull
{
public:
    std::vector<Vec2<T>> points;
    Polygon<T> convexHull;

    ConvexHull(const std::vector<Vec2<T>>& p);
    ConvexHull(std::vector<Vec2<T>>&& p);

    void calculate();
    void sortPoints();
private:
    bool sorted;
    T cross(const Vec2<T>& o, const Vec2<T>& a, const Vec2<T>& b);
};

#include "../src/ConvexHull.cpp"

#endif // CONVEXHULL_H_INCLUDED
