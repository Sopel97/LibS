#include "../include/Intersections.h"

/* proxy */

template <class T, size_t X, size_t Y>
void Vec2Proxy<T, X, Y>::fill(T value)
{
    static_assert(isWritable, "Can't perform modifying operation on vector through proxy with ununique references.");
    getX() = value;
    getY() = value;
}
template <class T, size_t X, size_t Y>
template <class TSecond>
Vec2<typename std::common_type<T, TSecond>::type> Vec2Proxy<T, X, Y>::operator+(const Vec2<TSecond>& v1) const
{
    return Vec2<T>(getX() + v1.x, getY() + v1.y);
}
template <class T, size_t X, size_t Y>
template <class TSecond>
Vec2<typename std::common_type<T, TSecond>::type> Vec2Proxy<T, X, Y>::operator-(const Vec2<TSecond>& v1) const
{
    return Vec2<T>(getX() - v1.x, getY() - v1.y);
}
template <class T, size_t X, size_t Y>
template <class TSecond>
Vec2<typename std::common_type<T, TSecond>::type> Vec2Proxy<T, X, Y>::operator*(const TSecond scalar) const
{
    return Vec2<T>(getX() * scalar, getY() * scalar);
}
template <class T, size_t X, size_t Y>
template <class TSecond>
Vec2<typename std::common_type<T, TSecond>::type> Vec2Proxy<T, X, Y>::operator*(const Vec2<TSecond>& v1) const
{
    return Vec2<T>(getX() * v1.x, getY() * v1.y);
}
template <class T, size_t X, size_t Y>
template <class TSecond>
Vec2<typename std::common_type<T, TSecond>::type> Vec2Proxy<T, X, Y>::operator/(const TSecond scalar) const
{
    return Vec2<T>(getX() / scalar, getY() / scalar);
}
template <class T, size_t X, size_t Y>
template <class TSecond>
Vec2<typename std::common_type<T, TSecond>::type> Vec2Proxy<T, X, Y>::operator/(const Vec2<TSecond>& v1) const
{
    return Vec2<T>(getX() / v1.x, getY() / v1.y);
}

template <class T, size_t X, size_t Y>
Vec2<T> Vec2Proxy<T, X, Y>::operator-() const
{
    return Vec2<T>(-getX(), -getY());
}
template <class T, size_t X, size_t Y>
T Vec2Proxy<T, X, Y>::magnitude() const
{
    return std::sqrt(getX() * getX() + getY() * getY());
}
template <class T, size_t X, size_t Y>
T Vec2Proxy<T, X, Y>::quadrance() const
{
    return getX() * getX() + getY() * getY();
}
template <class T, size_t X, size_t Y>
T Vec2Proxy<T, X, Y>::distanceTo(const Vec2<T>& v1) const
{
    T dx = getX() - v1.x;
    T dy = getY() - v1.y;
    return std::sqrt(dx * dx + dy * dy);
}
template <class T, size_t X, size_t Y>
void Vec2Proxy<T, X, Y>::normalize()
{
    T invertedSquareRoot = 1. / std::sqrt(getX() * getX() + getY() * getY());
    getX() *= invertedSquareRoot;
    getY() *= invertedSquareRoot;
}
template <class T, size_t X, size_t Y>
Vec2<T> Vec2Proxy<T, X, Y>::normalized() const
{
    T invertedSquareRoot = 1. / std::sqrt(getX() * getX() + getY() * getY());
    return Vec2<T>(getX() * invertedSquareRoot, getY() * invertedSquareRoot);
}
template <class T, size_t X, size_t Y>
T Vec2Proxy<T, X, Y>::dot(const Vec2<T>& b) const
{
    return getX() * b.x + getY() * b.y;
}
template <class T, size_t X, size_t Y>
T Vec2Proxy<T, X, Y>::cross(const Vec2<T>& b) const
{
    return getX() * b.y - getY() * b.x;
}
template <class T, size_t X, size_t Y>
Vec2<T> Vec2Proxy<T, X, Y>::project(const Vec2<T>& b) const
{
    Vec2<T> projection;
    T firstPart = dot(b) / (b.x * b.x + b.y * b.y);
    projection.x = firstPart * b.x;
    projection.y = firstPart * b.y;
    return projection;
}
template <class T, size_t X, size_t Y>
Vec2<T> Vec2Proxy<T, X, Y>::normalLeft() const
{
    return Vec2<T>(-getY(), getX()).normalized();
}
template <class T, size_t X, size_t Y>
Vec2<T> Vec2Proxy<T, X, Y>::normalRight() const
{
    return Vec2<T>(getY(), -getX()).normalized();
}
template <class T, size_t X, size_t Y>
Vec2<T> Vec2Proxy<T, X, Y>::normal() const //right one
{
    return Vec2<T>(getY(), -getX()).normalized();
}
template <class T, size_t X, size_t Y>
Angle<T> Vec2Proxy<T, X, Y>::angle() const
{
    return Angle<T>::radians(atan2(getY(), getX()));
}
template <class T, size_t X, size_t Y>
Angle<T> Vec2Proxy<T, X, Y>::angle(const Vec2<T>& other) const
{
    return Angle<T>::radians(atan2(cross(other), dot(other)));
}

