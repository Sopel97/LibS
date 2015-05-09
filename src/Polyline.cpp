template <class T>
Polyline<T>::Polyline(const std::initializer_list<Vec2<T>>& list) : vertices(list)
{
}
template <class T>
Polyline<T>::Polyline(const std::vector<Vec2<T>>& v) : vertices(v)
{
}
template <class T>
Polyline<T>::Polyline(std::vector<Vec2<T>>&& v) : vertices(std::move(v))
{
}
template <class T>
Polyline<T>::Polyline(Vec2<T>* v, size_t count)
{
    vertices.assign(v, v + count);
}
template <class T>
template <class X>
Polyline<T>::Polyline(const Polyline<X>& p) : vertices(p.vertives)
{
}
template <class T>
template <class X>
Polyline<T>& Polyline<T>::operator=(const Polyline<X>& p)
{
    vertices = p.vertices;
    return *this;
}

template <class T>
Polyline<T> Polyline<T>::operator+(const Vec2<T>& v) const
{
    Polyline newPolyline(*this);
    newPolyline += v;
    return newPolyline;
}
template <class T>
Polyline<T>& Polyline<T>::operator+=(const Vec2<T>& v)
{
    for(Vec2<T>& vertex : vertices)
    {
        vertex += v;
    }
    return *this;
}
template <class T>
Polyline<T> Polyline<T>::operator-(const Vec2<T>& v) const
{
    Polyline newPolyline(*this);
    newPolyline -= v;
    return newPolyline;
}
template <class T>
Polyline<T>& Polyline<T>::operator-=(const Vec2<T>& v)
{
    for(Vec2<T>& vertex : vertices)
    {
        vertex -= v;
    }
    return *this;
}

template <class T>
void Polyline<T>::add(const Vec2<T>& v)
{
    vertices.push_back(v);
}
template <class T>
void Polyline<T>::add(const std::vector<T>& v)
{
    vertices.insert(vertices.end(), v.begin(), v.end());
}
template <class T>
void Polyline<T>::add(const std::initializer_list<Vec2<T>>& list)
{
    vertices.insert(vertices.end(), list.begin(), list.end());
}

template <class T>
void Polyline<T>::add(Vec2<T>&& v)
{
    vertices.push_back(v);
}
template <class T>
void Polyline<T>::add(std::vector<T>&& v)
{
    vertices.insert(vertices.end(), v.begin(), v.end());
}

template <class T>
void Polyline<T>::translate(const Vec2<T>& v)
{

}
template <class T>
void Polyline<T>::scale(const Vec2<T>& c, const Vec2<T>& s)
{

}
template <class T>
void Polyline<T>::scale(const Vec2<T>& c, const T s)
{

}
template <class T>
void Polyline<T>::scale(const Vec2<T>& s)
{

}
template <class T>
void Polyline<T>::scale(const T s)
{

}
template <class T>
template <class Transformation>
void Polyline<T>::transform(Transformation&& func)
{
    func(*this);
}

template <class T>
Vec2<T> Polyline<T>::project(const Vec2<T>& b) const
{
    Vec2<T> projection;

    return projection;
}

template <class T>
std::pair<T, T> Polyline<T>::projectMinMax(const Vec2<T>& b) const
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
std::unique_ptr<typename Shape2<T>::RandomPointPickerPreprocessedData> Polyline<T>::createPreprocessedDataForRandomPointPicker() const
{
    return std::make_unique<RandomPointPickerPreprocessedData>();
}
template <class T>
Polyline<T> Polyline<T>::asPolyline() const
{
    return *this;
}

template <class T>
size_t Polyline<T>::size() const
{
    return vertices.size();
}

//for double dispatch
template <class T>
bool Polyline<T>::intersects(const Shape2<T>* other) const
{
    return other->intersects(*this);
}
//specifications fo double dispatch
template <class T>
bool Polyline<T>::intersects(const Circle<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class T>
bool Polyline<T>::intersects(const LineSegment<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class T>
bool Polyline<T>::intersects(const Polygon<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class T>
bool Polyline<T>::intersects(const Polyline<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class T>
bool Polyline<T>::intersects(const Ray<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class T>
bool Polyline<T>::intersects(const Rectangle<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class T>
bool Polyline<T>::intersects(const Triangle<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class T>
bool Polyline<T>::intersects(const Vec2<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class T>
Polyline<T> Polyline<T>::fromRectangle(const Rectangle<T>& rectangle)
{
    return Polyline<T> {Vec2<T>{rectangle.min.x, rectangle.min.y},
                        Vec2<T>{rectangle.max.x, rectangle.min.y},
                        Vec2<T>{rectangle.max.x, rectangle.max.y},
                        Vec2<T>{rectangle.min.x, rectangle.max.y},
                        Vec2<T>{rectangle.min.x, rectangle.min.y}
                       };
}
template <class T>
Polyline<T> Polyline<T>::fromTriangle(const Triangle<T>& triangle)
{
    return Polyline<T> {triangle.vertices[0], triangle.vertices[1], triangle.vertices[2], triangle.vertices[0]};
}
template <class T>
std::unique_ptr<Shape2<T>> Polyline<T>::clone() const
{
    return std::make_unique<Polyline<T>>(*this);
}
