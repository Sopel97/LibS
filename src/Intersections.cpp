template <class T>
bool Intersections::intersection(const Rectangle<T>& a, const Rectangle<T>& b)
{
    if(a.min.x >= b.max.x) return false;
    if(a.min.y >= b.max.y) return false;
    if(a.max.x <= b.min.x) return false;
    if(a.max.y <= b.min.y) return false;
    return true;
}
template <class T>
bool Intersections::intersection(const Rectangle<T>& a, const Circle<T>& b)
{
    double closestX = clamp(b.origin.x, a.min.x, a.max.x);
    double closestY = clamp(b.origin.y, a.min.y, a.max.y);
    double distanceX = b.origin.x - closestX;
    double distanceY = b.origin.y - closestY;
    double distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);
    return distanceSquared <= (b.radius * b.radius);
}
template <class T>
bool Intersections::intersection(const Circle<T>& a, const Rectangle<T>& b)
{
    return intersection(b, a);
}
template <class T>
bool Intersections::intersection(const Vec2<T>& a, const Circle<T>& b)
{
    double dx = b.origin.x - a.x;
    dx *= dx;
    double dy = b.origin.y - a.y;
    dy *= dy;
    return dx + dy <= b.radius * b.radius;
}
template <class T>
bool Intersections::intersection(const Circle<T>& a, const Vec2<T>& b)
{
    return intersection(b, a);
}

template <class T>
bool Intersections::intersection(const Circle<T>& a, const Circle<T>& b)
{
    double x = fabs(a.origin.x - b.origin.x);
    double y = fabs(a.origin.y - b.origin.y);
    double radiusSum = a.radius + b.radius;
    return x * x + y * y <= radiusSum * radiusSum;
}


template <class T>
bool Intersections::intersection(const Cylinder<T>& a, const Cylinder<T>& b)
{
    if(a.origin.z > b.origin.z + b.height) return false;
    if(a.origin.z + a.height < b.origin.z) return false;
    double x = fabs(a.origin.x - b.origin.x);
    double y = fabs(a.origin.y - b.origin.y);
    double radiusSum = a.radius + b.radius;
    return x * x + y * y <= radiusSum * radiusSum;
}

template <class T>
bool Intersections::intersection(const Cuboid<T>& a, const Cuboid<T>& b)
{
    if(a.min.x >= b.max.x) return false;
    if(a.min.y >= b.max.y) return false;
    if(a.min.z >= b.max.z) return false;
    if(a.max.x <= b.min.x) return false;
    if(a.max.y <= b.min.y) return false;
    if(a.max.z <= b.min.z) return false;
    return true;
}


template <class T>
bool Intersections::intersection(const LineSegment<T>& a, const LineSegment<T>& b, Vec2<T>& intersectionPoint)
{
    double s1_x, s1_y, s2_x, s2_y;
    s1_x = a.end.x - a.begin.x;     s1_y = a.end.y - a.begin.y;
    s2_x = b.end.x - b.begin.x;     s2_y = b.end.y - b.begin.y;

    float s, t;
    s = (-s1_y * (a.begin.x - b.begin.x) + s1_x * (a.begin.y - b.begin.y)) / (-s2_x * s1_y + s1_x * s2_y);
    t = (s2_x * (a.begin.y - b.begin.y) - s2_y * (a.begin.x - b.begin.x)) / (-s2_x * s1_y + s1_x * s2_y);

    if(s >= 0 && s <= 1 && t >= 0 && t <= 1)
    {
        // Collision detected
        intersectionPoint.x = a.begin.x + (t * s1_x);
        intersectionPoint.y = a.begin.y + (t * s1_y);
        return true;
    }

    return false; // No collision
    /*
    double denominator1 = a.end.x - a.begin.x;
    double denominator2 = b.end.x - a.begin.x;
    double aa = (a.end.y - a.begin.y) / denominator1;
    double bb = (b.end.y - b.begin.y) / denominator2;
    double cc = (a.end.x * a.begin.y - a.begin.x * a.end.y) / denominator1;
    double dd = (b.end.x * b.begin.y - b.begin.x * b.end.y) / denominator2;
    intersectionPoint.x = (dd - cc) / (aa - bb);
    intersectionPoint.y = aa * intersectionPoint.x + cc;
    return (intersectionPoint.x < max(a.begin.x, a.end.x) && intersectionPoint.x > min(a.begin.x, a.end.x));
    */
}

