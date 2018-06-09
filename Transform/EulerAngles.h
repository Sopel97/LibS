#pragma once

#include "LibS/Shapes/Angle2.h"

namespace ls
{
    template <typename T>
    struct EulerAngles
    {
    public:
        Angle2<T> pitch;
        Angle2<T> yaw;
        Angle2<T> roll;

        constexpr EulerAngles() noexcept = default;
        constexpr EulerAngles(const Angle2<T>& pitch, const Angle2<T>& yaw, const Angle2<T>& roll) :
            pitch(pitch),
            yaw(yaw),
            roll(roll)
        {

        }
        constexpr EulerAngles(const EulerAngles<T>&) = default;
        constexpr EulerAngles(EulerAngles<T>&&) noexcept = default;
        constexpr EulerAngles<T>& operator=(const EulerAngles<T>&) = default;
        constexpr EulerAngles<T>& operator=(EulerAngles<T>&&) noexcept = default;
    };

    using EulerAnglesD = EulerAngles<double>;
    using EulerAnglesF = EulerAngles<float>;
}
