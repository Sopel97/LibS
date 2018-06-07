#pragma once

#include "LibS/Shapes/Fwd.h"

#include "LibS/Common.h"

namespace ls
{
    template <typename T, int DimV>
    struct NoiseSampler
    {
    public:
        using ValueType = T;
        using VectorType = VecT<ValueType, DimV>;
        using VectorTypeI = VecT<int, DimV>;
        static constexpr int dim = DimV;

    protected:
        VectorType m_scale;
        VectorTypeI m_period;

        ValueType m_persistence;
        ValueType m_lowerBound;
        ValueType m_upperBound;

        int m_octaves;

    public:
        NoiseSampler() noexcept :
            m_scale(1),
            m_persistence(1),
            m_lowerBound(-1),
            m_upperBound(1),
            m_octaves(1)
        {
            removePeriod();
        }

        template <typename NoiseGenT>
        ValueType sample(const VectorType& pos, NoiseGenT&& gen)
        {
            VectorType frequency = m_scale;
            VectorTypeI period = m_period;

            if (m_octaves == 1)
            {
                return gen.raw(pos * frequency, period);
            }

            // do more octaves if needed
            ValueType total = ValueType(0);

            ValueType amplitude = ValueType(1);
            // We have to keep track of the sum of the amplitudes,
            // because each octave adds more, and we need a value in [-1, 1].
            ValueType amplitudeSum = ValueType(0);

            for (int i = 0; i < m_octaves; ++i)
            {
                total += gen.raw(pos * frequency, period) * amplitude;

                frequency *= ValueType(2);
                doublePeriod(period);
                amplitudeSum += amplitude;
                amplitude *= m_persistence;
            }

            return total / amplitudeSum;
        }

        template <typename NoiseGenT, typename RetT = decltype(m_noiseGen.rawDerivative(declval<VectorType>(), declval<VectorTypeI>()))>
        RetT sampleDerivative(const VectorType& pos, NoiseGenT&& gen)
        {
            VectorType frequency = m_scale;
            VectorTypeI period = m_period;

            if (m_octaves == 1)
            {
                return gen.rawDerivative(pos * frequency, period);
            }

            // do more octaves if needed
            RetT total = RetT(0);

            ValueType amplitude = ValueType(1);
            // We have to keep track of the sum of the amplitudes,
            // because each octave adds more, and we need a value in [-1, 1].
            ValueType amplitudeSum = ValueType(0);

            for (int i = 0; i < m_octaves; ++i)
            {
                total += gen.rawDerivative(pos * frequency, period) * amplitude;

                frequency *= ValueType(2);
                doublePeriod(period);
                amplitudeSum += amplitude;
                amplitude *= m_persistence;
            }

            return total / amplitudeSum;
        }

        void setScale(const VectorType& newScale)
        {
            m_scale = newScale;
        }
        template <int D = DimV, typename EnableT = std::enable_if_t<D >= 2>>
        void setScaleX(ValueType newScaleX)
        {
            m_scale.x = newScaleX;
        }
        template <int D = DimV, typename EnableT = std::enable_if_t<D >= 2>>
        void setScaleY(ValueType newScaleY)
        {
            m_scale.y = newScaleY;
        }
        template <int D = DimV, typename EnableT = std::enable_if_t<D >= 3>>
        void setScaleZ(ValueType newScaleZ)
        {
            m_scale.z = newScaleZ;
        }
        template <int D = DimV, typename EnableT = std::enable_if_t<D >= 4>>
        void setScaleW(ValueType newScaleZ)
        {
            m_scale.w = newScaleW;
        }

        void setPersistence(ValueType newPersistence)
        {
            m_persistence = newPersistence;
        }
        void setLowerBound(ValueType newLowerBound)
        {
            m_lowerBound = newLowerBound;
        }
        void setUpperBound(ValueType newUpperBound)
        {
            m_upperBound = newUpperBound;
        }

        void setPeriod(const VectorTypeI& newPeriod)
        {
            m_period = newPeriod;
        }
        template <int D = DimV, typename EnableT = std::enable_if_t<D >= 1>>
        void setPeriodX(int newPeriodX)
        {
            m_period.x = newPeriodX;
        }
        template <int D = DimV, typename EnableT = std::enable_if_t<D >= 2>>
        void setPeriodY(int newPeriodY)
        {
            m_period.y = newPeriodY;
        }
        template <int D = DimV, typename EnableT = std::enable_if_t<D >= 3>>
        void setPeriodZ(int newPeriodZ)
        {
            m_period.z = newPeriodZ;
        }
        template <int D = DimV, typename EnableT = std::enable_if_t<D >= 4>>
        void setPeriodW(int newPeriodW)
        {
            m_period.w = newPeriodW;
        }