template <class T>
bool Intersections::intersection(const LineSegment<T>& a, const LineSegment<T>& b)
{
    Vec2<T> intersectionPoint;
    return intersection(a, b, intersectionPoint);
}

template <class T>
bool Intersections::intersection(const Triangle<T>& a, const Vec2<T>& b)
{
    auto sign = [](const Vec2<T>& v1, const Vec2<T>& v2, const Vec2<T>& v3) -> double
    {
        return (v1.x - v3.x) * (v2.y - v3.y) - (v2.x - v3.x) * (v1.y - v3.y);
    };

    if(sign(b, a.vertices[0], a.vertices[1]) < 0.0) return false;
    if(sign(b, a.vertices[1], a.vertices[2]) < 0.0) return false;
    if(sign(b, a.vertices[2], a.vertices[0]) < 0.0) return false;

    return true;
}
template <class T>
bool Intersections::intersection(const Vec2<T>& a, const Triangle<T>& b)
{
    return intersection(b, a);
}
template <class T>
bool Intersections::intersection(const Triangle<T>& a, const Triangle<T>& b) //do poprawy
{
    int i = 0;
    while(i < 3)
    {
        if(intersection(a.vertices[i], b)) return true;
        ++i;
    }
    while(i < 3)
    {
        if(intersection(b.vertices[i], a)) return true;
        ++i;
    }
    for(i = 0; i < 3; ++i)
    {
        for(int j = 0; j < 3; ++j)
        {
            if(intersection(LineSegment<T>(a.vertices[i], a.vertices[(i + 1) % 3]), LineSegment<T>(b.vertices[j], b.vertices[(j + 1) % 3]))) return true;
        }
    }
    return false;
}

template <class T>
bool Intersections::intersection(const Rectangle<T>& a, const LineSegment<T>& b)
{
    // Find min and max X for the segment

    double minX = b.begin.x;
    double maxX = b.end.x;

    if(b.begin.x > b.end.x)
    {
        minX = b.end.x;
        maxX = b.begin.x;
    }

    // Find the intersection of the segment's and rectangle's x-projections

    if(maxX > a.max.x)
    {
        maxX = a.max.x;
    }

    if(minX < a.min.x)
    {
        minX = a.min.x;
    }

    if(minX > maxX) // If their projections do not intersect return false
    {
        return false;
    }

    // Find corresponding min and max Y for min and max X we found before

    double minY = b.begin.y;
    double maxY = b.end.y;

    double dx = b.end.x - b.begin.x;

    if(std::abs(dx) > 0.0000001)
    {
        double aa = (b.end.y - b.begin.y) / dx;
        double bb = b.begin.y - aa * b.begin.x;
        minY = aa * minX + bb;
        maxY = aa * maxX + bb;
    }

    if(minY > maxY)
    {
        double tmp = maxY;
        maxY = minY;
        minY = tmp;
    }

    // Find the intersection of the segment's and rectangle's y-projections

    if(maxY > a.max.y)
    {
        maxY = a.max.y;
    }

    if(minY < a.min.y)
    {
        minY = a.min.y;
    }

    if(minY > maxY) // If Y-projections do not intersect return false
    {
        return false;
    }

    return true;
}
template <class T>
bool Intersections::intersection(const LineSegment<T>& a, const Rectangle<T>& b)
{
    return intersection(b, a);
}

