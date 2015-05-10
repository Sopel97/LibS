#ifndef RandomEngineT_H_INCLUDED
#define RandomEngineT_H_INCLUDED

template <typename IntType>
class RandomEngineT : public RandomEngineBase
{
public:
    typedef IntType GeneratedValuesType;
    RandomEngineT();
    RandomEngineT(IntType seed, IntType max);
    ~RandomEngineT();
    virtual IntType nextRaw() = 0;

    virtual IntType next(IntType rangeFirst, IntType rangeLast);
    virtual int32_t nextInt32(int32_t rangeFirst, int32_t rangeLast);
    virtual uint32_t nextUint32(uint32_t rangeFirst, uint32_t rangeLast);
    virtual int64_t nextInt64(int64_t rangeFirst, int64_t rangeLast);
    virtual uint64_t nextUint64(uint64_t rangeFirst, uint64_t rangeLast);
    virtual float nextFloat(float rangeFirst, float rangeLast);
    virtual double nextDouble(double rangeFirst, double rangeLast);
    virtual long double nextLongDouble(double rangeFirst, double rangeLast);

    virtual IntType next();
    virtual int32_t nextInt32();
    virtual uint32_t nextUint32();
    virtual int64_t nextInt64();
    virtual uint64_t nextUint64();
    virtual float nextFloat();
    virtual double nextDouble();
    virtual bool nextBool();
    virtual long double nextLongDouble();

    virtual IntType seed() const;
    virtual IntType max() const;
protected:
    IntType m_seed;
    IntType m_max;
};
typedef RandomEngineT<uint32_t> RandomEngine32;
typedef RandomEngineT<uint64_t> RandomEngine64;

#include "../src/RandomEngine.cpp"

#endif // RandomEngineT_H_INCLUDED