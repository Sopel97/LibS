#pragma once

#include <cstdint>

// Noise
namespace ls
{
    template <typename T, int DimV>
    struct NoiseSampler;

    template <int SizeV, typename IntT = std::uint32_t>
    struct PermTable;

    struct OriginalPerlinPermTable;

    template <typename T, int DimV>
    struct ValueDerivativePair;

    template <typename T, typename HashT = OriginalPerlinPermTable>
    struct PerlinNoise;
    using PerlinNoiseD = PerlinNoise<double>;
    using PerlinNoiseF = PerlinNoise<float>;

    template <typename T, typename Hasher = OriginalPerlinPermTable>
    struct SimplexNoise;
    using SimplexNoiseD = SimplexNoise<double>;
    using SimplexNoiseF = SimplexNoise<float>;
}

// Json
namespace ls
{
    namespace json
    {
        struct Value;
        struct Document;
        struct Writer;
        struct DocumentParser;

        template <typename...>
        struct Reader;
    }
}

// Collisions
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

// Cellular Automata
namespace ls
{
    enum struct CellularAutomatonTopology;

    template <typename RuleT, CellularAutomatonTopology TopologyV> //class representing a rule
    struct CellularAutomaton2;

    template <typename StateT> //enum representing all possible states
    struct QuantityRule3x3;

    struct ConwaysGameOfLifeRule;

    struct WireworldRule;
}

// OpenGL
namespace ls
{
    // TODO: fwd for templated types that require opengl int types
    namespace gl
    {
        class Camera;

        class AsciiBitmapMonospaceFont;

        class Font;

        class FramebufferObject;

        class RenderbufferObject;

        struct ShaderLoadResult;
        struct ShaderLinkResult;
        class ProgramUniformView;
        class ShaderProgram;
        class ShaderProgramBuilder;

        class Text;

        class Texture2;

        class VertexArrayObject;

        template <class T>
        class VertexAttribute;
    }
}

// Random
namespace ls
{
    namespace detail
    {
        struct DefaultKFunc;
    }

    template <typename T, typename DistributionT = std::uniform_real_distribution<T>, typename KFuncT = detail::DefaultKFunc>
    struct ApproximatePoissonDiscSampler1;

    struct WangTile2;
    struct WangTile3;
    struct WangTileSet2;
    struct WangTileSet3;

    template <typename ValueT>
    struct TrainedSequenceExtender;
}

// Shapes
namespace ls
{
    template <typename T>
    struct Angle2;
    using Angle2F = Angle2<float>;
    using Angle2D = Angle2<double>;

    template <typename T>
    struct BezierCurve2;
    using BezierCurve2F = BezierCurve2<float>;
    using BezierCurve2D = BezierCurve2<double>;

    template <typename T>
    struct BezierPath2;
    using BezierPath2F = BezierPath2<float>;
    using BezierPath2D = BezierPath2<double>;

    template <typename T>
    struct Box2;
    using Box2F = Box2<float>;
    using Box2D = Box2<double>;
    using Box2I = Box2<int>;

    template <typename T>
    struct Box3;
    using Box3F = Box3<float>;
    using Box3D = Box3<double>;
    using Box3I = Box3<int>;

    template <typename T>
    struct Capsule3;
    using Capsule3F = Capsule3<float>;
    using Capsule3D = Capsule3<double>;

    template <typename T>
    struct Circle2;
    using Circle2F = Circle2<float>;
    using Circle2D = Circle2<double>;

    template <typename T>
    struct ConvexPolygon2;
    using ConvexPolygon2F = ConvexPolygon2<float>;
    using ConvexPolygon2D = ConvexPolygon2<double>;

    template <typename T>
    struct Cylinder3;
    using Cylinder3F = Cylinder3<float>;
    using Cylinder3D = Cylinder3<double>;

    template <typename T>
    struct Edge2;
    using Edge2F = Edge2<float>;
    using Edge2D = Edge2<double>;

    template <typename T>
    struct Edge3;
    using Edge3F = Edge3<float>;
    using Edge3D = Edge3<double>;

    template <typename T>
    struct Polyline2;
    using Polyline2F = Polyline2<float>;
    using Polyline2D = Polyline2<double>;

    template <typename T>
    struct Ray2;
    using Ray2F = Ray2<float>;
    using Ray2D = Ray2<double>;

    template <typename T>
    struct Ray3;
    using Ray3F = Ray3<float>;
    using Ray3D = Ray3<double>;

    template <typename T>
    struct Sphere3;
    using Sphere3F = Sphere3<float>;
    using Sphere3D = Sphere3<double>;

