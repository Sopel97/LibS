#pragma once
#pragma once

#include "../Shapes.h"

#include "../MathConstants.h"

#include "RandomUtil.h"

#include <random>
#include <functional>

namespace ls
{
    namespace detail
    {
        template <typename ShapeT, typename DistributionT, int DimV = DistributionT::Dim>
        struct SingleShapeParam
        {
            using distribution_type = DistributionT;
            using ShapeType = ShapeT;

            constexpr SingleShapeParam() noexcept :
                m_shape{}
            {

            }

            template <typename ShapeFwdT>
            constexpr explicit SingleShapeParam(ShapeFwdT&& shape) noexcept :
                m_shape(std::forward<ShapeFwdT>(shape))
            {

            }

            constexpr bool operator==(const SingleShapeParam& rhs) const noexcept
            {
                return false;
            }

            constexpr const ShapeT& shape() const noexcept
            {
                return m_shape;
            }

            constexpr SingleShapeParam(const SingleShapeParam&) noexcept = default;
            constexpr SingleShapeParam(SingleShapeParam&&) noexcept = default;
            constexpr SingleShapeParam& operator=(const SingleShapeParam&) noexcept = default;
            constexpr SingleShapeParam& operator=(SingleShapeParam&&) noexcept = default;
            ~SingleShapeParam() = default;

        private:
            ShapeT m_shape;
        };

        template <typename T, typename DistributionT>
        struct SingleShapeParam<Triangle2<T>, DistributionT, 1>
        {
            using distribution_type = DistributionT;
            using ShapeType = Triangle2<T>;

            constexpr SingleShapeParam() noexcept :
                m_shape{}
            {
                update();
            }

            template <typename ShapeFwdT>
            constexpr explicit SingleShapeParam(ShapeFwdT&& shape) noexcept :
                m_shape(std::forward<ShapeFwdT>(shape))
            {
                update();
            }

            constexpr bool operator==(const SingleShapeParam& rhs) const noexcept
            {
                return false;
            }

            constexpr const Triangle2<T>& shape() const noexcept
            {
                return m_shape;
            }

            constexpr const std::array<T, 3>& edgeLengthPartialSums() const noexcept
            {
                return m_edgeLengthPartialSums;
            }

            constexpr SingleShapeParam(const SingleShapeParam&) noexcept = default;
            constexpr SingleShapeParam(SingleShapeParam&&) noexcept = default;
            constexpr SingleShapeParam& operator=(const SingleShapeParam&) noexcept = default;
            constexpr SingleShapeParam& operator=(SingleShapeParam&&) noexcept = default;
            ~SingleShapeParam() = default;

        private:
            Triangle2<T> m_shape;
            std::array<T, 3> m_edgeLengthPartialSums;

            void update() noexcept
            {
                const T lengths[3] = {
                    m_shape.vertices[0].distance(m_shape.vertices[1]),
                    m_shape.vertices[1].distance(m_shape.vertices[2]),
                    m_shape.vertices[2].distance(m_shape.vertices[0])
                };

                m_edgeLengthPartialSums[0] = lengths[0];
                m_edgeLengthPartialSums[1] = lengths[0] + lengths[1];
                m_edgeLengthPartialSums[2] = lengths[0] + lengths[1] + lengths[2];
            }
        };

        template <typename ShapeT, int DimV>
        struct UniformVec2Samplers;

        template <typename ShapeT, int DimV>
        struct UniformVec2DistributionBase
        {
            using ShapeType = ShapeT;
            static constexpr int Dim = DimV;

            using result_type = Vec2<typename ShapeType::ValueType>;
            using param_type = SingleShapeParam<ShapeType, UniformVec2DistributionBase<ShapeT, DimV>>;

            constexpr UniformVec2DistributionBase() noexcept :
                m_params{}
            {

            }

            template <typename ShapeFwdT>
            constexpr explicit UniformVec2DistributionBase(ShapeFwdT&& shape) noexcept :
                m_params(std::forward<ShapeFwdT>(shape))
            {

            }

