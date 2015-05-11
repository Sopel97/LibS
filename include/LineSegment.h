#ifndef LINESEGMENT_H_INCLUDED
#define LINESEGMENT_H_INCLUDED

template <class T>
class LineSegment : public Shape2<T>
{
public:
    Vec2<T> begin;
    Vec2<T> end;

    LineSegment() = default;
    LineSegment(const Vec2<T>& _begin, const Vec2<T>& _end);

    LineSegment(const LineSegment<T>& s) = default;
    LineSegment(LineSegment<T>&& s) = default;

    LineSegment<T>& operator=(const LineSegment<T>& s) = default;
    LineSegment<T>& operator=(LineSegment<T> && s) = default;

    template <class X>
    LineSegment(const LineSegment<X>& s);
    template <class X>
    LineSegment<T>& operator=(const LineSegment<X>& s);

    LineSegment<T> operator+(const Vec2<T>& v) const;
    LineSegment<T>& operator+=(const Vec2<T>& v);
    LineSegment<T> operator-(const Vec2<T>& v) const;
    LineSegment<T>& operator-=(const Vec2<T>& v);

    T length() const;

    void translate(const Vec2<T>& v);
    void scale(const Vec2<T>& c, const Vec2<T>& s);
    void scale(const Vec2<T>& c, const T s);
    void scale(const Vec2<T>& s);
    void scale(const T s);

    virtual void transform(const std::function<void(Vec2<T>&)>& transformationFunction);
    virtual void transform(const Transformation2<T>& transformation);
    LineSegment<T> transformed(const std::function<void(Vec2<T>&)>& transformationFunction) const;
    LineSegment<T> transformed(const Transformation2<T>& transformation) const;

    Polyline<T> asPolyline() const;

    virtual Vec2<T> pickRandomPoint(Random::RandomEngineBase& randomEngine) const;
    virtual Vec2<T> pickRandomPoint(Random::RandomEngineBase& randomEngine, typename Shape2<T>::RandomPointPickerPreprocessedData& preprocessedData) const; //preprocessed data is of base type. All shapes have to cast it to use it.
    virtual Vec2<T> center() const;

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

    bool intersects(const LineSegment<T>& lineSegment, Vec2<T>& intersectionPoint) const;

    virtual std::unique_ptr<Shape2<T>> clone() const;

    virtual ~LineSegment(){}
};

typedef LineSegment<double> LineSegmentD;
typedef LineSegment<float> LineSegmentF;
typedef LineSegment<int> LineSegmentI;

#include "../src/LineSegment.cpp"
#endif // LINESEGMENT_H_INCLUDED
