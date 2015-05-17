template <class T>
Polygon<T>::Polygon(const std::initializer_list<Vec2<T>>& list) : vertices(list)
{
}
template <class T>
Polygon<T>::Polygon(const std::vector<Vec2<T>>& v) : vertices(v)
{
}
template <class T>
Polygon<T>::Polygon(std::vector<Vec2<T>>&& v) : vertices(std::move(v))
{
}
template <class T>
Polygon<T>::Polygon(Vec2<T>* v, size_t count)
{
    vertices.assign(v, v + count);
}
template <class T>
template <class X>
Polygon<T>::Polygon(const Polygon<X>& p) : vertices(p.vertives)
{
}
template <class T>
template <class X>
Polygon<T>& Polygon<T>::operator=(const Polygon<X>& p)
{
    vertices = p.vertices;
    return *this;
}

template <class T>
Polygon<T> Polygon<T>::operator+(const Vec2<T>& v) const
{
    Polygon newPolygon(*this);
    newPolygon += v;
    return newPolygon;
}
template <class T>
Polygon<T>& Polygon<T>::operator+=(const Vec2<T>& v)
{
    for(Vec2<T>& vertex : vertices)
    {
        vertex += v;
    }
    return *this;
}
template <class T>
Polygon<T> Polygon<T>::operator-(const Vec2<T>& v) const
{
    Polygon newPolygon(*this);
    newPolygon -= v;
    return newPolygon;
}
template <class T>
Polygon<T>& Polygon<T>::operator-=(const Vec2<T>& v)
{
    for(Vec2<T>& vertex : vertices)
    {
        vertex -= v;
    }
    return *this;
}

template <class T>
void Polygon<T>::add(const Vec2<T>& v)
{
    vertices.push_back(v);
}
template <class T>
void Polygon<T>::add(const std::vector<T>& v)
{
    vertices.insert(vertices.end(), v.begin(), v.end());
}
template <class T>
void Polygon<T>::add(const std::initializer_list<Vec2<T>>& list)
{
    vertices.insert(vertices.end(), list.begin(), list.end());
}

template <class T>
void Polygon<T>::add(Vec2<T>&& v)
{
    vertices.push_back(v);
}
template <class T>
void Polygon<T>::add(std::vector<T>&& v)
{
    vertices.insert(vertices.end(), v.begin(), v.end());
}

template <class T>
void Polygon<T>::translate(const Vec2<T>& v)
{
    for(auto& vert : vertices)
    {
        vert.translate(v);
    }
}
template <class T>
void Polygon<T>::scale(const Vec2<T>& s)
{
    for(auto& vert : vertices)
    {
        vert.scale(s);
    }
}
template <class T>
void Polygon<T>::transform(const std::function<void(Vec2<T>&)>& transformationFunction)
{
    for(auto& v : vertices)
    {
        transformationFunction(v);
    }
}
template <class T>
void Polygon<T>::transform(const Transformation2<T>& transformation)
{
    for(auto& v : vertices)
    {
        transformation.transform(v);
    }
}
template <class T>
Polygon<T> Polygon<T>::transformed(const std::function<void(Vec2<T>&)>& transformationFunction) const
{
    Polygon<T> copy(*this);
    copy.transform(transformationFunction);
    return copy;
}
template <class T>
Polygon<T> Polygon<T>::transformed(const Transformation2<T>& transformation) const
{
    Polygon<T> copy(*this);
    copy.transform(transformation);
    return copy;
}

template <class T>
Vec2<T> Polygon<T>::project(const Vec2<T>& b) const
{
    Vec2<T> projection;

    return projection;
}