template <class T, size_t X, size_t Y>
Vec2Proxy<T, X, Y>& Vec2Proxy<T, X, Y>::operator=(const Vec2<T>& v1)
{
    static_assert(isWritable, "Can't perform modifying operation on vector through proxy with ununique references.");
    getX() = v1.x;
    getY() = v1.y;
    return *this;
}
template <class T, size_t X, size_t Y>
Vec2Proxy<T, X, Y>& Vec2Proxy<T, X, Y>::operator=(Vec2<T> && v1)
{
    static_assert(isWritable, "Can't perform modifying operation on vector through proxy with ununique references.");
    getX() = std::move(v1.x);
    getY() = std::move(v1.y);
    return *this;
}
template <class T, size_t X, size_t Y>
template <class U>
Vec2Proxy<T, X, Y>& Vec2Proxy<T, X, Y>::operator=(const Vec2<U>& v1)
{
    static_assert(isWritable, "Can't perform modifying operation on vector through proxy with ununique references.");
    getX() = v1.x;
    getY() = v1.y;
    return *this;
}
template <class T, size_t X, size_t Y>
Vec2Proxy<T, X, Y>& Vec2Proxy<T, X, Y>::operator+=(const Vec2<T>& v1)
{
    getX() += v1.x;
    getY() += v1.y;
    return *this;
}
template <class T, size_t X, size_t Y>
Vec2Proxy<T, X, Y>& Vec2Proxy<T, X, Y>::operator-=(const Vec2<T>& v1)
{
    static_assert(isWritable, "Can't perform modifying operation on vector through proxy with ununique references.");
    getX() -= v1.x;
    getY() -= v1.y;
    return *this;
}
template <class T, size_t X, size_t Y>
Vec2Proxy<T, X, Y>& Vec2Proxy<T, X, Y>::operator*=(const T scalar)
{
    static_assert(isWritable, "Can't perform modifying operation on vector through proxy with ununique references.");
    getX() *= scalar;
    getY() *= scalar;
    return *this;
}
template <class T, size_t X, size_t Y>
Vec2Proxy<T, X, Y>& Vec2Proxy<T, X, Y>::operator*=(const Vec2<T>& v1)
{
    static_assert(isWritable, "Can't perform modifying operation on vector through proxy with ununique references.");
    getX() *= v1.x;
    getY() *= v1.y;
    return *this;
}
template <class T, size_t X, size_t Y>
Vec2Proxy<T, X, Y>& Vec2Proxy<T, X, Y>::operator/=(const T scalar)
{
    static_assert(isWritable, "Can't perform modifying operation on vector through proxy with ununique references.");
    getX() /= scalar;
    getY() /= scalar;
    return *this;
}
template <class T, size_t X, size_t Y>
Vec2Proxy<T, X, Y>& Vec2Proxy<T, X, Y>::operator/=(const Vec2<T>& v1)
{
    static_assert(isWritable, "Can't perform modifying operation on vector through proxy with ununique references.");
    getX() /= v1.x;
    getY() /= v1.y;
    return *this;
}
template <class T, size_t X, size_t Y>
T& Vec2Proxy<T, X, Y>::operator[](size_t index)
{
    switch(index)
    {
        case 0:
            return getX();
            break;
        case 1:
            return getY();
            break;
    }
    return getX();
}
template <class T, size_t X, size_t Y>
const T& Vec2Proxy<T, X, Y>::operator[](size_t index) const
{
    switch(index)
    {
        case 0:
            return getX();
            break;
        case 1:
            return getY();
            break;
    }
    return getX();
}

