template <class T>
ConvexHull<T>::ConvexHull(const std::vector<Vec2<T>>& p) : points(p), sorted(false)
{

}
template <class T>
ConvexHull<T>::ConvexHull(std::vector<Vec2<T>>&& p) : points(p), sorted(false)
{

}
template <class T>
void ConvexHull<T>::sortPoints() //made lhs.x <= rhs.x to omit warning about floating point comparsion
{
    std::sort(points.begin(), points.end(), [](const Vec2<T>& lhs, const Vec2<T>& rhs) -> bool {return (lhs.x < rhs.x || (lhs.x <= rhs.x && lhs.y < rhs.y));});
}
template <class T>
T ConvexHull<T>::cross(const Vec2<T>& o, const Vec2<T>& a, const Vec2<T>& b)
{
    return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
}
template <class T>
void ConvexHull<T>::calculate()
{
    if(!sorted)
    {
        sortPoints();
        sorted = true;
    }
    int n = points.size(), k = 0;

    convexHull.vertices.resize(n * 2);

    // Build lower hull
    for(int i = 0; i < n; ++i)
    {
        while(k >= 2 && cross(convexHull.vertices[k - 2], convexHull.vertices[k - 1], points[i]) <= 0) --k;
        convexHull.vertices[k++] = points[i];
    }

    // Build upper hull
    for(int i = n - 2, t = k + 1; i >= 0; --i)
    {
        while(k >= t && cross(convexHull.vertices[k - 2], convexHull.vertices[k - 1], points[i]) <= 0) --k;
        convexHull.vertices[k++] = points[i];
    }

    convexHull.vertices.resize(k - 1);
}
