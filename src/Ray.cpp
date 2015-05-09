template <class T>
Ray<T>::Ray(const Vec2<T>& o, const Vec2<T>& d) : origin(o), direction(d.normalized())
{

}

template <class T>
template <class X>
Ray<T>::Ray(const Ray<X>& r) : origin(r.origin), direction(r.direction)
{

}
template <class T>
template <class X>
Ray<T>& Ray<T>::operator=(const Ray<X>& r)
{
    origin = r.origin;
    direction = r.direction;
}

template <class T>
Ray<T> Ray<T>::operator+(const Vec2<T>& v) const
{
    Ray<T> r(*this);
    r.origin += v;
    return r;
}
template <class T>
Ray<T>& Ray<T>::operator+=(const Vec2<T>& v)
{
    origin += v;
    return *this;
}
template <class T>
Ray<T> Ray<T>::operator-(const Vec2<T>& v) const
{
    Ray<T> r(*this);
    r.origin -= v;
    return r;
}
template <class T>
Ray<T>& Ray<T>::operator-=(const Vec2<T>& v)
{
    origin -= v;
    return *this;
}

template <class T>
void Ray<T>::setDirection(const Vec2<T>& d)
{
    direction = d.normalized();
}

template <class T>
void Ray<T>::translate(const Vec2<T>& v)
{
    origin += v;
}
template <class T>
void Ray<T>::scale(const Vec2<T>& c, const Vec2<T>& s)
{

}
template <class T>
void Ray<T>::scale(const Vec2<T>& c, const T s)
{

}
template <class T>
void Ray<T>::scale(const Vec2<T>& s)
{

}
template <class T>
void Ray<T>::scale(const T s)
{

}

template <class T>
template <class Transformation>
void Ray<T>::transform(Transformation&& func)
{

}

template <class T>
Polyline<T> Ray<T>::outline() const
{
    //dont know
    return Polyline<T>({origin});
}

//for double dispatch
template <class T>
bool Ray<T>::intersects(const Shape2<T>* other) const
{
    return other->intersects(*this);
}
//specifications fo double dispatch
template <class T>
bool Ray<T>::intersects(const Circle<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class T>
bool Ray<T>::intersects(const LineSegment<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class T>
bool Ray<T>::intersects(const Polygon<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class T>
bool Ray<T>::intersects(const Polyline<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class T>
bool Ray<T>::intersects(const Ray<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class T>
bool Ray<T>::intersects(const Rectangle<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class T>
bool Ray<T>::intersects(const Triangle<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class T>
bool Ray<T>::intersects(const Vec2<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class T>
std::unique_ptr<Shape2<T>> Ray<T>::clone() const
{
    return std::make_unique<Ray<T>>(*this);
}