template <class T, size_t X, size_t Y>
void Vec2Proxy<T, X, Y>::translate(const Vec2<T>& v)
{
    static_assert(isWritable, "Can't perform modifying operation on vector through proxy with ununique references.");

}
template <class T, size_t X, size_t Y>
void Vec2Proxy<T, X, Y>::scale(const Vec2<T>& c, const Vec2<T>& s)
{
    static_assert(isWritable, "Can't perform modifying operation on vector through proxy with ununique references.");

}
template <class T, size_t X, size_t Y>
void Vec2Proxy<T, X, Y>::scale(const Vec2<T>& c, const T s)
{
    static_assert(isWritable, "Can't perform modifying operation on vector through proxy with ununique references.");

}
template <class T, size_t X, size_t Y>
void Vec2Proxy<T, X, Y>::scale(const Vec2<T>& s)
{
    static_assert(isWritable, "Can't perform modifying operation on vector through proxy with ununique references.");

}
template <class T, size_t X, size_t Y>
void Vec2Proxy<T, X, Y>::scale(const T s)
{
    static_assert(isWritable, "Can't perform modifying operation on vector through proxy with ununique references.");

}
template <class T, size_t X, size_t Y>
template <class Transformation>
void Vec2Proxy<T, X, Y>::transform(Transformation&& func)
{
    static_assert(isWritable, "Can't perform modifying operation on vector through proxy with ununique references.");
    func(Vec2<T>(getX(), getY()));
}

//for double dispatch
template <class T, size_t X, size_t Y>
bool Vec2Proxy<T, X, Y>::intersects(const Shape2<T>* other) const
{
    return other->intersects(Vec2<T>(*this));
}
//specifications fo double dispatch
template <class T, size_t X, size_t Y>
bool Vec2Proxy<T, X, Y>::intersects(const Circle<T>& other) const
{
    return Intersections::intersection(other, Vec2<T>(*this));
}
template <class T, size_t X, size_t Y>
bool Vec2Proxy<T, X, Y>::intersects(const LineSegment<T>& other) const
{
    return Intersections::intersection(other, Vec2<T>(*this));
}
template <class T, size_t X, size_t Y>
bool Vec2Proxy<T, X, Y>::intersects(const Polygon<T>& other) const
{
    return Intersections::intersection(other, Vec2<T>(*this));
}
template <class T, size_t X, size_t Y>
bool Vec2Proxy<T, X, Y>::intersects(const Polyline<T>& other) const
{
    return Intersections::intersection(other, Vec2<T>(*this));
}
template <class T, size_t X, size_t Y>
bool Vec2Proxy<T, X, Y>::intersects(const Ray<T>& other) const
{
    return Intersections::intersection(other, Vec2<T>(*this));
}
template <class T, size_t X, size_t Y>
bool Vec2Proxy<T, X, Y>::intersects(const Rectangle<T>& other) const
{
    return Intersections::intersection(other, Vec2<T>(*this));
}
template <class T, size_t X, size_t Y>
bool Vec2Proxy<T, X, Y>::intersects(const Triangle<T>& other) const
{
    return Intersections::intersection(other, Vec2<T>(*this));
}
template <class T, size_t X, size_t Y>
bool Vec2Proxy<T, X, Y>::intersects(const Vec2<T>& other) const
{
    return Intersections::intersection(other, Vec2<T>(*this));
}

template <class T, size_t X, size_t Y>
Polyline<T> Vec2Proxy<T, X, Y>::outline() const
{
    return Polyline<T>({Vec2<T>(*this)});
}

template <class T, size_t X, size_t Y>
Vec2<T> Vec2Proxy<T, X, Y>::pickRandomPoint(Random::RandomEngineBase& randomEngine) const
{
    return Vec2<T>(*this);
}
template <class T, size_t X, size_t Y>
std::vector<Vec2<T>> Vec2Proxy<T, X, Y>::pickRandomPoints(size_t quantity, Random::RandomEngineBase& randomEngine) const
{
    std::vector<Vec2<T>> result;
    result.reserve(quantity);
    while(quantity--)
    {
        result.emplace_back(pickRandomPoint(randomEngine));
    }
    return result;
}
template <class T, size_t X, size_t Y>
Vec2<T> Vec2Proxy<T, X, Y>::pickRandomPoint(Random::RandomEngineBase& randomEngine, typename Shape2<T>::RandomPointPickerPreprocessedData& preprocessedData) const
{
    return pickRandomPoint(randomEngine);
}
template <class T, size_t X, size_t Y>
std::vector<Vec2<T>> Vec2Proxy<T, X, Y>::pickRandomPoints(size_t quantity, Random::RandomEngineBase& randomEngine, typename Shape2<T>::RandomPointPickerPreprocessedData& preprocessedData) const
{
    return pickRandomPoints(quantity, randomEngine);
}

