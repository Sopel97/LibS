template <typename IntType>
RandomEngineT<IntType>::RandomEngineT()
{

}
template <typename IntType>
RandomEngineT<IntType>::~RandomEngineT()
{

}
template <typename IntType>
RandomEngineT<IntType>::RandomEngineT(IntType seed, IntType max) :
    m_seed(seed),
    m_max(max)
{

}
template <typename IntType>
IntType RandomEngineT<IntType>::next(IntType rangeFirst, IntType rangeLast) // <rangeFirst, rangeLast>
{
    return nextRaw() % (rangeLast - rangeFirst + IntType(1)) + rangeFirst;
}
template <typename IntType>
int32_t RandomEngineT<IntType>::nextInt32(int32_t rangeFirst, int32_t rangeLast)
{
    return next(rangeFirst, rangeLast);
}
template <typename IntType>
uint32_t RandomEngineT<IntType>::nextUint32(uint32_t rangeFirst, uint32_t rangeLast)
{
    return next(rangeFirst, rangeLast);
}
/* TODO: find a way to make it (currently there is a problem that this is seen as not a template and make a multiple definition error) [same with the ones lower]
template <>
int64_t RandomEngine32::nextInt64(int64_t rangeFirst, int64_t rangeLast)
{
    int32_t rf1 = rangeFirst << 32ll;
    int32_t rl1 = rangeLast << 32ll;
    int32_t rf2 = rangeFirst;
    int32_t rl2 = rangeLast;
    return (static_cast<int64_t>(next(rf1, rl1)) << 32ll) + next(rf2, rl2);
}
template <>
int64_t RandomEngine64::nextInt64(int64_t rangeFirst, int64_t rangeLast)
{
    return next(rangeFirst, rangeLast);
}
template <>
uint64_t RandomEngine32::nextUint64(uint64_t rangeFirst, uint64_t rangeLast)
{
    uint32_t rf1 = rangeFirst << 32ull;
    uint32_t rl1 = rangeLast << 32ull;
    uint32_t rf2 = rangeFirst;
    uint32_t rl2 = rangeLast;
    return (static_cast<uint64_t>(next(rf1, rl1)) << 32ull) + next(rf2, rl2);
}
template <>
uint64_t RandomEngine64::nextUint64(uint64_t rangeFirst, uint64_t rangeLast)
{
    return next(rangeFirst, rangeLast);
}
*/
template <typename IntType>
uint64_t RandomEngineT<IntType>::nextUint64(uint64_t rangeFirst, uint64_t rangeLast)
{
    return next(rangeFirst, rangeLast);
}
template <typename IntType>
int64_t RandomEngineT<IntType>::nextInt64(int64_t rangeFirst, int64_t rangeLast)
{
    return next(rangeFirst, rangeLast);
}
template <typename IntType>
float RandomEngineT<IntType>::nextFloat(float rangeFirst, float rangeLast) // <rangeFirst, rangeLast) (if m_max can't fit in int type used for nextRaw() then it's <rangeFirst, rangeLast>
{
    return nextRaw() / static_cast<float>(m_max) * (rangeLast - rangeFirst) + rangeFirst;
}
template <typename IntType>
double RandomEngineT<IntType>::nextDouble(double rangeFirst, double rangeLast) // <rangeFirst, rangeLast) (if m_max can't fit in int type used for nextRaw() then it's <rangeFirst, rangeLast>
{
    return nextRaw() / static_cast<double>(m_max) * (rangeLast - rangeFirst) + rangeFirst;
}
template <typename IntType>
long double RandomEngineT<IntType>::nextLongDouble(double rangeFirst, double rangeLast)
{
    return nextRaw() / static_cast<long double>(m_max) * (rangeLast - rangeFirst) + rangeFirst;
}

template <typename IntType>
int32_t RandomEngineT<IntType>::nextInt32()
{
    return nextRaw();
}
template <typename IntType>
uint32_t RandomEngineT<IntType>::nextUint32()
{
    return nextRaw();
}
/*
template <>
int64_t RandomEngine32::nextInt64()
{
    return (static_cast<int64_t>(nextRaw()) << 32ll) + nextRaw();
}
template <>
int64_t RandomEngine64::nextInt64()
{
    return nextRaw();
}
template <>
uint64_t RandomEngine32::nextUint64()
{
    return (static_cast<uint64_t>(nextRaw()) << 32ull) + nextRaw();
}
template <>
uint64_t RandomEngine64::nextUint64()
{
    return nextRaw();
}
*/
template <typename IntType>
int64_t RandomEngineT<IntType>::nextInt64()
{
    return nextRaw();
}
template <typename IntType>
uint64_t RandomEngineT<IntType>::nextUint64()
{
    return nextRaw();
}
template <typename IntType>
IntType RandomEngineT<IntType>::next() //range defined by IntType size and generator
{
    return nextRaw();
}
template <typename IntType>
float RandomEngineT<IntType>::nextFloat() // <0, 1) (if m_max can't fit in int type used for nextRaw() then it's <0, 1>
{
    return nextRaw() / static_cast<float>(m_max);
}
template <typename IntType>
double RandomEngineT<IntType>::nextDouble() // <0, 1) (if m_max can't fit in int type used for nextRaw() then it's <0, 1>
{
    return nextRaw() / static_cast<double>(m_max);
}
template <typename IntType>
long double RandomEngineT<IntType>::nextLongDouble() // <0, 1) (if m_max can't fit in int type used for nextRaw() then it's <0, 1>
{
    return nextRaw() / static_cast<long double>(m_max);
}
template <typename IntType>
bool RandomEngineT<IntType>::nextBool() //0 or 1
{
    return nextRaw() > (m_max >> 1);
}

template <typename IntType>
IntType RandomEngineT<IntType>::seed() const
{
    return m_seed;
}
template <typename IntType>
IntType RandomEngineT<IntType>::max() const //max+1 or max if max+1 dont fir in IntType
{
    return m_max;
}
