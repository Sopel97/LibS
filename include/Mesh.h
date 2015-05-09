#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED


//TODO: may need a rewrite to separate 2d case from 3d case
template <class T>
class Mesh : public Shape<T>
{
public:
    std::vector<T> elements;
    Mesh();
    Mesh(const std::vector<T>& e);
    Mesh(std::vector<T>&& e);
    Mesh(const std::initializer_list<T>& list);
    Mesh(T* e, size_t count);
    Mesh(const Mesh<T>& m);
    Mesh(Mesh<T>&& m);

    Mesh<T>& operator=(const Mesh<T>& m);
    Mesh<T>& operator=(Mesh<T> && m);

    template <class VectorType>
    Mesh<T> operator+(const VectorType& v) const;
    template <class VectorType>
    Mesh<T>& operator+=(const VectorType& v);
    template <class VectorType>
    Mesh<T> operator-(const VectorType& v) const;
    template <class VectorType>
    Mesh<T>& operator-=(const VectorType& v);

    void add(const T& e);
    void add(const std::vector<T>& e);
    void add(const Mesh<T>& m);
    void add(const std::initializer_list<T>& list);

    void add(T&& e);
    void add(std::vector<T>&& e);
    void add(Mesh<T>&& m);

    void translate(const Vec2<T>& v);
    void scale(const Vec2<T>& c, const Vec2<T>& s);
    void scale(const Vec2<T>& c, const T s);
    void scale(const Vec2<T>& s);
    void scale(const T s);

    template <class Transformation>
    void transform(Transformation&& func);

    Polyline<T> outline() const;

    size_t size() const;

    template <class S>
    bool intersects(const S& shape) const;
};

#include "../src/Mesh.cpp"
#endif // MESH_H_INCLUDED
