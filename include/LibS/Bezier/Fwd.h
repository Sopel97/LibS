#pragma once

namespace ls
{
    template <typename T, int OrderV>
    struct BezierCurve2;

    template <int OrderV>
    using BezierCurve2F = BezierCurve2<float, OrderV>;

    template <int OrderV>
    using BezierCurve2D = BezierCurve2<double, OrderV>;

    template <typename T>
    using CubicBezierCurve2 = BezierCurve2<T, 3>;

    using CubicBezierCurve2F = CubicBezierCurve2<float>;
    using CubicBezierCurve2D = CubicBezierCurve2<double>;

    template <typename T>
    using QuadraticBezierCurve2 = BezierCurve2<T, 2>;

    using QuadraticBezierCurve2F = QuadraticBezierCurve2<float>;
    using QuadraticBezierCurve2D = QuadraticBezierCurve2<double>;

    template <typename T>
    using LinearBezierCurve2 = BezierCurve2<T, 1>;

    using LinearBezierCurve2F = LinearBezierCurve2<float>;
    using LinearBezierCurve2D = LinearBezierCurve2<double>;
}