            constexpr UniformVec2DistributionBase(const UniformVec2DistributionBase&) noexcept = default;
            constexpr UniformVec2DistributionBase(UniformVec2DistributionBase&&) noexcept = default;
            constexpr UniformVec2DistributionBase& operator=(const UniformVec2DistributionBase&) noexcept = default;
            constexpr UniformVec2DistributionBase& operator=(UniformVec2DistributionBase&&) noexcept = default;
            ~UniformVec2DistributionBase() = default;

            constexpr void reset()
            {

            }

            constexpr const param_type& param() const
            {
                return m_params;
            }

            constexpr const ShapeType& shape() const
            {
                return m_params.shape();
            }

            template <typename ParamT>
            constexpr void param(ParamT&& newParams)
            {
                m_params = std::forward<ParamT>(newParams);
            }

            constexpr bool operator==(const UniformVec2DistributionBase& rhs) const noexcept
            {
                return m_params == rhs.m_params;
            }

            constexpr bool operator!=(const UniformVec2DistributionBase& rhs) const noexcept
            {
                return !(operator==(rhs));
            }

            template <typename RngT>
            result_type operator()(RngT&& rng, const param_type& params) const
            {
                return UniformVec2Samplers<ShapeType, DimV>::sample(rng, params);
            }

            template <typename RngT>
            result_type operator()(RngT&& rng) const
            {
                return UniformVec2Samplers<ShapeType, DimV>::sample(rng, m_params);
            }

        private:
            param_type m_params;
        };

        template <typename T>
        struct UniformVec2Samplers<Circle2<T>, 2>
        {
            template <typename RngT, typename ParamsT>
            static Vec2<T> sample(RngT&& rng, const ParamsT& params)
            {
                using std::sqrt;

                const detail::UniformDistributionT<T> dAngleRad{ static_cast<T>(0), static_cast<T>(2) * pi<T> };
                const detail::UniformDistributionT<T> dRadiusSquared{ static_cast<T>(0), static_cast<T>(1) };

                const Circle2<T>& circle = params.shape();

                const T r = sqrt(dRadiusSquared(rng)) * circle.radius;
                const Angle2<T> angle = Angle2<T>::radians(dAngleRad(rng));
                return circle.origin + Vec2<T>(r * angle.cos(), r * angle.sin());
            }
        };

        template <typename T>
        struct UniformVec2Samplers<Circle2<T>, 1>
        {
            template <typename RngT, typename ParamsT>
            static Vec2<T> sample(RngT&& rng, const ParamsT& params)
            {
                const detail::UniformDistributionT<T> dAngleRad{ static_cast<T>(0), static_cast<T>(2) * pi<T> };

                const Circle2<T>& circle = params.shape();

                const Angle2<T> angle = Angle2<T>::radians(dAngleRad(rng));
                return circle.origin + Vec2<T>(angle.cos(), angle.sin()) * circle.radius;
            }
        };

        template <typename T>
        struct UniformVec2Samplers<Edge2<T>, 1>
        {
            template <typename RngT, typename ParamsT>
            static Vec2<T> sample(RngT&& rng, const ParamsT& params)
            {
                const detail::UniformDistributionT<T> dT(static_cast<T>(0), static_cast<T>(1));

                const Edge2<T>& edge = params.shape();

                const T t = dT(rng);
                return edge.vertices[0] + (edge.vertices[1] - edge.vertices[0]) * t;
            }
        };

        template <typename T>
        struct UniformVec2Samplers<Box2<T>, 1>
        {
            template <typename RngT, typename ParamsT>
            static Vec2<T> sample(RngT&& rng, const ParamsT& params)
            {
                const Box2<T>& box = params.shape();

                const T width = box.width();
                const T height = box.height();
                std::bernoulli_distribution dDoHorizontal(static_cast<double>(width) / static_cast<double>(height));
                std::bernoulli_distribution dDoOppositeEdge(0.5);

                const bool doOppositEdge = dDoOppositeEdge(rng);
                if (dDoHorizontal(rng)) //horizontally
                {
                    const detail::UniformDistributionT<T> dX(box.min.x, box.max.x);
                    const T x = dX(rng);
                    const T y = doOppositEdge ? box.min.y : box.max.y;
                }
                else //vertically
                {
                    const detail::UniformDistributionT<T> dY(box.min.y, box.max.y);
                    const T x = doOppositEdge ? box.min.x : box.max.x;
                    const T y = dY(rng);
                }

                return point;
            }
        };

