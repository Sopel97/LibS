template <class T>
Circle<T>::Circle(const Vec2<T>& p1, T r) : origin(p1), radius(r)
{
}
template <class T>
template <class X>
Circle<T>::Circle(const Circle<X>& c) : origin(c.origin), radius(c.radius)
{
}
template <class T>
template <class X>
Circle<T>& Circle<T>::operator =(const Circle<X>& c)
{
    origin = c.origin;
    radius = c.radius;
    return *this;
}
template <class T>
Circle<T> Circle<T>::operator +(const Vec2<T>& v) const
{
    return Circle<T>(origin + v, radius);
}
template <class T>
Circle<T>& Circle<T>::operator +=(const Vec2<T>& v)
{
    origin += v;
    return *this;
}
template <class T>
Circle<T> Circle<T>::operator -(const Vec2<T>& v) const
{
    return Circle<T>(origin - v, radius);
}
template <class T>
Circle<T>& Circle<T>::operator -=(const Vec2<T>& v)
{
    origin -= v;
    return *this;
}

template <class T>
void Circle<T>::translate(const Vec2<T>& v)
{

}
template <class T>
void Circle<T>::scale(const Vec2<T>& c, const Vec2<T>& s)
{

}
template <class T>
void Circle<T>::scale(const Vec2<T>& c, const T s)
{

}
template <class T>
void Circle<T>::scale(const Vec2<T>& s)
{

}
template <class T>
template <class Transformation>
void Circle<T>::transform(Transformation&& func)
{
    func(*this);
}
template <class T>
void Circle<T>::scale(const T s)
{

}

template <class T>
Polyline<T> Circle<T>::asPolyline() const
{
    const int segmentCount = 64;
    Polyline<T> out;
    float angle = 0;
    float angleDiff = 2 * PI / (float)segmentCount;
    for(int i = 0; i < segmentCount; ++i)
    {
        angle += angleDiff;
        Vec2<T> next(cos(angle)*radius + origin.x, sin(angle)*radius + origin.y);
        out.add(next);
    }
    out.add(out.vertices[0]);
    return out;
}

template <class T>
Vec2<T> Circle<T>::pickRandomPoint(Random::RandomEngineBase& randomEngine) const
{
    T r = std::sqrt(randomEngine.nextDouble()) * radius;
    AngleD angle = AngleD::radians(randomEngine.nextDouble(0.0, 2 * PI));
    return origin + Vec2<T>(r * angle.cos(), r * angle.sin());
}
template <class T>
Vec2<T> Circle<T>::pickRandomPoint(Random::RandomEngineBase& randomEngine, typename Shape2<T>::RandomPointPickerPreprocessedData& preprocessedData) const
{
    return pickRandomPoint(randomEngine);
}
template <class T>
Vec2<T> Circle<T>::center() const
{
    return origin;
}

/* INTERSECTIONS */

//for double dispatch
template <class T>
bool Circle<T>::intersects(const Shape2<T>* other) const
{
    return other->intersects(*this);
}
//specifications fo double dispatch
template <class T>
bool Circle<T>::intersects(const Circle<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class T>
bool Circle<T>::intersects(const LineSegment<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class T>
bool Circle<T>::intersects(const Polygon<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class T>
bool Circle<T>::intersects(const Polyline<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class T>
bool Circle<T>::intersects(const Ray<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class T>
bool Circle<T>::intersects(const Rectangle<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class T>
bool Circle<T>::intersects(const Triangle<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class T>
bool Circle<T>::intersects(const Vec2<T>& other) const
{
    return Intersections::intersection(other, *this);
}

template <class T>
std::unique_ptr<Shape2<T>> Circle<T>::clone() const
{
    return std::make_unique<Circle<T>>(*this);
}