template <class T, size_t X, size_t Y>
Vec2<T> Vec2Proxy<T, X, Y>::center() const
{
    return Vec2<T>(*this);
}
template <class T, size_t X, size_t Y>
std::unique_ptr<Shape2<T>> Vec2Proxy<T, X, Y>::clone() const
{
    return std::make_unique<Vec2<T>>(Vec2<T>(*this));
}


template <class T, size_t X, size_t Y>
Vec2Proxy<T, X, Y>::operator Vec2<T>() const
{
    return Vec2<T>(getX(), getY());
}
template <class T, size_t X, size_t Y>
T& Vec2Proxy<T, X, Y>::getX()
{
    return *(((T*)this) + X);
}
template <class T, size_t X, size_t Y>
const T& Vec2Proxy<T, X, Y>::getX() const
{
    return *(((T*)this) + X);
}
template <class T, size_t X, size_t Y>
T& Vec2Proxy<T, X, Y>::getY()
{
    return *(((T*)this) + Y);
}
template <class T, size_t X, size_t Y>
const T& Vec2Proxy<T, X, Y>::getY() const
{
    return *(((T*)this) + Y);
}

/* main vector class */

template <class T>
void Vec2<T>::fill(T value)
{
    x = value;
    y = value;
}
template <class T>
template <class TSecond>
Vec2<typename std::common_type<T, TSecond>::type> Vec2<T>::operator+(const Vec2<TSecond>& v1) const
{
    return Vec2<typename std::common_type<T, TSecond>::type>(x + v1.x, y + v1.y);
}
template <class T>
template <class TSecond>
Vec2<typename std::common_type<T, TSecond>::type> Vec2<T>::operator-(const Vec2<TSecond>& v1) const
{
    return Vec2<typename std::common_type<T, TSecond>::type>(x - v1.x, y - v1.y);
}
template <class T>
template <class TSecond>
Vec2<typename std::common_type<T, TSecond>::type> Vec2<T>::operator*(const TSecond scalar) const
{
    return Vec2<typename std::common_type<T, TSecond>::type>(x * scalar, y * scalar);
}
template <class T>
template <class TSecond>
Vec2<typename std::common_type<T, TSecond>::type> Vec2<T>::operator*(const Vec2<TSecond>& v1) const
{
    return Vec2<typename std::common_type<T, TSecond>::type>(x * v1.x, y * v1.y);
}
template <class T>
template <class TSecond>
Vec2<typename std::common_type<T, TSecond>::type> Vec2<T>::operator/(const TSecond scalar) const
{
    return Vec2<typename std::common_type<T, TSecond>::type>(x / scalar, y / scalar);
}
template <class T>
template <class TSecond>
Vec2<typename std::common_type<T, TSecond>::type> Vec2<T>::operator/(const Vec2<TSecond>& v1) const
{
    return Vec2<typename std::common_type<T, TSecond>::type>(x / v1.x, y / v1.y);
}

