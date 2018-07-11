#pragma once

#include "CommonDetail.h"

#include "LibS/Shapes/Vec2.h"

#include <vector>

namespace ls
{
    template <typename CurveT, typename IntegratorT>
    struct EquidistantBezierCurveParams : IntegratorT
    {
        using ValueType = typename CurveT::ValueType;
        using VectorType = ls::Vec2<ValueType>;

        // using detail::bezierCurveArgEps<ValueType> for default eps crashes VS compiler
        EquidistantBezierCurveParams(const CurveT& curve, int numPoints, const ValueType& tMin, const ValueType& tMax, int iterMax, IntegratorT integrator, const ValueType& eps = static_cast<ValueType>(0.0001)) :
            IntegratorT(std::move(integrator)),
            m_curve(curve),
            m_tMin(tMin),
            m_tMax(tMax),
            m_eps(eps),
            m_iterMax(iterMax),
            m_numPoints(numPoints)
        {
            m_params.reserve(numPoints);
            updateParams();
        }

        const ValueType& operator[](int i) const
        {
            return m_params[i];
        }

        auto begin() const
        {
            return m_params.cbegin();
        }

        auto end() const
        {
            return m_params.cend();
        }

        auto cbegin() const
        {
            return m_params.cbegin();
        }

        auto cend() const
        {
            return m_params.cend();
        }

    private:
        CurveT m_curve;
        ValueType m_tMin;
        ValueType m_tMax;
        ValueType m_eps;
        int m_iterMax;
        int m_numPoints;
        std::vector<ValueType> m_params;

        // since the length can be nicely approximated by curve parameter
        // it uses false position method.
        // First creates (m_numPoints - 1) segments in which the method is applied.
        // This is done to greatly reduce the search range without increasing the
        // complexity (it remains O(m_numPoints * m_iterMax).
        // Curve length function being strictly increasing
        // also allows faster search for segment with a known solution.
        void updateParams()
        {
            using std::abs;

            const auto curveDerivative = m_curve.derivative();

            const ValueType segmentParamDiff = (m_tMax - m_tMin) / static_cast<ValueType>(m_numPoints - 1);

            std::vector<ValueType> lengths;
            {
                lengths.reserve(m_numPoints);
                lengths.emplace_back(static_cast<ValueType>(0));
                ValueType currentT = m_tMin + segmentParamDiff;
                for (int i = 1; i < m_numPoints; ++i)
                {
                    lengths.emplace_back(curveDerivative.antiderivativeLength(m_tMin, currentT, static_cast<const IntegratorT&>(*this)));
                    currentT += segmentParamDiff;
                }
            }

            const ValueType totalLength = lengths.back();
            const ValueType segmentLength = totalLength / static_cast<ValueType>(m_numPoints - 1);

            {
                ValueType currentT = m_tMin;
                for (int i = 0; i < m_numPoints; ++i)
                {
                    m_params.emplace_back(currentT);
                    currentT += segmentParamDiff;
                }
            }

            ValueType wantedLength = segmentLength;
            int rightSampleNo = 1;
            for (int i = 1; i < m_numPoints - 1; ++i)
            {
                while (lengths[rightSampleNo] < wantedLength)
                {
                    ++rightSampleNo;
                }
                const int leftSampleNo = rightSampleNo - 1;

                ValueType tLeft = m_params[leftSampleNo];
                ValueType tRight = m_params[rightSampleNo];
                ValueType sLeft = lengths[leftSampleNo];
                ValueType sRight = lengths[rightSampleNo];

                ValueType tCurrent = tLeft + (tRight - tLeft) * ((wantedLength - sLeft) / (sRight - sLeft)); // linearly interpolate t by length
                ValueType sCurrent = curveDerivative.antiderivativeLength(m_tMin, tCurrent, static_cast<const IntegratorT&>(*this));

                for (int j = 0; j < m_iterMax && abs(wantedLength - sCurrent) > m_eps; ++j)
                {
                    if (sCurrent < wantedLength)
                    {
                        //left
                        tLeft = std::move(tCurrent);
                        sLeft = std::move(sCurrent);
                    }
                    else
                    {
                        //right
                        tRight = std::move(tCurrent);
                        sRight = std::move(sCurrent);
                    }

                    tCurrent = tLeft + (tRight - tLeft) * ((wantedLength - sLeft) / (sRight - sLeft)); // linearly interpolate t by length
                    sCurrent = curveDerivative.antiderivativeLength(m_tMin, tCurrent, static_cast<const IntegratorT&>(*this));
                }

                m_params[i] = tCurrent;
                lengths[i] = sCurrent;
                wantedLength += segmentLength;
            }
        }
    };
}
