template <class ShapeType>
Mesh2<ShapeType>::Mesh2()
{

}
template <class ShapeType>
Mesh2<ShapeType>::Mesh2(const std::vector<ShapeType>& e) : elements(e)
{
}
template <class ShapeType>
Mesh2<ShapeType>::Mesh2(std::vector<ShapeType>&& e) : elements(std::move(e))
{
}
template <class ShapeType>
Mesh2<ShapeType>::Mesh2(const std::initializer_list<ShapeType>& list) : elements(list)
{
}
template <class ShapeType>
Mesh2<ShapeType>::Mesh2(ShapeType* e, size_t count)
{
    elements.assign(e, e + count);
}
template <class ShapeType>
Mesh2<ShapeType>::Mesh2(const Mesh2<ShapeType>& m) : elements(m.elements)
{
}
template <class ShapeType>
Mesh2<ShapeType>::Mesh2(Mesh2<ShapeType>&& m) : elements(std::move(m.elements))
{
}

template <class ShapeType>
Mesh2<ShapeType>& Mesh2<ShapeType>::operator=(const Mesh2<ShapeType>& m)
{
    elements = m.elements;
    return *this;
}
template <class ShapeType>
Mesh2<ShapeType>& Mesh2<ShapeType>::operator=(Mesh2<ShapeType> && m)
{
    elements = std::move(m.elements);
    return *this;
}

template <class ShapeType>
Mesh2<ShapeType> Mesh2<ShapeType>::operator+(const Vec2<T>& v) const
{
    Mesh2<ShapeType> m(*this);
    m += v;
    return m;
}
template <class ShapeType>
Mesh2<ShapeType>& Mesh2<ShapeType>::operator+=(const Vec2<T>& v)
{
    int size = elements.size();
    for(T& element : elements)
    {
        element += v;
    }
    return *this;
}
template <class ShapeType>
Mesh2<ShapeType> Mesh2<ShapeType>::operator-(const Vec2<T>& v) const
{
    Mesh2<ShapeType> m(*this);
    m -= v;
    return m;
}
template <class ShapeType>
Mesh2<ShapeType>& Mesh2<ShapeType>::operator-=(const Vec2<T>& v)
{
    int size = elements.size();
    for(T& element : elements)
    {
        element -= v;
    }
    return *this;
}

template <class ShapeType>
void Mesh2<ShapeType>::add(const ShapeType& e)
{
    elements.push_back(e);
}
template <class ShapeType>
void Mesh2<ShapeType>::add(const std::vector<ShapeType>& e)
{
    elements.insert(elements.end(), e.begin(), e.end());
}
template <class ShapeType>
void Mesh2<ShapeType>::add(const Mesh2<ShapeType>& m)
{
    add(m.elements);
}
template <class ShapeType>
void Mesh2<ShapeType>::add(const std::initializer_list<ShapeType>& list)
{
    elements.insert(elements.end(), list.begin, list.end());
}

template <class ShapeType>
void Mesh2<ShapeType>::add(ShapeType&& e)
{
    elements.push_back(e);
}
template <class ShapeType>
void Mesh2<ShapeType>::add(std::vector<ShapeType>&& e)
{
    elements.insert(elements.end(), e.begin(), e.end());
}
template <class ShapeType>
void Mesh2<ShapeType>::add(Mesh2<ShapeType>&& m)
{
    add(std::move(m.elements));
}

