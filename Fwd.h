#pragma once

namespace ls
{
    enum struct ArrayStorageType
    {
        Automatic,
        Dynamic
    };

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