    template <typename T>
    struct Plane3;
    using Plane3F = Plane3<float>;
    using Plane3D = Plane3<double>;

    template <typename T>
    struct Frustum3;
    using Frustum3F = Frustum3<float>;
    using Frustum3D = Frustum3<double>;

    template <typename T>
    struct Triangle2;
    using Triangle2F = Triangle2<float>;
    using Triangle2D = Triangle2<double>;

    template <typename T>
    struct Triangle3;
    using Triangle3F = Triangle3<float>;
    using Triangle3D = Triangle3<double>;

    template <typename T>
    struct Vec2;
    using Vec2F = Vec2<float>;
    using Vec2D = Vec2<double>;
    using Vec2I = Vec2<int>;

    template <typename T>
    struct Vec3;
    using Vec3F = Vec3<float>;
    using Vec3D = Vec3<double>;
    using Vec3I = Vec3<int>;

    template <typename T>
    struct Vec4;
    using Vec4F = Vec4<float>;
    using Vec4D = Vec4<double>;
    using Vec4I = Vec4<int>;

    template <typename T, int DimV>
    struct Vec;

    template <typename T>
    struct Vec<T, 1>
    {
        using Type = T;
    };
    template <typename T>
    struct Vec<T, 2>
    {
        using Type = Vec2<T>;
    };
    template <typename T>
    struct Vec<T, 3>
    {
        using Type = Vec3<T>;
    };
    template <typename T>
    struct Vec<T, 4>
    {
        using Type = Vec4<T>;
    };

    template <typename T, int DimV>
    using VecT = typename Vec<T, DimV>::Type;
}

// Others
namespace ls
{
    template <typename T>
    struct AxisAngle;
    using AxisAngleF = AxisAngle<float>;
    using AxisAngleD = AxisAngle<double>;

    template <typename T>
    struct EulerAngles;
    using EulerAnglesF = EulerAngles<float>;
    using EulerAnglesD = EulerAngles<double>;

    enum struct MatrixLayout
    {
        RowMajor,
        ColumnMajor
    };

    template <typename T, int R, int C, MatrixLayout LayoutV = MatrixLayout::RowMajor>
    struct Matrix;

    template <typename T, int R, int C>
    using RMMatrix = Matrix<T, R, C>;

    template <typename T, int R, int C>
    using CMMatrix = Matrix<T, R, C, MatrixLayout::ColumnMajor>;

    using Matrix2x2D = Matrix<double, 2, 2>;
    using Matrix2x2F = Matrix<float, 2, 2>;
    using Matrix2x2I = Matrix<int, 2, 2>;

    using Matrix2x3D = Matrix<double, 2, 3>;
    using Matrix2x3F = Matrix<float, 2, 3>;
    using Matrix2x3I = Matrix<int, 2, 3>;

    using Matrix2x4D = Matrix<double, 2, 4>;
    using Matrix2x4F = Matrix<float, 2, 4>;
    using Matrix2x4I = Matrix<int, 2, 4>;

    using Matrix3x2D = Matrix<double, 3, 2>;
    using Matrix3x2F = Matrix<float, 3, 2>;
    using Matrix3x2I = Matrix<int, 3, 2>;

    using Matrix3x3D = Matrix<double, 3, 3>;
    using Matrix3x3F = Matrix<float, 3, 3>;
    using Matrix3x3I = Matrix<int, 3, 3>;

    using Matrix3x4D = Matrix<double, 3, 4>;
    using Matrix3x4F = Matrix<float, 3, 4>;
    using Matrix3x4I = Matrix<int, 3, 4>;

    using Matrix4x2D = Matrix<double, 4, 2>;
    using Matrix4x2F = Matrix<float, 4, 2>;
    using Matrix4x2I = Matrix<int, 4, 2>;

    using Matrix4x3D = Matrix<double, 4, 3>;
    using Matrix4x3F = Matrix<float, 4, 3>;
    using Matrix4x3I = Matrix<int, 4, 3>;

    using Matrix4x4D = Matrix<double, 4, 4>;
    using Matrix4x4F = Matrix<float, 4, 4>;
    using Matrix4x4I = Matrix<int, 4, 4>;

    using RMMatrix2x2D = Matrix<double, 2, 2>;
    using RMMatrix2x2F = Matrix<float, 2, 2>;
    using RMMatrix2x2I = Matrix<int, 2, 2>;

    using RMMatrix2x3D = Matrix<double, 2, 3>;
    using RMMatrix2x3F = Matrix<float, 2, 3>;
    using RMMatrix2x3I = Matrix<int, 2, 3>;