template <class T>
Vec2<T> Vec2<T>::operator-() const
{
    return Vec2<T>(-x, -y);
}
template <class T>
T Vec2<T>::magnitude() const
{
    return std::sqrt(x * x + y * y);
}
template <class T>
T Vec2<T>::quadrance() const
{
    return x * x + y * y;
}
template <class T>
T Vec2<T>::distanceTo(const Vec2<T>& v1) const
{
    T dx = x - v1.x;
    T dy = y - v1.y;
    return std::sqrt(dx * dx + dy * dy);
}
template <class T>
void Vec2<T>::normalize()
{
    T invertedSquareRoot = 1. / std::sqrt(x * x + y * y);
    x *= invertedSquareRoot;
    y *= invertedSquareRoot;
}
template <class T>
Vec2<T> Vec2<T>::normalized() const
{
    T invertedSquareRoot = 1. / std::sqrt(x * x + y * y);
    return Vec2<T>(x * invertedSquareRoot, y * invertedSquareRoot);
}
template <class T>
T Vec2<T>::dot(const Vec2<T>& b) const
{
    return x * b.x + y * b.y;
}
template <class T>
T Vec2<T>::cross(const Vec2<T>& b) const
{
    return x * b.y - y * b.x;
}
template <class T>
Vec2<T> Vec2<T>::project(const Vec2<T>& b) const
{
    Vec2<T> projection;
    T firstPart = dot(b) / (b.x * b.x + b.y * b.y);
    projection.x = firstPart * b.x;
    projection.y = firstPart * b.y;
    return projection;
}
template <class T>
Vec2<T> Vec2<T>::normalLeft() const
{
    return Vec2<T>(-y, x).normalized();
}
template <class T>
Vec2<T> Vec2<T>::normalRight() const
{
    return Vec2<T>(y, -x).normalized();
}
template <class T>
Vec2<T> Vec2<T>::normal() const //right one
{
    return Vec2<T>(y, -x).normalized();
}
template <class T>
Angle<T> Vec2<T>::angle() const
{
    return Angle<T>::radians(atan2(y, x));
}
template <class T>
Angle<T> Vec2<T>::angle(const Vec2<T>& other) const
{
    return Angle<T>::radians(atan2(cross(other), dot(other)));
}


template <class T>
Vec2<T>::Vec2(T _xy) : x(_xy), y(_xy)
{
}
template <class T>
Vec2<T>::Vec2() : x(0), y(0)
{
};
template <class T>
Vec2<T>::Vec2(T _x, T _y) : x(_x), y(_y)
{
}
template <class T>
Vec2<T>::Vec2(const Vec2<T>& v) : x(v.x), y(v.y)
{

}
template <class T>
Vec2<T>::Vec2(Vec2<T>&& v) : x(std::move(v.x)), y(std::move(v.y))
{

}
template <class T>
Vec2<T>::Vec2(const std::initializer_list<T>& list)
{
    typename std::initializer_list<T>::iterator it = list.begin();
    x = *it;
    y = *(it + 1);
}
template <class T>
Vec2<T>& Vec2<T>::operator=(const Vec2<T>& v1)
{
    x = v1.x;
    y = v1.y;
    return *this;
}
template <class T>
Vec2<T>& Vec2<T>::operator=(Vec2<T> && v1)
{
    x = std::move(v1.x);
    y = std::move(v1.y);
    return *this;
}
template <class T>
template <class X>
Vec2<T>::Vec2(const Vec2<X>& v) : x(v.x), y(v.y)
{
}
template <class T>
template <class X>
Vec2<T>& Vec2<T>::operator=(const Vec2<X>& v1)
{
    x = v1.x;
    y = v1.y;
    return *this;
}
template <class T>
Vec2<T>& Vec2<T>::operator+=(const Vec2<T>& v1)
{
    x += v1.x;
    y += v1.y;
    return *this;
}
template <class T>
Vec2<T>& Vec2<T>::operator-=(const Vec2<T>& v1)
{
    x -= v1.x;
    y -= v1.y;
    return *this;
}
template <class T>
Vec2<T>& Vec2<T>::operator*=(const T scalar)
{
    x *= scalar;
    y *= scalar;
    return *this;
}
template <class T>
Vec2<T>& Vec2<T>::operator*=(const Vec2<T>& v1)
{
    x *= v1.x;
    y *= v1.y;
    return *this;
}
template <class T>
Vec2<T>& Vec2<T>::operator/=(const T scalar)
{
    x /= scalar;
    y /= scalar;
    return *this;
}
template <class T>
Vec2<T>& Vec2<T>::operator/=(const Vec2<T>& v1)
{
    x /= v1.x;
    y /= v1.y;
    return *this;
}
template <class T>
T& Vec2<T>::operator[](size_t index)
{
    return v[index];
}
template <class T>
const T& Vec2<T>::operator[](size_t index) const
{
    return v[index];
}

template <class T>
void Vec2<T>::translate(const Vec2<T>& v)
{

}
template <class T>
void Vec2<T>::scale(const Vec2<T>& c, const Vec2<T>& s)
{

}
template <class T>
void Vec2<T>::scale(const Vec2<T>& c, const T s)
{

}
template <class T>
void Vec2<T>::scale(const Vec2<T>& s)
{

}
template <class T>
void Vec2<T>::scale(const T s)
{

}
template <class T>
template <class Transformation>
void Vec2<T>::transform(Transformation&& func)
{
    func(*this);
}

