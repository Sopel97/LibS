#pragma once

namespace ls
{
    template <typename T>
    struct ContinuousCollision2;

    template <typename ShapeT, typename VectorT = typename ShapeT::VectorType>
    struct Accelerating;

    template <typename ShapeT, typename VectorT = typename ShapeT::VectorType>
    struct Moving;

    template <typename T>
    struct PointNormalPair2;
}
