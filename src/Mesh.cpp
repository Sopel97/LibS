template <class T>
Mesh<T>::Mesh()
{

}
template <class T>
Mesh<T>::Mesh(const std::vector<T>& e) : elements(e)
{
}
template <class T>
Mesh<T>::Mesh(std::vector<T>&& e) : elements(std::move(e))
{
}
template <class T>
Mesh<T>::Mesh(const std::initializer_list<T>& list) : elements(list)
{
}
template <class T>
Mesh<T>::Mesh(T* e, size_t count)
{
    elements.assign(e, e + count);
}
template <class T>
Mesh<T>::Mesh(const Mesh<T>& m) : elements(m.elements)
{
}
template <class T>
Mesh<T>::Mesh(Mesh<T>&& m) : elements(std::move(m.elements))
{
}

template <class T>
Mesh<T>& Mesh<T>::operator=(const Mesh<T>& m)
{
    elements = m.elements;
    return *this;
}
template <class T>
Mesh<T>& Mesh<T>::operator=(Mesh<T> && m)
{
    elements = std::move(m.elements);
    return *this;
}

template <class T>
template <class VectorType>
Mesh<T> Mesh<T>::operator+(const VectorType& v) const
{
    Mesh<T> m(*this);
    m += v;
    return m;
}
template <class T>
template <class VectorType>
Mesh<T>& Mesh<T>::operator+=(const VectorType& v)
{
    int size = elements.size();
    for(T& element : elements)
    {
        element += v;
    }
    return *this;
}
template <class T>
template <class VectorType>
Mesh<T> Mesh<T>::operator-(const VectorType& v) const
{
    Mesh<T> m(*this);
    m -= v;
    return m;
}
template <class T>
template <class VectorType>
Mesh<T>& Mesh<T>::operator-=(const VectorType& v)
{
    int size = elements.size();
    for(T& element : elements)
    {
        element -= v;
    }
    return *this;
}

template <class T>
void Mesh<T>::add(const T& e)
{
    elements.push_back(e);
}
template <class T>
void Mesh<T>::add(const std::vector<T>& e)
{
    elements.insert(elements.end(), e.begin(), e.end());
}
template <class T>
void Mesh<T>::add(const Mesh<T>& m)
{
    add(m.elements);
}
template <class T>
void Mesh<T>::add(const std::initializer_list<T>& list)
{
    elements.insert(elements.end(), list.begin, list.end());
}

template <class T>
void Mesh<T>::add(T&& e)
{
    elements.push_back(e);
}
template <class T>
void Mesh<T>::add(std::vector<T>&& e)
{
    elements.insert(elements.end(), e.begin(), e.end());
}
template <class T>
void Mesh<T>::add(Mesh<T>&& m)
{
    add(std::move(m.elements));
}

template <class T>
void Mesh<T>::translate(const Vec2<T>& v)
{

}
template <class T>
void Mesh<T>::scale(const Vec2<T>& c, const Vec2<T>& s)
{

}
template <class T>
void Mesh<T>::scale(const Vec2<T>& c, const T s)
{

}
template <class T>
void Mesh<T>::scale(const Vec2<T>& s)
{

}
template <class T>
template <class Transformation>
void Mesh<T>::transform(Transformation&& func)
{
    func(*this);
}
template <class T>
void Mesh<T>::scale(const T s)
{

}

template <class T>
Polyline<T> Mesh<T>::asPolyline() const //TODO: outline using convex hull
{
    Polyline<T> outline;
    return outline;
}


template <class T>
size_t Mesh<T>::size() const
{
    return elements.size();
}

template <class T>
template <class S>
bool Mesh<T>::intersects(const S& shape) const
{
    return Intersections::intersection(*this, shape);
}