template <class T>
std::pair<T, T> Polygon<T>::projectMinMax(const Vec2<T>& b) const
{
    T minProjection;
    T maxProjection;
    minProjection = maxProjection = vertices[0].dot(b);
    for(const Vec2<T>& vertex : vertices)
    {
        T dotProduct = vertex.dot(b);
        if(dotProduct < minProjection) minProjection = dotProduct;
        else if(dotProduct > maxProjection) maxProjection = dotProduct;
    }
    return std::pair<T, T>(minProjection, maxProjection);
}
template <class T>
T Polygon<T>::distanceTo(const Vec2<T>& point) const
{
    return point.distanceTo(nearestPointTo(point));
}
template <class T>
Vec2<T> Polygon<T>::nearestPointTo(const Vec2<T>& point) const
{
    if(point.intersects(*this)) return point;

    T minDistance = std::numeric_limits<T>::max();
    Vec2<T> nearestPoint(std::numeric_limits<T>::max(), std::numeric_limits<T>::max());
    size_t polySize = size();
    for(size_t i = 0; i < polySize; ++i)
    {
        const Vec2<T>& thisVertex = vertices[i];
        const Vec2<T>& nextVertex = vertices[(i + 1) % polySize];

        Vec2<T> nearPoint = LineSegment<T>(thisVertex, nextVertex).nearestPointTo(point);
        T distance = nearPoint.distanceTo(point);
        if(distance < minDistance)
        {
            minDistance = distance;
            nearestPoint = nearPoint;
        }
    }
    return nearestPoint;
}
template <class T>
std::unique_ptr<typename Shape2<T>::RandomPointPickerPreprocessedData> Polygon<T>::createPreprocessedDataForRandomPointPicker() const
{
    PolygonTriangulation<T> triangulation(*this);
    triangulation.calculate();
    return std::make_unique<RandomPointPickerPreprocessedData>(std::move(triangulation));
}
template <class T>
Vec2<T> Polygon<T>::pickRandomPoint(Random::RandomEngineBase& randomEngine) const
{
    return pickRandomPoint(randomEngine, *createPreprocessedDataForRandomPointPicker());
}
template <class T>
Vec2<T> Polygon<T>::pickRandomPoint(Random::RandomEngineBase& randomEngine, typename Shape2<T>::RandomPointPickerPreprocessedData& preprocessedData) const
{
    Polygon<T>::RandomPointPickerPreprocessedData& polygonPreprocessedData = static_cast<Polygon<T>::RandomPointPickerPreprocessedData&>(preprocessedData);
    T sumOfAreas = polygonPreprocessedData.trianglesByArea.back().second;
    T randomArea = randomEngine.nextDouble(0.0, sumOfAreas);
    auto chosenTriangleIter = std::upper_bound(polygonPreprocessedData.trianglesByArea.begin(), polygonPreprocessedData.trianglesByArea.end(), randomArea, [](const T& lhs, const std::pair<const Triangle<T>*, T>& rhs)->bool{return lhs < rhs.second;});
    if(chosenTriangleIter == polygonPreprocessedData.trianglesByArea.end()) chosenTriangleIter = polygonPreprocessedData.trianglesByArea.begin();
    return chosenTriangleIter->first->pickRandomPoint(randomEngine);
}
template <class T>
Polyline<T> Polygon<T>::asPolyline() const
{
    Polyline<T> out(vertices);
    out.add(out.vertices[0]);
    return out;
}
template <class T>
Vec2<T> Polygon<T>::center() const
{
    double xsum = 0.0;
    double ysum = 0.0;
    double area = 0.0;
    for(size_t i = 0; i < size(); ++i)
    {
        const Vec2D& p0 = vertices[i];
        const Vec2D& p1 = vertices[(i + 1) % size()];

        double areaSum = (p0.x * p1.y) - (p1.x * p0.y);

        xsum += (p0.x + p1.x) * areaSum;
        ysum += (p0.y + p1.y) * areaSum;

        area += areaSum;
    }

    double centMassX = xsum / (area * 3.0);
    double centMassY = ysum / (area * 3.0);

    return Vec2D {centMassX, centMassY};
}
template <class T>
bool Polygon<T>::isConvex() const
{
    size_t numberOfVertices = vertices.size();
    T prevCross = 0.0; //will be initialized by correct cross when i equals 0 in the loop
    for(size_t i = 0; i < numberOfVertices; ++i)
    {
        const Vec2<T>& v0 = vertices[i];
        const Vec2<T>& v1 = vertices[(i+1)&numberOfVertices];
        const Vec2<T>& v2 = vertices[(i+2)&numberOfVertices];

        Vec2<T> e0 = v1-v0;
        Vec2<T> e1 = v2-v1;

        T cross = e0.cross(e1);
        if(i != 0)
        {
            if(cross * prevCross < 0.0) return false; //if cross and prev cross have different signs
        }
        prevCross = cross;
    }
    return true;
}
template <class T>
T Polygon<T>::signedArea() const
{
    double area = 0.0;
    for(size_t i = 0; i < size(); ++i)
    {
        const Vec2D& p0 = vertices[i];
        const Vec2D& p1 = vertices[(i + 1) % size()];

        area += (p0.x * p1.y) - (p1.x * p0.y);
    }
    return area / 2.0;
}

template <class T>
size_t Polygon<T>::size() const
{
    return vertices.size();
}

template <class T>
Polygon<T> Polygon<T>::regular(const Vec2D& center, int sides, int radius)
{
    Polygon<T> polygon;
    polygon.vertices.reserve(sides);
    for(int i = 0; i < sides; ++i)
    {
        polygon.vertices.emplace_back(radius * std::cos(2.0 * PI * i / sides), radius * std::sin(2.0 * PI * i / sides));
    }
    return polygon;
}
template <class T>
Polygon<T> Polygon<T>::fromRectangle(const Rectangle<T>& rectangle)
{
    return Polygon<T> {Vec2<T>{rectangle.min.x, rectangle.min.y},
                       Vec2<T>{rectangle.max.x, rectangle.min.y},
                       Vec2<T>{rectangle.max.x, rectangle.max.y},
                       Vec2<T>{rectangle.min.x, rectangle.max.y}
                      };
}
template <class T>
Polygon<T> Polygon<T>::fromTriangle(const Triangle<T>& triangle)
{
    return Polygon<T> {triangle.vertices[0], triangle.vertices[1], triangle.vertices[2]};
}
template <class T>
std::unique_ptr<Shape2<T>> Polygon<T>::clone() const
{
    return std::make_unique<Polygon<T>>(*this);
}