template <class ShapeType>
void Mesh2<ShapeType>::translate(const Vec2<T>& v)
{
    for(auto& shape : elements)
    {
        shape.translate(v);
    }
}
template <class ShapeType>
void Mesh2<ShapeType>::scale(const Vec2<T>& s)
{
    for(auto& shape : elements)
    {
        shape.scale(s);
    }
}
template <class ShapeType>
void Mesh2<ShapeType>::transform(const std::function<void(Vec2<T>&)>& transformationFunction)
{
    for(auto& shape : elements)
    {
        shape.transform(transformationFunction);
    }
}
template <class ShapeType>
void Mesh2<ShapeType>::transform(const Transformation2<T>& transformation)
{
    for(auto& shape : elements)
    {
        shape.transform(transformation);
    }
}
template <class ShapeType>
Mesh2<typename Mesh2<ShapeType>::T> Mesh2<ShapeType>::transformed(const std::function<void(Vec2<T>&)>& transformationFunction) const
{
    Mesh2<T> copy(*this);
    copy.transform(transformationFunction);
    return copy;
}
template <class ShapeType>
Mesh2<typename Mesh2<ShapeType>::T> Mesh2<ShapeType>::transformed(const Transformation2<T>& transformation) const
{
    Mesh2<T> copy(*this);
    copy.transform(transformation);
    return copy;
}
template <class ShapeType>
Polyline<typename Mesh2<ShapeType>::T> Mesh2<ShapeType>::asPolyline() const //TODO: outline using convex hull
{
    Polyline<typename Mesh2<ShapeType>::T> outline;
    return outline;
}


template <class ShapeType>
size_t Mesh2<ShapeType>::size() const
{
    return elements.size();
}

template <class ShapeType>
bool Mesh2<ShapeType>::isConvex() const
{
    return false;
}
template <class ShapeType>
std::unique_ptr<typename Shape2<typename Mesh2<ShapeType>::T>::RandomPointPickerPreprocessedData> Mesh2<ShapeType>::createPreprocessedDataForRandomPointPicker() const
{
    return std::make_unique<RandomPointPickerPreprocessedData>(*this);
}
template <class ShapeType>
Vec2<typename Mesh2<ShapeType>::T> Mesh2<ShapeType>::pickRandomPoint(Random::RandomEngineBase& randomEngine) const
{
    return pickRandomPoint(randomEngine, *createPreprocessedDataForRandomPointPicker());
}
template <class ShapeType>
Vec2<typename Mesh2<ShapeType>::T> Mesh2<ShapeType>::pickRandomPoint(Random::RandomEngineBase& randomEngine, typename Shape2<T>::RandomPointPickerPreprocessedData& preprocessedData) const
{
    Mesh2<ShapeType>::RandomPointPickerPreprocessedData& polygonPreprocessedData = static_cast<Mesh2<ShapeType>::RandomPointPickerPreprocessedData&>(preprocessedData);
    T sumOfAreas = polygonPreprocessedData.shapesByArea.back().second;
    T randomArea = randomEngine.nextDouble(0.0, sumOfAreas);
    auto chosenShapeIter = std::upper_bound(polygonPreprocessedData.shapesByArea.begin(), polygonPreprocessedData.shapesByArea.end(), randomArea, [](const T& lhs, const std::pair<const ShapeType*, T>& rhs)->bool{return lhs < rhs.second;});
    if(chosenShapeIter == polygonPreprocessedData.shapesByArea.end()) chosenShapeIter = polygonPreprocessedData.shapesByArea.begin();
    return chosenShapeIter->first->pickRandomPoint(randomEngine);
}
//for double dispatch
template <class ShapeType>
bool Mesh2<ShapeType>::intersects(const Shape2<T>* other) const
{
    for(auto& shape : elements)
    {
        if(other->intersects(shape)) return true;
    }
    return false;
}
//specifications fo double dispatch
template <class ShapeType>
bool Mesh2<ShapeType>::intersects(const Circle<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class ShapeType>
bool Mesh2<ShapeType>::intersects(const LineSegment<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class ShapeType>
bool Mesh2<ShapeType>::intersects(const Polygon<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class ShapeType>
bool Mesh2<ShapeType>::intersects(const Polyline<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class ShapeType>
bool Mesh2<ShapeType>::intersects(const Ray<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class ShapeType>
bool Mesh2<ShapeType>::intersects(const Rectangle<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class ShapeType>
bool Mesh2<ShapeType>::intersects(const Triangle<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class ShapeType>
bool Mesh2<ShapeType>::intersects(const Vec2<T>& other) const
{
    return Intersections::intersection(other, *this);
}
template <class ShapeType>
std::unique_ptr<Shape2<typename Mesh2<ShapeType>::T>> Mesh2<ShapeType>::clone() const
{
    return std::make_unique<Mesh2<ShapeType>>(*this);
}

