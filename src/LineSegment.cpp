template <class T>
LineSegment<T>::LineSegment(const Vec2<T>& _begin, const Vec2<T>& _end) : begin(_begin), end(_end)
{
}

template <class T>
template <class X>
LineSegment<T>::LineSegment(const LineSegment<X>& s) : begin(s._begin), end(s._end)
{
}
template <class T>
template <class X>
LineSegment<T>& LineSegment<T>::operator=(const LineSegment<X>& s)
{
    begin = s.begin;
    end = s.end;
    return *this;
}

template <class T>
LineSegment<T> LineSegment<T>::operator+(const Vec2<T>& v) const
{
    return LineSegment<T>(begin + v, end + v);
}
template <class T>
LineSegment<T>& LineSegment<T>::operator+=(const Vec2<T>& v)
{
    begin += v;
    end += v;
    return *this;
}
template <class T>
LineSegment<T> LineSegment<T>::operator-(const Vec2<T>& v) const
{
    return LineSegment<T>(begin - v, end - v);
}
template <class T>
LineSegment<T>& LineSegment<T>::operator-=(const Vec2<T>& v)
{
    begin -= v;
    end -= v;
    return *this;
}

template <class T>
T LineSegment<T>::length() const
{
    return begin.distanceTo(end);
}
template <class T>
void LineSegment<T>::translate(const Vec2<T>& v)
{

}
template <class T>
void LineSegment<T>::scale(const Vec2<T>& c, const Vec2<T>& s)
{

}
template <class T>
void LineSegment<T>::scale(const Vec2<T>& c, const T s)
{

}
template <class T>
void LineSegment<T>::scale(const Vec2<T>& s)
{

}
template <class T>
void LineSegment<T>::scale(const T s)
{

}
template <class T>
void LineSegment<T>::transform(const std::function<void(Vec2<T>&)>& transformationFunction)
{
    transformationFunction(begin);
    transformationFunction(end);
}
template <class T>
void LineSegment<T>::transform(const Transformation2<T>& transformation)
{
    transformation.transform(begin);
    transformation.transform(end);
}
template <class T>
LineSegment<T> LineSegment<T>::transformed(const std::function<void(Vec2<T>&)>& transformationFunction) const
{
    LineSegment<T> copy(*this);
    copy.transform(transformationFunction);
    return copy;
}
template <class T>
LineSegment<T> LineSegment<T>::transformed(const Transformation2<T>& transformation) const
{
    LineSegment<T> copy(*this);
    copy.transform(transformation);
    return copy;
}
template <class T>
Polyline<T> LineSegment<T>::asPolyline() const
{
    return Polyline<T>({begin, end});
}

template <class T>
Vec2<T> LineSegment<T>::pickRandomPoint(Random::RandomEngineBase& randomEngine) const
{
    T t = randomEngine.nextDouble();
    return begin + (end - begin) * t;
}
template <class T>
Vec2<T> LineSegment<T>::pickRandomPoint(Random::RandomEngineBase& randomEngine, typename Shape2<T>::RandomPointPickerPreprocessedData& preprocessedData) const
{
    return pickRandomPoint(randomEngine);
}
template <class T>
Vec2<T> LineSegment<T>::center() const
{
    return (begin + end) * 0.5;
}

template <class T>
bool LineSegment<T>::intersects(const LineSegment<T>& lineSegment, Vec2<T>& intersectionPoint) const
{
    return Intersections::intersection(*this, lineSegment, intersectionPoint);
}

//for double dispatch
template <class T>
bool LineSegment<T>::intersects(const Shape2<T>* other) const
{
    return other->intersects(*this);
}
//specifications fo double dispatch
template <class T>
bool LineSegment<T>::intersects(const Circle<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class T>
bool LineSegment<T>::intersects(const LineSegment<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class T>
bool LineSegment<T>::intersects(const Polygon<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class T>
bool LineSegment<T>::intersects(const Polyline<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class T>
bool LineSegment<T>::intersects(const Ray<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class T>
bool LineSegment<T>::intersects(const Rectangle<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class T>
bool LineSegment<T>::intersects(const Triangle<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class T>
bool LineSegment<T>::intersects(const Vec2<T>& other) const
{
    return Intersections::intersection(other, *this);
}

template <class T>
std::unique_ptr<Shape2<T>> LineSegment<T>::clone() const
{
    return std::make_unique<LineSegment<T>>(*this);
}
