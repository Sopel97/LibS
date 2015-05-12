#ifndef POLYLINE_H
#define POLYLINE_H

template <class T>
class Polyline : public Shape2<T>
{
public:
    struct RandomPointPickerPreprocessedData : public Shape2<T>::RandomPointPickerPreprocessedData
    {
        //currently empty but will be used later
    };
    std::vector<Vec2<T>> vertices;

    Polyline() = default;
    Polyline(const std::initializer_list<Vec2<T>>& list);
    Polyline(const std::vector<Vec2<T>>& v);
    Polyline(std::vector<Vec2<T>>&& v);
    Polyline(Vec2<T>* v, size_t count);
    Polyline(const Polyline<T>& p) = default;
    Polyline(Polyline<T>&& p) = default;

    Polyline<T>& operator=(const Polyline<T>& p) = default;
    Polyline<T>& operator=(Polyline<T> && p) = default;

    template <class X>
    Polyline(const Polyline<X>& p);
    template <class X>
    Polyline<T>& operator=(const Polyline<X>& p);

    Polyline<T> operator+(const Vec2<T>& v) const;
    Polyline<T>& operator+=(const Vec2<T>& v);
    Polyline<T> operator-(const Vec2<T>& v) const;
    Polyline<T>& operator-=(const Vec2<T>& v);

    void add(const Vec2<T>& v);
    void add(const std::vector<T>& v);
    void add(const std::initializer_list<Vec2<T>>& list);

    void add(Vec2<T>&& v);
    void add(std::vector<T>&& v);

    virtual void translate(const Vec2<T>& v);
    virtual void scale(const Vec2<T>& s);

    Vec2<T> project(const Vec2<T>& b) const;
    std::pair<T, T> projectMinMax(const Vec2<T>& b) const;

    virtual void transform(const std::function<void(Vec2<T>&)>& transformationFunction);
    virtual void transform(const Transformation2<T>& transformation);
    Polyline<T> transformed(const std::function<void(Vec2<T>&)>& transformationFunction) const;
    Polyline<T> transformed(const Transformation2<T>& transformation) const;

    virtual std::unique_ptr<typename Shape2<T>::RandomPointPickerPreprocessedData> createPreprocessedDataForRandomPointPicker() const;
    Polyline<T> asPolyline() const;

    size_t size() const;

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

    static Polyline<T> fromRectangle(const Rectangle<T>& rectangle);
    static Polyline<T> fromTriangle(const Triangle<T>& triangle);

    virtual std::unique_ptr<Shape2<T>> clone() const;

    virtual ~Polyline(){}
};

typedef Polyline<double> PolylineD;
typedef Polyline<float> PolylineF;
typedef Polyline<int> PolylineI;

#include "../src/Polyline.cpp"

#endif // POLYLINE_H
