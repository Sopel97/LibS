#ifndef POLYGON_H_INCLUDED
#define POLYGON_H_INCLUDED

template <class T>
class Polygon : public Shape2<T> //Supports convex and concave polygons. Does not support self intersecting (complex) polygons (some methods may work, some may not).
{
public:
    struct RandomPointPickerPreprocessedData : public Shape2<T>::RandomPointPickerPreprocessedData
    {
        //currently empty but will be used later
    };
    std::vector<Vec2<T>> vertices;

    Polygon() = default;
    Polygon(const std::initializer_list<Vec2<T>>& list);
    Polygon(const std::vector<Vec2<T>>& v);
    Polygon(std::vector<Vec2<T>>&& v);
    Polygon(Vec2<T>* v, size_t count);
    Polygon(const Polygon<T>& p) = default;
    Polygon(Polygon<T>&& p) = default;

    Polygon<T>& operator=(const Polygon<T>& p) = default;
    Polygon<T>& operator=(Polygon<T> && p) = default;

    template <class X>
    Polygon(const Polygon<X>& p);
    template <class X>
    Polygon<T>& operator=(const Polygon<X>& p);

    Polygon<T> operator+(const Vec2<T>& v) const;
    Polygon<T>& operator+=(const Vec2<T>& v);
    Polygon<T> operator-(const Vec2<T>& v) const;
    Polygon<T>& operator-=(const Vec2<T>& v);

    void add(const Vec2<T>& v);
    void add(const std::vector<T>& v);
    void add(const std::initializer_list<Vec2<T>>& list);

    void add(Vec2<T>&& v);
    void add(std::vector<T>&& v);

    void translate(const Vec2<T>& v);
    void scale(const Vec2<T>& c, const Vec2<T>& s);
    void scale(const Vec2<T>& c, const T s);
    void scale(const Vec2<T>& s);
    void scale(const T s);

    Vec2<T> project(const Vec2<T>& b) const;
    std::pair<T, T> projectMinMax(const Vec2<T>& b) const;

    virtual void transform(const std::function<void(Vec2<T>&)>& transformationFunction);
    virtual void transform(const Transformation2<T>& transformation);
    Polygon<T> transformed(const std::function<void(Vec2<T>&)>& transformationFunction) const;
    Polygon<T> transformed(const Transformation2<T>& transformation) const;

    virtual std::unique_ptr<typename Shape2<T>::RandomPointPickerPreprocessedData> createPreprocessedDataForRandomPointPicker() const;
    Polyline<T> asPolyline() const;
    virtual Vec2<T> center() const;

    //bool isConvex() const; //TODO: this
    //bool isConcave() const; //TODO: this

    size_t size() const;

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

    //static Polygon<T> randomInscribedInCircle(const Circle<T>& circle, int edges); later, when using Random.h will be somehow standarized
    static Polygon<T> regular(const Vec2D& center, int sides, int radius);
    static Polygon<T> fromRectangle(const Rectangle<T>& rectangle);
    static Polygon<T> fromTriangle(const Triangle<T>& triangle);

    virtual std::unique_ptr<Shape2<T>> clone() const;
};

typedef Polygon<double> PolygonD;
typedef Polygon<float> PolygonF;
typedef Polygon<int> PolygonI;

#include "../src/Polygon.cpp"
#endif // POLYGON_H_INCLUDED