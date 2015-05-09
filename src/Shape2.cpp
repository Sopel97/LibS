template <class T>
Polyline<T> Shape2<T>::outline() const
{
    return Polyline<T>();
}

template <class T>
Vec2<T> Shape2<T>::project(const Vec2<T>& b) const
{
    //not specified
    return Vec2<T>();
}

template <class T>
std::pair<T, T> Shape2<T>::projectMinMax(const Vec2<T>& b) const
{
    //not specified
    return std::make_pair(T(), T());
}
template <class T>
std::unique_ptr<typename Shape2<T>::RandomPointPickerPreprocessedData> Shape2<T>::createPreprocessedDataForRandomPointPicker() const
{
    return std::make_unique<RandomPointPickerPreprocessedData>();
}
template <class T>
Vec2<T> Shape2<T>::pickRandomPoint(Random::RandomEngineBase& randomEngine) const
{
    return Vec2<T> {0.0, 0.0};
}
template <class T>
std::vector<Vec2<T>> Shape2<T>::pickRandomPoints(size_t quantity, Random::RandomEngineBase& randomEngine) const
{
    return std::vector<Vec2<T>>(quantity);
}
template <class T>
Vec2<T> Shape2<T>::pickRandomPoint(Random::RandomEngineBase& randomEngine, typename Shape2<T>::RandomPointPickerPreprocessedData& preprocessedData) const
{
    return pickRandomPoint(randomEngine);
}
template <class T>
std::vector<Vec2<T>> Shape2<T>::pickRandomPoints(size_t quantity, Random::RandomEngineBase& randomEngine, typename Shape2<T>::RandomPointPickerPreprocessedData& preprocessedData) const
{
    return pickRandomPoints(quantity, randomEngine);
}

template <class T>
Vec2<T> Shape2<T>::center() const
{
    return Vec2<T> {0.0, 0.0};
}