        void setOctaves(int newOctaves)
        {
            m_octaves = newOctaves;
        }

        void removePeriod()
        {
            m_period = VectorTypeI(std::numeric_limits<int>::max());
        }
        template <int D = DimV, typename EnableT = std::enable_if_t<D >= 1>>
        void removePeriodX()
        {
            m_period.x = std::numeric_limits<int>::max();
        }
        template <int D = DimV, typename EnableT = std::enable_if_t<D >= 2>>
        void removePeriodY()
        {
            m_period.y = std::numeric_limits<int>::max();
        }
        template <int D = DimV, typename EnableT = std::enable_if_t<D >= 3>>
        void removePeriodZ()
        {
            m_period.z = std::numeric_limits<int>::max();
        }
        template <int D = DimV, typename EnableT = std::enable_if_t<D >= 4>>
        void removePeriodW()
        {
            m_period.w = std::numeric_limits<int>::max();
        }

        const VectorType& scale() const
        {
            return m_scale;
        }
        template <int D = DimV, typename EnableT = std::enable_if_t<D >= 1>>
        ValueType scaleX() const
        {
            return m_scale.x;
        }
        template <int D = DimV, typename EnableT = std::enable_if_t<D >= 2>>
        ValueType scaleY() const
        {
            return m_scale.y;
        }
        template <int D = DimV, typename EnableT = std::enable_if_t<D >= 3>>
        ValueType scaleZ() const
        {
            return m_scale.z;
        }
        template <int D = DimV, typename EnableT = std::enable_if_t<D >= 4>>
        ValueType scaleW() const
        {
            return m_scale.w;
        }


        ValueType persistence() const
        {
            return m_persistence;
        }
        ValueType lowerBound() const
        {
            return m_lowerBound;
        }
        ValueType upperBound() const
        {
            return m_upperBound;
        }

        const VectorTypeI& period() const
        {
            return m_period;
        }
        template <int D = DimV, typename EnableT = std::enable_if_t<D >= 1>>
        int periodX() const
        {
            return m_period.x;
        }
        template <int D = DimV, typename EnableT = std::enable_if_t<D >= 2>>
        int periodY() const
        {
            return m_period.y;
        }
        template <int D = DimV, typename EnableT = std::enable_if_t<D >= 3>>
        int periodZ() const
        {
            return m_period.z;
        }
        template <int D = DimV, typename EnableT = std::enable_if_t<D >= 4>>
        int periodW() const
        {
            return m_period.w;
        }

        int octaves() const
        {
            return m_octaves;
        }

    private:

        static void doublePeriod(int& period)
        {
            if (period < std::numeric_limits<int>::max() / 2) period *= 2;
        }
        static void doublePeriod(Vec2<int>& period)
        {
            if (period.x < std::numeric_limits<int>::max() / 2) period.x *= 2;
            if (period.y < std::numeric_limits<int>::max() / 2) period.y *= 2;
        }
        static void doublePeriod(Vec3<int>& period)
        {
            if (period.x < std::numeric_limits<int>::max() / 2) period.x *= 2;
            if (period.y < std::numeric_limits<int>::max() / 2) period.y *= 2;
            if (period.z < std::numeric_limits<int>::max() / 2) period.z *= 2;
        }

        static void doublePeriod(Vec4<int>& period)
        {
            if (period.x < std::numeric_limits<int>::max() / 2) period.x *= 2;
            if (period.y < std::numeric_limits<int>::max() / 2) period.y *= 2;
            if (period.z < std::numeric_limits<int>::max() / 2) period.z *= 2;
            if (period.w < std::numeric_limits<int>::max() / 2) period.w *= 2;
        }
        
    };

    using NoiseSampler1D = NoiseSampler<double, 1>;
    using NoiseSampler1F = NoiseSampler<float, 1>;

    using NoiseSampler2D = NoiseSampler<double, 2>;
    using NoiseSampler2F = NoiseSampler<float, 2>;

    using NoiseSampler3D = NoiseSampler<double, 3>;
    using NoiseSampler3F = NoiseSampler<float, 3>;

    using NoiseSampler4D = NoiseSampler<double, 4>;
    using NoiseSampler4F = NoiseSampler<float, 4>;
}