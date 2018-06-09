#pragma once

#include "LibS/Shapes/Angle2.h"
#include "LibS/Shapes/Vec3.h"

namespace ls
{
    template <typename T>
    struct AxisAngle
    {
    public:
        constexpr AxisAngle() noexcept = default;
        constexpr AxisAngle(const Vec3<T>& axis, const Angle2<T>& angle) :
            m_axis(axis.normalized()),
            m_angle(angle)
        {

        }
        constexpr AxisAngle(const AxisAngle<T>&) = default;
        constexpr AxisAngle(AxisAngle<T>&&) noexcept = default;
        constexpr AxisAngle<T>& operator=(const AxisAngle<T>&) = default;
        constexpr AxisAngle<T>& operator=(AxisAngle<T>&&) noexcept = default;

        constexpr const Vec3<T>& axis() const
        {
            return m_axis;
        }

        constexpr const Angle2<T>& angle() const
        {
            return m_angle;
        }

        constexpr Angle2<T>& angle()
        {
            return m_angle;
        }

        void setAxis(const Vec3<T>& newAxis)
        {
            m_axis = newAxis.normalized();
        }

    private:
        Vec3<T> m_axis;
        Angle2<T> m_angle;
    };

    using AxisAngleD = AxisAngle<double>;
    using AxisAngleF = AxisAngle<float>;
}