//for double dispatch
template <class T>
bool Vec2<T>::intersects(const Shape2<T>* other) const
{
    return other->intersects(*this);
}
//specifications fo double dispatch
template <class T>
bool Vec2<T>::intersects(const Circle<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class T>
bool Vec2<T>::intersects(const LineSegment<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class T>
bool Vec2<T>::intersects(const Polygon<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class T>
bool Vec2<T>::intersects(const Polyline<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class T>
bool Vec2<T>::intersects(const Ray<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class T>
bool Vec2<T>::intersects(const Rectangle<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class T>
bool Vec2<T>::intersects(const Triangle<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class T>
bool Vec2<T>::intersects(const Vec2<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class T>
std::unique_ptr<Shape2<T>> Vec2<T>::clone() const
{
    return std::make_unique<Vec2<T>>(*this);
}

template <class T>
Polyline<T> Vec2<T>::outline() const
{
    return Polyline<T>({*this});
}
template <class T>
Vec2<T> Vec2<T>::pickRandomPoint(Random::RandomEngineBase& randomEngine) const
{
    return *this;
}
template <class T>
std::vector<Vec2<T>> Vec2<T>::pickRandomPoints(size_t quantity, Random::RandomEngineBase& randomEngine) const
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
Vec2<T> Vec2<T>::pickRandomPoint(Random::RandomEngineBase& randomEngine, typename Shape2<T>::RandomPointPickerPreprocessedData& preprocessedData) const
{
    return pickRandomPoint(randomEngine);
}
template <class T>
std::vector<Vec2<T>> Vec2<T>::pickRandomPoints(size_t quantity, Random::RandomEngineBase& randomEngine, typename Shape2<T>::RandomPointPickerPreprocessedData& preprocessedData) const
{
    return pickRandomPoints(quantity, randomEngine);
}

template <class T>
Vec2<T> Vec2<T>::center() const
{
    return *this;
}
template <class T>
Vec2<T> Vec2<T>::direction(const Angle<T>& angle)
{

}

/* non-member functions */
/*
template <class TFirst, class TSecond>
Vec2<typename std::common_type<TFirst, TSecond>::type> operator+(TFirst lhs, const Vec2<TSecond>& rhs)
{
    return Vec2<typename std::common_type<TFirst, TSecond>::type>(lhs + rhs.x, lhs + rhs.y);
}
template <class TFirst, class TSecond>
Vec2<typename std::common_type<TFirst, TSecond>::type> operator-(TFirst lhs, const Vec2<TSecond>& rhs)
{
    return Vec2<typename std::common_type<TFirst, TSecond>::type>(lhs - rhs.x, lhs - rhs.y);
}
template <class TFirst, class TSecond>
Vec2<typename std::common_type<TFirst, TSecond>::type> operator*(TFirst lhs, const Vec2<TSecond>& rhs)
{
    return Vec2<typename std::common_type<TFirst, TSecond>::type>(lhs * rhs.x, lhs * rhs.y);
}
template <class TFirst, class TSecond>
Vec2<typename std::common_type<TFirst, TSecond>::type> operator/(TFirst lhs, const Vec2<TSecond>& rhs)
{
    return Vec2<typename std::common_type<TFirst, TSecond>::type>(lhs / rhs.x, lhs / rhs.y);
}

template <class T, size_t X, size_t Y>
Vec2<T> operator+(T lhs, const Vec2Proxy<T, X, Y>& rhs)
{
    return Vec2<T>(lhs + rhs.getX(), lhs + rhs.getY());
}
template <class T, size_t X, size_t Y>
Vec2<T> operator-(T lhs, const Vec2Proxy<T, X, Y>& rhs)
{
    return Vec2<T>(lhs - rhs.getX(), lhs - rhs.getY());
}
template <class T, size_t X, size_t Y>
Vec2<T> operator*(T lhs, const Vec2Proxy<T, X, Y>& rhs)
{
    return Vec2<T>(lhs * rhs.getX(), lhs * rhs.getY());
}
template <class T, size_t X, size_t Y>
Vec2<T> operator/(T lhs, const Vec2Proxy<T, X, Y>& rhs)
{
    return Vec2<T>(lhs / rhs.getX(), lhs / rhs.getY());
}
*/