        template <typename T>
        struct UniformVec2Samplers<Box2<T>, 2>
        {
            template <typename RngT, typename ParamsT>
            static Vec2<T> sample(RngT&& rng, const ParamsT& params)
            {
                const Box2<T>& box = params.shape();

                const detail::UniformDistributionT<T> dX(box.min.x, box.max.x);
                const detail::UniformDistributionT<T> dY(box.min.y, box.max.y);

                const T x = dX(rng);
                const T y = dY(rng);

                return Vec2<T>(x, y);
            }
        };

        template <typename T>
        struct UniformVec2Samplers<Triangle2<T>, 1>
        {
            template <typename RngT, typename ParamsT>
            static Vec2<T> sample(RngT&& rng, const ParamsT& params)
            {
                const detail::UniformDistributionT<T> dT(static_cast<T>(0), static_cast<T>(1));

                const Triangle2<T>& triangle = params.shape();
                const auto& edgeLengthPartialSums = params.edgeLengthPartialSums();

                const detail::UniformDistributionT<T> dLengthAlongPerimeter(static_cast<T>(0), edgeLengthPartialSums[2]);
                const T lengthAlongPerimeter = dLengthAlongPerimeter(rng);
                int edgeIndex = 0;
                if (lengthAlongPerimeter > edgeLengthPartialSums[1]) edgeIndex = 2;
                else if (lengthAlongPerimeter > edgeLengthPartialSums[0]) edgeIndex = 1;

                const Vec2<T> begin = triangle.vertices[edgeIndex];
                const Vec2<T> end = triangle.vertices[(edgeIndex + 1) % 3];
                const T t = dT(rng);
                return begin + (end - begin) * t;
            }
        };

        template <typename T>
        struct UniformVec2Samplers<Triangle2<T>, 2>
        {
            template <typename RngT, typename ParamsT>
            static Vec2<T> sample(RngT&& rng, const ParamsT& params)
            {
                const detail::UniformDistributionT<T> dT(static_cast<T>(0), static_cast<T>(1));

                const Triangle2<T>& triangle = params.shape();

                const T t1 = dT(rng);
                const T t2 = dT(rng);
                Vec2<T> point(
                    triangle.vertices[0]
                    + (triangle.vertices[2] - triangle.vertices[0]) * t1
                    + (triangle.vertices[1] - triangle.vertices[0]) * t2
                );
                const Vec2<T> e12 = triangle.vertices[2] - triangle.vertices[1];
                const Vec2<T> e10 = triangle.vertices[0] - triangle.vertices[1];
                const Vec2<T> e1p = point - triangle.vertices[1];
                if (e12.cross(e1p) * e12.cross(e10) < T(0)) //points vertices[0] and point are on the opposite sides of the diagonal of the parallelogram
                {
                    const Vec2<T> edgeCenter = triangle.vertices[1] + (e12 * T(0.5));
                    point = edgeCenter * T(2) - point;
                }
                return point;
            }
        };
    }

    template <typename ShapeT, int DimV>
    using UniformVec2Distribution = detail::UniformVec2DistributionBase<ShapeT, DimV>;

    template <typename ShapeT>
    using UniformVec2Distribution2 = UniformVec2Distribution<ShapeT, 2>;

    template <typename ShapeT>
    using UniformVec2Distribution1 = UniformVec2Distribution<ShapeT, 1>;

    // TODO: convex polygons, polylines, maybe bezier curves, circular sectors
    // TODO: maybe do 0 dim distributions for shapes with vertices (polygon, polyline, edge, box, triangle)
}
