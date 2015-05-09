#ifndef CIRCLE_H_INCLUDED
#define CIRCLE_H_INCLUDED

template <class T>
class Circle : public Shape2<T>
{
public:
    Vec2<T> origin;
    T radius;

    Circle() = default;
    Circle(const Vec2<T>& p1, T r);
    virtual ~Circle(){}

    Circle(const Circle<T>& c) = default;
    Circle(Circle<T>&& c) = default;

    Circle& operator =(Circle<T> && c) = default;
    Circle& operator =(const Circle<T>& c) = default;

    template<class X>
    Circle(const Circle<X>& c);
    template<class X>
    Circle& operator =(const Circle<X>& c);

    Circle<T> operator +(const Vec2<T>& v) const;
    Circle<T>& operator +=(const Vec2<T>& v);
    Circle<T> operator -(const Vec2<T>& v) const;
    Circle<T>& operator -=(const Vec2<T>& v);

    void translate(const Vec2<T>& v);
    void scale(const Vec2<T>& c, const Vec2<T>& s);
    void scale(const Vec2<T>& c, const T s);
    void scale(const Vec2<T>& s);
    void scale(const T s);

    template <class Transformation>
    void transform(Transformation&& func);

    Polyline<T> asPolyline() const;

    virtual Vec2<T> pickRandomPoint(Random::RandomEngineBase& randomEngine) const;
    virtual Vec2<T> pickRandomPoint(Random::RandomEngineBase& randomEngine, typename Shape2<T>::RandomPointPickerPreprocessedData& preprocessedData) const; //preprocessed data is of base type. All shapes have to cast it to use it.
    virtual Vec2<T> center() const;

    /* INTERSECTIONS */

    //for double dispatch
    virtual bool intersects(const Shape2<T>* other) const;
    //specifications fo double dispatch
    virtual bool intersects(const Circle<T>& other) const;
    virtual bool intersects(const LineSegment<T>& other) const;
    virtual bool intersects(const Polygon<T>& other) const;
    virtual bool intersects(const Polyline<T>& other) const;
    virtual bool intersects(const Ray<T>& other) const;
    virtual bool intersects(const Rectangle<T>& other) const;
    virtual bool intersects(const Triangle<T>& other) const;
    virtual bool intersects(const Vec2<T>& other) const;

    virtual std::unique_ptr<Shape2<T>> clone() const;
};

typedef Circle<double> CircleD;
typedef Circle<float> CircleF;
typedef Circle<int> CircleI;

#include "../src/Circle.cpp"

#endif // CIRCLE_H_INCLUDED
