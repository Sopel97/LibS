template <class T>
Polygon<T>::Polygon(const std::initializer_list<Vec2<T>>& list) : vertices(list)
{
}
template <class T>
Polygon<T>::Polygon(const std::vector<Vec2<T>>& v) : vertices(v)
{
}
template <class T>
Polygon<T>::Polygon(std::vector<Vec2<T>>&& v) : vertices(std::move(v))
{
}
template <class T>
Polygon<T>::Polygon(Vec2<T>* v, size_t count)
{
    vertices.assign(v, v + count);
}
template <class T>
template <class X>
Polygon<T>::Polygon(const Polygon<X>& p) : vertices(p.vertives)
{
}
template <class T>
template <class X>
Polygon<T>& Polygon<T>::operator=(const Polygon<X>& p)
{
    vertices = p.vertices;
    return *this;
}

template <class T>
Polygon<T> Polygon<T>::operator+(const Vec2<T>& v) const
{
    Polygon newPolygon(*this);
    newPolygon += v;
    return newPolygon;
}
template <class T>
Polygon<T>& Polygon<T>::operator+=(const Vec2<T>& v)
{
    for(Vec2<T>& vertex : vertices)
    {
        vertex += v;
    }
    return *this;
}
template <class T>
Polygon<T> Polygon<T>::operator-(const Vec2<T>& v) const
{
    Polygon newPolygon(*this);
    newPolygon -= v;
    return newPolygon;
}
template <class T>
Polygon<T>& Polygon<T>::operator-=(const Vec2<T>& v)
{
    for(Vec2<T>& vertex : vertices)
    {
        vertex -= v;
    }
    return *this;
}

template <class T>
void Polygon<T>::add(const Vec2<T>& v)
{
    vertices.push_back(v);
}
template <class T>
void Polygon<T>::add(const std::vector<T>& v)
{
    vertices.insert(vertices.end(), v.begin(), v.end());
}
template <class T>
void Polygon<T>::add(const std::initializer_list<Vec2<T>>& list)
{
    vertices.insert(vertices.end(), list.begin(), list.end());
}

template <class T>
void Polygon<T>::add(Vec2<T>&& v)
{
    vertices.push_back(v);
}
template <class T>
void Polygon<T>::add(std::vector<T>&& v)
{
    vertices.insert(vertices.end(), v.begin(), v.end());
}

template <class T>
void Polygon<T>::translate(const Vec2<T>& v)
{

}
template <class T>
void Polygon<T>::scale(const Vec2<T>& c, const Vec2<T>& s)
{

}
template <class T>
void Polygon<T>::scale(const Vec2<T>& c, const T s)
{

}
template <class T>
void Polygon<T>::scale(const Vec2<T>& s)
{

}
template <class T>
void Polygon<T>::scale(const T s)
{

}
template <class T>
void Polygon<T>::transform(const std::function<void(Vec2<T>&)>& transformationFunction)
{
    for(auto& v : vertices)
    {
        transformationFunction(v);
    }
}
template <class T>
void Polygon<T>::transform(const Transformation2<T>& transformation)
{
    for(auto& v : vertices)
    {
        transformation.transform(v);
    }
}
template <class T>
Polygon<T> Polygon<T>::transformed(const std::function<void(Vec2<T>&)>& transformationFunction) const
{
    Polygon<T> copy(*this);
    copy.transform(transformationFunction);
    return copy;
}
template <class T>
Polygon<T> Polygon<T>::transformed(const Transformation2<T>& transformation) const
{
    Polygon<T> copy(*this);
    copy.transform(transformation);
    return copy;
}

template <class T>
Vec2<T> Polygon<T>::project(const Vec2<T>& b) const
{
    Vec2<T> projection;

    return projection;
}

template <class T>
std::pair<T, T> Polygon<T>::projectMinMax(const Vec2<T>& b) const
{
    T minProjection;
    T maxProjection;
    minProjection = maxProjection = vertices[0].dot(b);
    for(const Vec2<T>& vertex : vertices)
    {
        T dotProduct = vertex.dot(b);
        if(dotProduct < minProjection) minProjection = dotProduct;
        else if(dotProduct > maxProjection) maxProjection = dotProduct;
    }
    return std::pair<T, T>(minProjection, maxProjection);
}
template <class T>
std::unique_ptr<typename Shape2<T>::RandomPointPickerPreprocessedData> Polygon<T>::createPreprocessedDataForRandomPointPicker() const
{
    return std::make_unique<RandomPointPickerPreprocessedData>();
}
template <class T>
Polyline<T> Polygon<T>::asPolyline() const
{
    Polyline<T> out(vertices);
    out.add(out.vertices[0]);
    return out;
}
template <class T>
Vec2<T> Polygon<T>::center() const
{
    double xsum = 0.0;
    double ysum = 0.0;
    double area = 0.0;
    for(size_t i = 0; i < size(); ++i)
    {
        const Vec2D& p0 = vertices[i];
        const Vec2D& p1 = vertices[(i + 1) % size()];

        double areaSum = (p0.x * p1.y) - (p1.x * p0.y);

        xsum += (p0.x + p1.x) * areaSum;
        ysum += (p0.y + p1.y) * areaSum;

        area += areaSum;
    }

    double centMassX = xsum / (area * 3.0);
    double centMassY = ysum / (area * 3.0);

    return Vec2D {centMassX, centMassY};
}

template <class T>
size_t Polygon<T>::size() const
{
    return vertices.size();
}

//for double dispatch
template <class T>
bool Polygon<T>::intersects(const Shape2<T>* other) const
{
    return other->intersects(*this);
}
//specifications fo double dispatch
template <class T>
bool Polygon<T>::intersects(const Circle<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class T>
bool Polygon<T>::intersects(const LineSegment<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class T>
bool Polygon<T>::intersects(const Polygon<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class T>
bool Polygon<T>::intersects(const Polyline<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class T>
bool Polygon<T>::intersects(const Ray<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class T>
bool Polygon<T>::intersects(const Rectangle<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class T>
bool Polygon<T>::intersects(const Triangle<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class T>
bool Polygon<T>::intersects(const Vec2<T>& other) const
{
    return Intersections::intersection(other, *this);
}

template <class T>
Polygon<T> Polygon<T>::regular(const Vec2D& center, int sides, int radius)
{
    Polygon<T> polygon;
    polygon.vertices.reserve(sides);
    for(int i = 0; i < sides; ++i)
    {
        polygon.vertices.emplace_back(radius * std::cos(2.0 * PI * i / sides), radius * std::sin(2.0 * PI * i / sides));
    }
    return polygon;
}
template <class T>
Polygon<T> Polygon<T>::fromRectangle(const Rectangle<T>& rectangle)
{
    return Polygon<T> {Vec2<T>{rectangle.min.x, rectangle.min.y},
                       Vec2<T>{rectangle.max.x, rectangle.min.y},
                       Vec2<T>{rectangle.max.x, rectangle.max.y},
                       Vec2<T>{rectangle.min.x, rectangle.max.y}
                      };
}
template <class T>
Polygon<T> Polygon<T>::fromTriangle(const Triangle<T>& triangle)
{
    return Polygon<T> {triangle.vertices[0], triangle.vertices[1], triangle.vertices[2]};
}
template <class T>
std::unique_ptr<Shape2<T>> Polygon<T>::clone() const
{
    return std::make_unique<Polygon<T>>(*this);
}
