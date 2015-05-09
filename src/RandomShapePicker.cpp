template <class T>
RandomShapePicker<T>::RandomShapePicker(const Shape2<T>& shape) :
    m_shape(shape.clone()),
    m_randomEngine(nullptr),
    m_pointPickerPreprocessedData(shape.createPreprocessedDataForRandomPointPicker())
{

}
template <class T>
RandomShapePicker<T>::RandomShapePicker(const Shape2<T>& shape, const Random::RandomEngineBase& randomEngine) :
    m_shape(shape.clone()),
    m_randomEngine(randomEngine.clone()),
    m_pointPickerPreprocessedData(shape.createPreprocessedDataForRandomPointPicker())
{

}
template <class T>
void RandomShapePicker<T>::setRandomEngine(const Random::RandomEngineBase& randomEngine)
{
    m_randomEngine = randomEngine.clone();
}
template <class T>
void RandomShapePicker<T>::updatePreprocessedData()
{
    m_pointPickerPreprocessedData = m_shape->createPreprocessedDataForRandomPointPicker();
}

template <class T>
Vec2<T> RandomShapePicker<T>::pickRandomPoint() //m_randomEngine must be set for this
{
    return m_shape->pickRandomPoint(*m_randomEngine, *m_pointPickerPreprocessedData);
}
template <class T>
Vec2<T> RandomShapePicker<T>::pickRandomPoint(Random::RandomEngineBase& randomEngine) const
{
    return m_shape->pickRandomPoint(randomEngine, *m_pointPickerPreprocessedData);
}
template <class T>
std::vector<Vec2<T>> RandomShapePicker<T>::pickRandomPoints(size_t quantity)
{
    return m_shape->pickRandomPoint(quantity, *m_randomEngine, m_pointPickerPreprocessedData);
}
template <class T>
std::vector<Vec2<T>> RandomShapePicker<T>::pickRandomPoints(size_t quantity, Random::RandomEngineBase& randomEngine) const
{
    return m_shape->pickRandomPoint(quantity, randomEngine, m_pointPickerPreprocessedData);
}