template <class T>
bool Intersections::intersection(const Rectangle<T>& a, const Vec2<T>& b)
{
    return (b.x > a.min.x && b.x < a.max.x && b.y > a.min.y && b.y < a.max.y);
}
template <class T>
bool Intersections::intersection(const Vec2<T>& a, const Rectangle<T>& b)
{
    return intersection(b, a);
}
template <class T>
bool Intersections::intersection(const Polygon<T>& a, const Vec2<T>& b)
{
    auto determineQuadrant = [](const Vec2<T>& p, const Vec2<T>& c) -> int
    {
        if(p.x < c.x)
            return p.y < c.y ? 1 : 4;
        else
            return p.y < c.y ? 2 : 3;
    };
    size_t vertexCount = a.size();
    const Vec2<T>* currentVertex = &(a.vertices[vertexCount - 1]);
    int totalQuadrantCrossDelta = 0;
    for(size_t i = 0; i < vertexCount; ++i)
    {
        const Vec2<T>* nextVertex = &(a.vertices[i]);
        //quadrants
        //1 2
        //4 3
        int currentVertexQuadrant = determineQuadrant(*currentVertex, b);
        int nextVertexQuadrant = determineQuadrant(*nextVertex, b);

        int quadrantCrossDelta = nextVertexQuadrant - currentVertexQuadrant;
        switch(quadrantCrossDelta)
        {
            case 2:
            case -2:
                if((nextVertex->x - (((nextVertex->y - b.y) * (currentVertex->x - nextVertex->x)) / (currentVertex->y - nextVertex->y))) < b.x) quadrantCrossDelta = -quadrantCrossDelta;
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
template <class T>
bool Intersections::intersection(const Polygon<T>& a, const Polygon<T>& b)
{
    Vec2<T> lastVertex = a.vertices.back();
    for(const Vec2<T>& vertex : a.vertices)
    {
        Vec2<T> perp = (vertex - lastVertex).normal();
        std::pair<T, T> aProjection = a.projectMinMax(perp);
        std::pair<T, T> bProjection = b.projectMinMax(perp);
        if(aProjection.first > bProjection.second || bProjection.first > aProjection.second) return false;
        lastVertex = vertex;
    }

    lastVertex = b.vertices.back();
    for(const Vec2<T>& vertex : b.vertices)
    {
        Vec2<T> perp = (vertex - lastVertex).normal();
        std::pair<T, T> aProjection = a.projectMinMax(perp);
        std::pair<T, T> bProjection = b.projectMinMax(perp);
        if(aProjection.first > bProjection.second || bProjection.first > aProjection.second) return false;
        lastVertex = vertex;
    }
    return true;
}
template <class T>
bool Intersections::intersection(const Vec2<T>& a, const Polygon<T>& b)
{
    return intersection(b, a);
}
template <class T>
bool Intersections::intersection(const LineSegment<T>& a, const Polygon<T>& b)
{
    if(intersection(a.begin, b)) return true;
    if(intersection(a.end, b)) return true;
    size_t polySize = b.size();
    for(size_t i = 0; i < polySize; ++i)
    {
        const Vec2<T>& thisVertex = b.vertices[i];
        const Vec2<T>& nextVertex = b.vertices[(i + 1) % polySize];

        if(intersection(LineSegment<T>(thisVertex, nextVertex), a)) return true;
    }
    return false;
}
template <class T>
bool Intersections::intersection(const Polygon<T>& a, const LineSegment<T>& b)
{
    return intersection(b, a);
}
template <class T>
bool Intersections::intersection(const LineSegment<T>& a, const Polyline<T>& b)
{
    size_t polySize = b.size();
    for(size_t i = 0; i < polySize - 1; ++i)
    {
        const Vec2<T>& thisVertex = b.vertices[i];
        const Vec2<T>& nextVertex = b.vertices[i + 1];

        if(intersection(LineSegment<T>(thisVertex, nextVertex), a)) return true;
    }
    return false;
}
template <class T>
bool Intersections::intersection(const Rectangle<T>& a, const Triangle<T>& b)
{
    return intersection(Polygon<T>::fromRectangle(a), Polygon<T>::fromTriangle(b));
}
template <class T>
bool Intersections::intersection(const Triangle<T>& a, const Rectangle<T>& b)
{
    return intersection(b, a);
}
template <class T>
bool Intersections::intersection(const Rectangle<T>& a, const Polyline<T>& b)
{
    if(intersection(a.asPolyline(), b)) return true;

    if(intersection(a, b.vertices.back())) return true; //only one check is needed because the polyline is either intersecting edge of the rectangle or is enclosed by it

    return false;
}
template <class T>
bool Intersections::intersection(const Polyline<T>& a, const Rectangle<T>& b)
{
    return intersection(b, a);
}
template <class T>
bool Intersections::intersection(const Polyline<T>& a, const Triangle<T>& b)
{
    if(intersection(a, b.asPolyline())) return true;

    if(intersection(a.vertices.back(), b)) return true;

    return false;
}
template <class T>
bool Intersections::intersection(const Triangle<T>& a, const Polyline<T>& b)
{
    return intersection(b, a);
}
template <class T>
bool Intersections::intersection(const Polyline<T>& a, const LineSegment<T>& b)
{
    return intersection(b, a);
}
template <class T>
bool Intersections::intersection(const Rectangle<T>& a, const Polygon<T>& b)
{
    return intersection(Polygon<T>::fromRectangle(a), b);
}
template <class T>
bool Intersections::intersection(const Polygon<T>& a, const Rectangle<T>& b)
{
    return intersection(b, a);
}
template <class T>
bool Intersections::intersection(const Triangle<T>& a, const Polygon<T>& b)
{
    return intersection(Polygon<T>::fromTriangle(a), b);
}
template <class T>
bool Intersections::intersection(const Polygon<T>& a, const Triangle<T>& b)
{
    return intersection(b, a);
}
template <class T>
bool Intersections::intersection(const Polyline<T>& a, const Polyline<T>& b)
{
    size_t polySizeA = a.size();
    for(size_t i = 0; i < polySizeA - 1; ++i)
    {
        const Vec2<T>& thisVertex = a.vertices[i];
        const Vec2<T>& nextVertex = a.vertices[i + 1];

        if(intersection(LineSegment<T>(thisVertex, nextVertex), b)) return true;
    }
    return false;
}

template <class T>
bool Intersections::intersection(const LineSegment<T>& a, const Triangle<T>& b)
{
    return intersection(a.asPolyline(), b);
}
template <class T>
bool Intersections::intersection(const Triangle<T>& a, const LineSegment<T>& b)
{
    return intersection(b, a);
}
template <class T>
bool Intersections::intersection(const Polyline<T>& a, const Polygon<T>& b)
{
    size_t polySizeA = a.size();
    for(size_t i = 0; i < polySizeA - 1; ++i)
    {
        const Vec2<T>& thisVertex = a.vertices[i];
        const Vec2<T>& nextVertex = a.vertices[i + 1];

        if(intersection(LineSegment<T>(thisVertex, nextVertex), b)) return true;
    }

    return false;
}
template <class T>
bool Intersections::intersection(const Polygon<T>& a, const Polyline<T>& b)
{
    return intersection(b, a);
}
//meshes

template <class T>
bool Intersections::intersection(const Mesh2<T>& a, const Mesh2<T>& b)
{
    size_t sizeA = a.size();
    size_t sizeB = b.size();
    for(int i = 0; i < sizeA; ++i)
    {
        for(int j = 0; j < sizeB; ++j)
        {
            if(intersection(a.elements[i], a.elements[j])) return true;
        }
    }
    return false;
}
template <class T, class S>
bool Intersections::intersection(const Mesh2<T>& a, const S& b)
{
    int size = a.size();
    for(int i = 0; i < size; ++i)
    {
        if(intersection(a.elements[i], b)) return true;
    }
    return false;
}
template <class T, class S>
bool Intersections::intersection(const S& a, const Mesh2<T>& b)
{
    return intersection(b, a);
}

//temporary
template <class T>
bool Intersections::intersection(const Vec2<T>& a, const Vec2<T>& b)
{
    return Circle<T>(a, T(5)).intersects(Circle<T>(b, T(10)));
}