    using RMMatrix2x4D = Matrix<double, 2, 4>;
    using RMMatrix2x4F = Matrix<float, 2, 4>;
    using RMMatrix2x4I = Matrix<int, 2, 4>;

    using RMMatrix3x2D = Matrix<double, 3, 2>;
    using RMMatrix3x2F = Matrix<float, 3, 2>;
    using RMMatrix3x2I = Matrix<int, 3, 2>;

    using RMMatrix3x3D = Matrix<double, 3, 3>;
    using RMMatrix3x3F = Matrix<float, 3, 3>;
    using RMMatrix3x3I = Matrix<int, 3, 3>;

    using RMMatrix3x4D = Matrix<double, 3, 4>;
    using RMMatrix3x4F = Matrix<float, 3, 4>;
    using RMMatrix3x4I = Matrix<int, 3, 4>;

    using RMMatrix4x2D = Matrix<double, 4, 2>;
    using RMMatrix4x2F = Matrix<float, 4, 2>;
    using RMMatrix4x2I = Matrix<int, 4, 2>;

    using RMMatrix4x3D = Matrix<double, 4, 3>;
    using RMMatrix4x3F = Matrix<float, 4, 3>;
    using RMMatrix4x3I = Matrix<int, 4, 3>;

    using RMMatrix4x4D = Matrix<double, 4, 4>;
    using RMMatrix4x4F = Matrix<float, 4, 4>;
    using RMMatrix4x4I = Matrix<int, 4, 4>;

    using CMMatrix2x2D = Matrix<double, 2, 2, MatrixLayout::ColumnMajor>;
    using CMMatrix2x2F = Matrix<float, 2, 2, MatrixLayout::ColumnMajor>;
    using CMMatrix2x2I = Matrix<int, 2, 2, MatrixLayout::ColumnMajor>;

    using CMMatrix2x3D = Matrix<double, 2, 3, MatrixLayout::ColumnMajor>;
    using CMMatrix2x3F = Matrix<float, 2, 3, MatrixLayout::ColumnMajor>;
    using CMMatrix2x3I = Matrix<int, 2, 3, MatrixLayout::ColumnMajor>;

    using CMMatrix2x4D = Matrix<double, 2, 4, MatrixLayout::ColumnMajor>;
    using CMMatrix2x4F = Matrix<float, 2, 4, MatrixLayout::ColumnMajor>;
    using CMMatrix2x4I = Matrix<int, 2, 4, MatrixLayout::ColumnMajor>;

    using CMMatrix3x2D = Matrix<double, 3, 2, MatrixLayout::ColumnMajor>;
    using CMMatrix3x2F = Matrix<float, 3, 2, MatrixLayout::ColumnMajor>;
    using CMMatrix3x2I = Matrix<int, 3, 2, MatrixLayout::ColumnMajor>;

    using CMMatrix3x3D = Matrix<double, 3, 3, MatrixLayout::ColumnMajor>;
    using CMMatrix3x3F = Matrix<float, 3, 3, MatrixLayout::ColumnMajor>;
    using CMMatrix3x3I = Matrix<int, 3, 3, MatrixLayout::ColumnMajor>;

    using CMMatrix3x4D = Matrix<double, 3, 4, MatrixLayout::ColumnMajor>;
    using CMMatrix3x4F = Matrix<float, 3, 4, MatrixLayout::ColumnMajor>;
    using CMMatrix3x4I = Matrix<int, 3, 4, MatrixLayout::ColumnMajor>;

    using CMMatrix4x2D = Matrix<double, 4, 2, MatrixLayout::ColumnMajor>;
    using CMMatrix4x2F = Matrix<float, 4, 2, MatrixLayout::ColumnMajor>;
    using CMMatrix4x2I = Matrix<int, 4, 2, MatrixLayout::ColumnMajor>;

    using CMMatrix4x3D = Matrix<double, 4, 3, MatrixLayout::ColumnMajor>;
    using CMMatrix4x3F = Matrix<float, 4, 3, MatrixLayout::ColumnMajor>;
    using CMMatrix4x3I = Matrix<int, 4, 3, MatrixLayout::ColumnMajor>;

    using CMMatrix4x4D = Matrix<double, 4, 4, MatrixLayout::ColumnMajor>;
    using CMMatrix4x4F = Matrix<float, 4, 4, MatrixLayout::ColumnMajor>;
    using CMMatrix4x4I = Matrix<int, 4, 4, MatrixLayout::ColumnMajor>;

    template <typename T>
    struct Quat;
    using QuatF = Quat<float>;
    using QuatD = Quat<double>;

    template <typename T>
    struct Transform3;
    using Transform3D = Transform3<double>;
    using Transform3F = Transform3<float>;
}
