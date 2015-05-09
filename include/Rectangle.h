#ifndef RECTANGLE_H_INCLUDED
#define RECTANGLE_H_INCLUDED

template <class T>
class Rectangle : public Shape2<T>
{
public:
    Vec2<T> min, max;
    Rectangle() = default;
    Rectangle(const Vec2<T>& p1, const Vec2<T>& p2);
    Rectangle(const Vec2<T>& p1, const T width, const T height);

    Rectangle(const Rectangle<T>& r) = default;
    Rectangle(Rectangle<T>&& r) = default;
    Rectangle<T>& operator =(const Rectangle<T>& r) = default;
    Rectangle<T>& operator =(Rectangle<T> && r) = default;

    template <class X>
    Rectangle(const Rectangle<X>& r);
    template <class X>
    Rectangle<T>& operator =(const Rectangle<X>& r);

    Rectangle<T> operator +(const Vec2<T>& v) const;
    Rectangle<T>& operator +=(const Vec2<T>& v);
    Rectangle<T> operator -(const Vec2<T>& v) const;
    Rectangle<T>& operator -=(const Vec2<T>& v);

    T width() const;
    T height() const;

    void translate(const Vec2<T>& v);
    void scale(const Vec2<T>& c, const Vec2<T>& s);
    void scale(const Vec2<T>& c, const T s);
    void scale(const Vec2<T>& s);
    void scale(const T s);

    template <class Transformation>
    void transform(Transformation&& func);

    Polyline<T> outline() const;

    virtual Vec2<T> pickRandomPoint(Random::RandomEngineBase& randomEngine) const;
    virtual Vec2<T> pickRandomPoint(Random::RandomEngineBase& randomEngine, typename Shape2<T>::RandomPointPickerPreprocessedData& preprocessedData) const; //preprocessed data is of base type. All shapes have to cast it to use it.
    virtual std::vector<Vec2<T>> pickRandomPoints(size_t quantity, Random::RandomEngineBase& randomEngine) const;
    virtual std::vector<Vec2<T>> pickRandomPoints(size_t quantity, Random::RandomEngineBase& randomEngine, typename Shape2<T>::RandomPointPickerPreprocessedData& preprocessedData) const; //preprocessed data is of base type. All shapes have to cast it to use it.
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

    static Rectangle<T> unitRectangle();
};

typedef Rectangle<double> RectangleD;
typedef Rectangle<float> RectangleF;
typedef Rectangle<int> RectangleI;

#include "../src/Rectangle.cpp"
#endif // RECTANGLE_H_INCLUDED
