#ifndef RAY_H
#define RAY_H

template <class T>
class Ray : public Shape2<T>
{
public:
    Vec2<T> origin;
    Vec2<T> direction; //must be normalized

    Ray() = default;
    Ray(const Vec2<T>& o, const Vec2<T>& d);

    Ray(const Ray<T>& r) = default;
    Ray(Ray<T>&& r) = default;

    Ray<T>& operator=(const Ray<T>& r) = default;
    Ray<T>& operator=(Ray<T> && r) = default;

    template <class X>
    Ray(const Ray<X>& r);
    template <class X>
    Ray<T>& operator=(const Ray<X>& r);

    Ray<T> operator+(const Vec2<T>& v) const;
    Ray<T>& operator+=(const Vec2<T>& v);
    Ray<T> operator-(const Vec2<T>& v) const;
    Ray<T>& operator-=(const Vec2<T>& v);

    void setDirection(const Vec2<T>& d);

    virtual void translate(const Vec2<T>& v);
    virtual void scale(const Vec2<T>& s);

    virtual void transform(const std::function<void(Vec2<T>&)>& transformationFunction);
    virtual void transform(const Transformation2<T>& transformation);
    Ray<T> transformed(const std::function<void(Vec2<T>&)>& transformationFunction) const;
    Ray<T> transformed(const Transformation2<T>& transformation) const;

    Polyline<T> asPolyline() const;

    //for double dispatch
    virtual bool intersects(const Shape2<T>* other) const;
    //specifications for double dispatch
    virtual bool intersects(const Circle<T>& other) const;
    virtual bool intersects(const LineSegment<T>& other) const;
    virtual bool intersects(const Polygon<T>& other) const;
    virtual bool intersects(const Polyline<T>& other) const;
    virtual bool intersects(const Ray<T>& other) const;
    virtual bool intersects(const Rectangle<T>& other) const;
    virtual bool intersects(const Triangle<T>& other) const;
    virtual bool intersects(const Vec2<T>& other) const;


    //specifications for double dispatch
    virtual bool contains(const Circle<T>& other) const;
    virtual bool contains(const LineSegment<T>& other) const;
    virtual bool contains(const Polygon<T>& other) const;
    virtual bool contains(const Polyline<T>& other) const;
    virtual bool contains(const Ray<T>& other) const;
    virtual bool contains(const Rectangle<T>& other) const;
    virtual bool contains(const Triangle<T>& other) const;
    virtual bool contains(const Vec2<T>& other) const;

    virtual std::unique_ptr<Shape2<T>> clone() const;

    virtual ~Ray(){}
};

typedef Ray<double> RayD;
typedef Ray<float> RayF;
typedef Ray<int> RayI;

#include "../src/Ray.cpp"

#endif // RAY_H
