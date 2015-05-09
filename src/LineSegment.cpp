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
template <class Transformation>
void LineSegment<T>::transform(Transformation&& func)
{
    func(*this);
}

template <class T>
Polyline<T> LineSegment<T>::outline() const
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
std::vector<Vec2<T>> LineSegment<T>::pickRandomPoints(size_t quantity, Random::RandomEngineBase& randomEngine) const
{
    std::vector<Vec2<T>> result;
    result.reserve(quantity);
    while(quantity--)
    {
        result.emplace_back(pickRandomPoint(randomEngine));
    }
    return result;
}
template <class T>
Vec2<T> LineSegment<T>::pickRandomPoint(Random::RandomEngineBase& randomEngine, typename Shape2<T>::RandomPointPickerPreprocessedData& preprocessedData) const
{
    return pickRandomPoint(randomEngine);
}
template <class T>
std::vector<Vec2<T>> LineSegment<T>::pickRandomPoints(size_t quantity, Random::RandomEngineBase& randomEngine, typename Shape2<T>::RandomPointPickerPreprocessedData& preprocessedData) const
{
    return pickRandomPoints(quantity, randomEngine);
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
