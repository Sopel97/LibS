#pragma once

#include "Shapes/Fwd.h"

#include "Shapes/Vec2.h"
#include "Shapes/Vec3.h"
#include "Shapes/Vec4.h"
#include "Quat.h"
#include "Transform3.h"
#include "Shapes/Angle2.h"

#include <cstring>

namespace ls
{
    namespace detail
    {
        template <typename T>
        struct matrix_align
        {
            static constexpr size_t value = alignof(T);
        };

        template <>
        struct matrix_align<float>
        {
            static constexpr size_t value = 16;
        };

        template <typename T, int R, int C, MatrixLayout LayoutV>
        struct MatrixStorage;

        template <typename T, int R, int C>
        struct MatrixStorage<T, R, C, MatrixLayout::RowMajor>
        {
        public:
            T* operator[](int row)
            {
                return m + row * C;
            }

            const T* operator[](int row) const
            {
                return m + row * C;
            }

            T* data()
            {
                return m;
            }

            const T* data() const
            {
                return m;
            }

        protected:
            T m[R*C];
        };

        template <typename T, int R, int C>
        struct MatrixStorage<T, R, C, MatrixLayout::ColumnMajor>
        {
        public:
            struct Column
            {
                T & operator[](int col)
                {
                    return *(m + col * R + row);
                }

                const T & operator[](int col) const
                {
                    return *(m + col * R + row);
                }

                T* m;
                int row;
            };
            struct ConstColumn
            {
                const T & operator[](int col) const
                {
                    return *(m + col * R + row);
                }

                const T* m;
                int row;
            };

            Column operator[](int row)
            {
                return { m, row };
            }

            ConstColumn operator[](int row) const
            {
                return { m, row };
            }

            T* data()
            {
                return m;
            }

            const T* data() const
            {
                return m;
            }

        protected:
            T m[R*C];
        };

        constexpr MatrixLayout oppositeLayout(MatrixLayout layout)
        {
            if (layout == MatrixLayout::ColumnMajor)
            {
                return MatrixLayout::RowMajor;
            }
            else
            {
                return MatrixLayout::ColumnMajor;
            }
        }
    }

    // row major layout
    template <typename T, int R, int C, MatrixLayout LayoutV>
    struct alignas(detail::matrix_align<T>::value) Matrix : detail::MatrixStorage<T, R, C, LayoutV>
    {
        static_assert(std::is_arithmetic<T>::value, "T must be of arithmetic type");
        static_assert(R >= 2 && R <= 4 && C >= 2 && C <= 4, "Matrix must not be smaller than 2x2 nor bigger than 4x4.");

        using Base = detail::MatrixStorage<T, R, C, LayoutV>;
    public:
        using ValueType = T;
        static constexpr int numRows = R;
        static constexpr int numColumns = C;
        static constexpr MatrixLayout layout = LayoutV;
        static constexpr MatrixLayout oppositeLayout = detail::oppositeLayout(LayoutV);

        using ColVecType = VecT<T, R>;
        using RowVecType = VecT<T, C>;

        using Base::operator[];
        using Base::data;

        static const Matrix<T, R, C, LayoutV>& zero() noexcept
        {
            static const Matrix<T, R, C, LayoutV> matrix(T(0));
            return matrix;
        }

        template <int Rows = R, int Cols = C, typename EnableT = typename std::enable_if<Rows == Cols>::type>
        static const Matrix<T, R, C, LayoutV>& identity() noexcept
        {
            static const Matrix<T, R, C, LayoutV> matrix = []()
            {
                Matrix<T, R, C, LayoutV> matrix(T(0));

                for (int i = 0; i < R; ++i)
                    matrix[i][i] = T(1);

                return matrix;
            }();

            return matrix;
        }

        template <int Rows = R, int Cols = C, typename EnableT = typename std::enable_if<(Rows == 3 && Cols == 3) || (Rows == 4 && Cols == 4)>::type>
        static Matrix<T, R, C, LayoutV> rotation(const Quat<T>& q) noexcept
        {
            static constexpr bool is4 = R == 4 && C == 4;

            Matrix<T, R, C, LayoutV> result;

            const T x2 = q.x*q.x;
            const T y2 = q.y*q.y;
            const T z2 = q.z*q.z;

            // row 0
            result[0][0] = T(1) - T(2) * (y2 + z2);
            result[0][1] = T(2) * (q.x*q.y - q.z*q.w);
            result[0][2] = T(2) * (q.x*q.z + q.y*q.w);
            if constexpr(is4)
                result[0][3] = T(0);

            // row 1
            result[1][0] = T(2) * (q.x*q.y + q.z*q.w);
            result[1][1] = T(1) - T(2) * (x2 + z2);
            result[1][2] = T(2) * (q.y*q.z - q.x*q.w);
            if constexpr(is4)
                result[1][3] = T(0);

            // row 2
            result[2][0] = T(2) * (q.x*q.z - q.y*q.w);
            result[2][1] = T(2) * (q.y*q.z + q.x*q.w);
            result[2][2] = T(1) - T(2) * (x2 + y2);
            if constexpr(is4)
                result[2][3] = T(0);

            // row 3
            if constexpr(is4)
            {
                result[3][0] = T(0);
                result[3][1] = T(0);
                result[3][2] = T(0);
                result[3][3] = T(1);
            }

            return result;
        }

        template <int Rows = R, int Cols = C, typename EnableT = typename std::enable_if<Rows == 4 && Cols == 4>::type>
        static Matrix<T, R, C, LayoutV> rotation(const Quat<T>& q, const Vec3<T>& origin) noexcept
        {
            // TODO: optimize
            return translation(origin) * rotation(q) * translation(-origin);
        }

        template <int Rows = R, int Cols = C, typename EnableT = typename std::enable_if<(Rows == 3 && Cols == 3) || (Rows == 4 && Cols == 4)>::type>
        static Matrix<T, R, C, LayoutV> scale(const T& s) noexcept
        {
            return Matrix<T, R, C, LayoutV>::scale(Vec3<T>(s, s, s));
        }

        template <int Rows = R, int Cols = C, typename EnableT = typename std::enable_if<(Rows == 3 && Cols == 3) || (Rows == 4 && Cols == 4)>::type>
        static Matrix<T, R, C, LayoutV> scale(const Vec3<T>& s) noexcept
        {
            static constexpr bool is4 = R == 4 && C == 4;

            Matrix<T, R, C, LayoutV> result;

            const T x2 = x * x;
            const T y2 = y * y;
            const T z2 = z * z;

            // row 0
            result[0][0] = s.x;
            result[0][1] = T(0);
            result[0][2] = T(0);
            if constexpr(is4)
                result[0][3] = T(0);

            // row 1
            result[1][0] = T(0);
            result[1][1] = s.y;
            result[1][2] = T(0);
            if constexpr(is4)
                result[1][3] = T(0);

            // row 2
            result[2][0] = T(0);
            result[2][1] = T(0);
            result[2][2] = s.z;
            if constexpr(is4)
                result[2][3] = T(0);

            // row 3
            if constexpr(is4)
            {
                result[3][0] = T(0);
                result[3][1] = T(0);
                result[3][2] = T(0);
                result[3][3] = T(1);
            }

            return result;
        }

        template <int Rows = R, int Cols = C, typename EnableT = typename std::enable_if<Rows == 4 && Cols == 4>::type>
        static Matrix<T, R, C, LayoutV> translation(const Vec3<T>& v) noexcept
        {
            Matrix<T, R, C, LayoutV> result;

            // row 0
            result[0][0] = T(1);
            result[0][1] = T(0);
            result[0][2] = T(0);
            result[0][3] = v.x;

            // row 1
            result[1][0] = T(0);
            result[1][1] = T(1);
            result[1][2] = T(0);
            result[1][3] = v.y;

            // row 2
            result[2][0] = T(0);
            result[2][1] = T(0);
            result[2][2] = T(1);
            result[2][3] = v.z;

            // row 3
            result[3][0] = T(0);
            result[3][1] = T(0);
            result[3][2] = T(0);
            result[3][3] = T(1);

            return result;
        }

        template <int Rows = R, int Cols = C, typename EnableT = typename std::enable_if<Rows == 4 && Cols == 4>::type>
        static Matrix<T, R, C, LayoutV> transform(const Transform3<T>& t) noexcept
        {
            Matrix<T, R, C, LayoutV> result;

            const auto& rotation = t.rotation;
            const auto& translation = t.translation;
            const auto& scale = t.scale;

            result[0][3] = translation.x;
            result[1][3] = translation.y;
            result[2][3] = translation.z;

            const T x2 = rotation.x + rotation.x;
            const T y2 = rotation.y + rotation.y;
            const T z2 = rotation.z + rotation.z;
            {
                const T xx2 = rotation.x * x2;
                const T yy2 = rotation.y * y2;
                const T zz2 = rotation.z * z2;

                result[0][0] = (T(1) - (yy2 + zz2)) * scale.x;
                result[1][1] = (T(1) - (xx2 + zz2)) * scale.y;
                result[2][2] = (T(1) - (xx2 + yy2)) * scale.z;
            }
            {
                const T yz2 = rotation.y * z2;
                const T wx2 = rotation.w * x2;

                result[1][2] = (yz2 - wx2) * scale.z;
                result[2][1] = (yz2 + wx2) * scale.y;
            }
            {
                const T xy2 = rotation.x * y2;
                const T wz2 = rotation.w * z2;

                result[0][1] = (xy2 - wz2) * scale.y;
                result[1][0] = (xy2 + wz2) * scale.x;
            }
            {
                const T xz2 = rotation.x * z2;
                const T wy2 = rotation.w * y2;

                result[0][2] = (xz2 + wy2) * scale.z;
                result[2][0] = (xz2 - wy2) * scale.x;
            }

            result[3][0] = 0.0f;
            result[3][1] = 0.0f;
            result[3][2] = 0.0f;
            result[3][3] = 1.0f;

            return result;
        }

        template <int Rows = R, int Cols = C, typename EnableT = typename std::enable_if<Rows == 4 && Cols == 4>::type>
        static Matrix<T, R, C, LayoutV> perspective(const Angle2<T>& fovY, const T& aspect, const T& nearZ, const T& farZ) noexcept
        {
            Matrix<T, 4, 4> result(T(0));

            const T tanHalfFovY = (fovY / T(2)).tan();

            result[0][0] = T(1) / (aspect * tanHalfFovY);
            result[1][1] = T(1) / (tanHalfFovY);
            result[2][2] = -(farZ + nearZ) / (farZ - nearZ);
            result[2][3] = (T(-2) * farZ * nearZ) / (farZ - nearZ);
            result[3][2] = T(-1);

            return result;
        }

        template <int Rows = R, int Cols = C, typename EnableT = typename std::enable_if<Rows == 4 && Cols == 4>::type>
        static Matrix<T, R, C, LayoutV> lookAt(const Vec3<T>& cameraPos, const Vec3<T>& cameraTarget, const Vec3<T>& upVector) noexcept
        {
            const Vec3<T> fwd = (cameraTarget - cameraPos).normalized();
            const Vec3<T> side = fwd.cross(upVector).normalized();
            const Vec3<T> up = side.cross(fwd);

            Matrix<T, 4, 4> result;

            result[0][0] = side.x;
            result[0][1] = side.y;
            result[0][2] = side.z;
            result[0][3] = -side.dot(cameraPos);

            result[1][0] = up.x;
            result[1][1] = up.y;
            result[1][2] = up.z;
            result[1][3] = -up.dot(cameraPos);

            result[2][0] = -fwd.x;
            result[2][1] = -fwd.y;
            result[2][2] = -fwd.z;
            result[2][3] = fwd.dot(cameraPos);

            result[3][0] = T(0);
            result[3][1] = T(0);
            result[3][2] = T(0);
            result[3][3] = T(1);

            return result;
        }

        constexpr Matrix() noexcept = default;

        constexpr Matrix(const Matrix<T, R, C, LayoutV>&) = default;
        constexpr Matrix(Matrix<T, R, C, LayoutV>&&) noexcept = default;

        Matrix<T, R, C, LayoutV>& operator=(const Matrix<T, R, C, LayoutV>&) = default;
        Matrix<T, R, C, LayoutV>& operator=(Matrix<T, R, C, LayoutV> &&) noexcept = default;

        constexpr Matrix(const Matrix<T, R, C, oppositeLayout>& other)
        {
            for (int r = 0; r < R; ++r)
            {
                for (int c = 0; c < C; ++c)
                {
                    (*this)[r][c] = other[r][c];
                }
            }
        }

        Matrix& operator=(const Matrix<T, R, C, oppositeLayout>& other)
        {
            for (int r = 0; r < R; ++r)
            {
                for (int c = 0; c < C; ++c)
                {
                    (*this)[r][c] = other[r][c];
                }
            }
            return *this;
        }

        explicit Matrix(const T& value) noexcept(std::is_nothrow_copy_constructible<T>::value)
        {
            fill(value);
        }

        template <MatrixLayout OtherLayoutV>
        Matrix<T, R, C, LayoutV>& operator+=(const Matrix<T, R, C, OtherLayoutV>& rhs)
        {
            for (int r = 0; r < R; ++r)
            {
                for (int c = 0; c < C; ++c)
                {
                    (*this)[r][c] += rhs[r][c];
                }
            }
            return *this;
        }

        Matrix<T, R, C, LayoutV>& operator+=(const T& rhs)
        {
            for (int r = 0; r < R; ++r)
            {
                for (int c = 0; c < C; ++c)
                {
                    (*this)[r][c] += rhs;
                }
            }
            return *this;
        }

        template <MatrixLayout OtherLayoutV>
        Matrix<T, R, C, LayoutV>& operator-=(const Matrix<T, R, C, OtherLayoutV>& rhs)
        {
            for (int r = 0; r < R; ++r)
            {
                for (int c = 0; c < C; ++c)
                {
                    (*this)[r][c] -= rhs[r][c];
                }
            }
            return *this;
        }

        Matrix<T, R, C, LayoutV>& operator-=(const T& rhs)
        {
            for (int r = 0; r < R; ++r)
            {
                for (int c = 0; c < C; ++c)
                {
                    (*this)[r][c] -= rhs;
                }
            }
            return *this;
        }

        Matrix<T, R, C, LayoutV>& operator*=(const T& rhs)
        {
            for (int r = 0; r < R; ++r)
            {
                for (int c = 0; c < C; ++c)
                {
                    (*this)[r][c] *= rhs;
                }
            }
            return *this;

        }

        Matrix<T, R, C, LayoutV>& operator/=(const T& rhs)
        {
            for (int r = 0; r < R; ++r)
            {
                for (int c = 0; c < C; ++c)
                {
                    (*this)[r][c] /= rhs;
                }
            }
            return *this;
        }


        T sum() const
        {
            T sumOfElements = 0;
            for (int r = 0; r < R; ++r)
            {
                for (int c = 0; c < C; ++c)
                {
                    sumOfElements += (*this)[r][c];
                }
            }
            return sumOfElements;
        }

        T product() const
        {
            T productOfElements = 1;
            for (int r = 0; r < R; ++r)
            {
                for (int c = 0; c < C; ++c)
                {
                    productOfElements *= (*this)[r][c];
                }
            }
            return productOfElements;
        }

        T avg() const
        {
            return sum() / (R * C);
        }

        T min() const
        {
            T minElement = (*this)[0][0];
            for (int r = 0; r < R; ++r)
            {
                for (int c = 0; c < C; ++c)
                {
                    if ((*this)[r][c] < minElement) minElement = (*this)[r][c];
                }
            }
            return minElement;
        }

        T max() const
        {
            T maxElement = (*this)[0][0];
            for (int r = 0; r < R; ++r)
            {
                for (int c = 0; c < C; ++c)
                {
                    if ((*this)[r][c] > maxElement) maxElement = (*this)[r][c];
                }
            }
            return maxElement;
        }

        template <int Rows = R, int Cols = C, typename EnableT = typename std::enable_if<Rows == Cols, T>::type>
        T trace() const
        {
            T sumOfDiagonal = 0;
            for (int i = 0; i < R; ++i) sumOfDiagonal += (*this)[i][i];
            return sumOfDiagonal;
        }

        template <int Rows = R, int Cols = C, typename EnableT = typename std::enable_if<Rows == Cols, T>::type>
        T determinant() const
        {
            if constexpr(R == 2)
            {
                return (*this)[0][0] * (*this)[1][1] - (*this)[0][1] * (*this)[1][0];
            }
            else if constexpr (R == 3)
            {
                return
                    +(*this)[0][0] * ((*this)[1][1] * (*this)[2][2] - (*this)[1][2] * (*this)[2][1])
                    + (*this)[0][1] * ((*this)[1][2] * (*this)[2][0] - (*this)[1][0] * (*this)[2][2])
                    + (*this)[0][2] * ((*this)[1][0] * (*this)[2][1] - (*this)[1][1] * (*this)[2][0]);
            }
            else // R == 4
            {
                const T sub0 = (*this)[2][2] * (*this)[3][3] - (*this)[3][2] * (*this)[2][3];
                const T sub1 = (*this)[2][1] * (*this)[3][3] - (*this)[3][1] * (*this)[2][3];
                const T sub2 = (*this)[2][1] * (*this)[3][2] - (*this)[3][1] * (*this)[2][2];
                const T sub3 = (*this)[2][0] * (*this)[3][3] - (*this)[3][0] * (*this)[2][3];
                const T sub4 = (*this)[2][0] * (*this)[3][2] - (*this)[3][0] * (*this)[2][2];
                const T sub5 = (*this)[2][0] * (*this)[3][1] - (*this)[3][0] * (*this)[2][1];

                const T cof0 = (*this)[1][1] * sub0 - (*this)[1][2] * sub1 + (*this)[1][3] * sub2;
                const T cof1 = (*this)[1][2] * sub3 - (*this)[1][0] * sub0 - (*this)[1][3] * sub4;
                const T cof2 = (*this)[1][0] * sub1 - (*this)[1][1] * sub3 + (*this)[1][3] * sub5;
                const T cof3 = (*this)[1][1] * sub4 - (*this)[1][0] * sub2 - (*this)[1][2] * sub5;


                return
                    (*this)[0][0] * cof0 + (*this)[0][1] * cof1 +
                    (*this)[0][2] * cof2 + (*this)[0][3] * cof3;
            }
        }

        template <int Rows = R, int Cols = C, typename EnableT = typename std::enable_if<Rows == Cols, T>::type>
        Matrix<T, R, C, LayoutV> inverse() const
        {
            if constexpr(R == 2)
            {
                Matrix<T, 2, 2> inv;
                inv[0][0] = (*this)[1][1];
                inv[0][1] = -(*this)[0][1];
                inv[1][0] = -(*this)[1][0];
                inv[1][1] = (*this)[0][0];
                inv /= determinant();

                return inv;
            }
            else if constexpr (R == 3)
            {
                Matrix<T, 3, 3> inv;
                inv[0][0] = +((*this)[1][1] * (*this)[2][2] - (*this)[1][2] * (*this)[2][1]);
                inv[0][1] = -((*this)[0][1] * (*this)[2][2] - (*this)[0][2] * (*this)[2][1]);
                inv[0][2] = +((*this)[0][1] * (*this)[1][2] - (*this)[0][2] * (*this)[1][1]);
                inv[1][0] = -((*this)[1][0] * (*this)[2][2] - (*this)[1][2] * (*this)[2][0]);
                inv[1][1] = +((*this)[0][0] * (*this)[2][2] - (*this)[0][2] * (*this)[2][0]);
                inv[1][2] = -((*this)[0][0] * (*this)[1][2] - (*this)[0][2] * (*this)[1][0]);
                inv[2][0] = +((*this)[1][0] * (*this)[2][1] - (*this)[1][1] * (*this)[2][0]);
                inv[2][1] = -((*this)[0][0] * (*this)[2][1] - (*this)[0][1] * (*this)[2][0]);
                inv[2][2] = +((*this)[0][0] * (*this)[1][1] - (*this)[0][1] * (*this)[1][0]);
                inv /= determinant();

                return inv;
            }
            else if constexpr (R == 4)
            {
                const T sub00 = (*this)[2][2] * (*this)[3][3] - (*this)[2][3] * (*this)[3][2];
                const T sub01 = (*this)[1][2] * (*this)[3][3] - (*this)[1][3] * (*this)[3][2];
                const T sub02 = (*this)[1][2] * (*this)[2][3] - (*this)[1][3] * (*this)[2][2];
                const T sub03 = (*this)[0][2] * (*this)[3][3] - (*this)[0][3] * (*this)[3][2];
                const T sub04 = (*this)[0][2] * (*this)[2][3] - (*this)[0][3] * (*this)[2][2];
                const T sub05 = (*this)[0][2] * (*this)[1][3] - (*this)[0][3] * (*this)[1][2];
                const T sub06 = (*this)[2][1] * (*this)[3][3] - (*this)[2][3] * (*this)[3][1];
                const T sub07 = (*this)[1][1] * (*this)[3][3] - (*this)[1][3] * (*this)[3][1];
                const T sub08 = (*this)[1][1] * (*this)[2][3] - (*this)[1][3] * (*this)[2][1];
                const T sub09 = (*this)[0][1] * (*this)[3][3] - (*this)[0][3] * (*this)[3][1];
                const T sub10 = (*this)[0][1] * (*this)[2][3] - (*this)[0][3] * (*this)[2][1];
                const T sub11 = (*this)[1][1] * (*this)[3][3] - (*this)[1][3] * (*this)[3][1];
                const T sub12 = (*this)[0][1] * (*this)[1][3] - (*this)[0][3] * (*this)[1][1];
                const T sub13 = (*this)[2][1] * (*this)[3][2] - (*this)[2][2] * (*this)[3][1];
                const T sub14 = (*this)[1][1] * (*this)[3][2] - (*this)[1][2] * (*this)[3][1];
                const T sub15 = (*this)[1][1] * (*this)[2][2] - (*this)[1][2] * (*this)[2][1];
                const T sub16 = (*this)[0][1] * (*this)[3][2] - (*this)[0][2] * (*this)[3][1];
                const T sub17 = (*this)[0][1] * (*this)[2][2] - (*this)[0][2] * (*this)[2][1];
                const T sub18 = (*this)[0][1] * (*this)[1][2] - (*this)[0][2] * (*this)[1][1];

                Matrix<T, 4, 4> inv;
                inv[0][0] = +((*this)[1][1] * sub00 - (*this)[2][1] * sub01 + (*this)[3][1] * sub02);
                inv[0][1] = -((*this)[0][1] * sub00 - (*this)[2][1] * sub03 + (*this)[3][1] * sub04);
                inv[0][2] = +((*this)[0][1] * sub01 - (*this)[1][1] * sub03 + (*this)[3][1] * sub05);
                inv[0][3] = -((*this)[0][1] * sub02 - (*this)[1][1] * sub04 + (*this)[2][1] * sub05);

                inv[1][0] = -((*this)[1][0] * sub00 - (*this)[2][0] * sub01 + (*this)[3][0] * sub02);
                inv[1][1] = +((*this)[0][0] * sub00 - (*this)[2][0] * sub03 + (*this)[3][0] * sub04);
                inv[1][2] = -((*this)[0][0] * sub01 - (*this)[1][0] * sub03 + (*this)[3][0] * sub05);
                inv[1][3] = +((*this)[0][0] * sub02 - (*this)[1][0] * sub04 + (*this)[2][0] * sub05);

                inv[2][0] = +((*this)[1][0] * sub06 - (*this)[2][0] * sub07 + (*this)[3][0] * sub08);
                inv[2][1] = -((*this)[0][0] * sub06 - (*this)[2][0] * sub09 + (*this)[3][0] * sub10);
                inv[2][2] = +((*this)[0][0] * sub11 - (*this)[1][0] * sub09 + (*this)[3][0] * sub12);
                inv[2][3] = -((*this)[0][0] * sub08 - (*this)[1][0] * sub10 + (*this)[2][0] * sub12);

                inv[3][0] = -((*this)[1][0] * sub13 - (*this)[2][0] * sub14 + (*this)[3][0] * sub15);
                inv[3][1] = +((*this)[0][0] * sub13 - (*this)[2][0] * sub16 + (*this)[3][0] * sub17);
                inv[3][2] = -((*this)[0][0] * sub14 - (*this)[1][0] * sub16 + (*this)[3][0] * sub18);
                inv[3][3] = +((*this)[0][0] * sub15 - (*this)[1][0] * sub17 + (*this)[2][0] * sub18);

                const T det =
                    + (*this)[0][0] * inv[0][0]
                    + (*this)[1][0] * inv[0][1]
                    + (*this)[2][0] * inv[0][2]
                    + (*this)[3][0] * inv[0][3];

                inv /= det;

                return inv;
            }
        }


        template <typename EnableT = std::enable_if<R == C, T>>
        typename Matrix<T, R, C, LayoutV>::RowVecType diagonal() const
        {
            Matrix<T, R, C, LayoutV>::RowVecType diagonalVector;
            for (int i = 0; i < R; ++i) diagonalVector[i] = (*this)[i][i];
            return diagonalVector;
        }

        void fill(const T& value)
        {
            for (int r = 0; r < R; ++r)
            {
                for (int c = 0; c < C; ++c)
                {
                    (*this)[r][c] = value;
                }
            }
        }

        Matrix<T, C, R, oppositeLayout> transposed() const
        {
            Matrix<T, C, R, oppositeLayout> result;
            std::memcpy(result.data(), data(), C * R * sizeof(T));
            return result;
        }

        Matrix<T, C, R, LayoutV> transposedSameLayout() const
        {
            Matrix<T, C, R, oppositeLayout> result;
            std::memcpy(result.data(), data(), C * R * sizeof(T));
            return result;
        }

    };

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

    template <typename T, int R, int C, MatrixLayout LhsLayoutV, MatrixLayout RhsLayoutV>
    bool operator ==(const Matrix<T, R, C, LhsLayoutV>& lhs, const Matrix<T, R, C, RhsLayoutV>& rhs)
    {
        for (int r = 0; r < R; ++r)
        {
            for (int c = 0; c < C; ++c)
            {
                if (lhs[r][c] != rhs[r][c]) return false;
            }
        }
        return true;
    }

    template <typename T, int R, int C, MatrixLayout LhsLayoutV, MatrixLayout RhsLayoutV>
    bool operator !=(const Matrix<T, R, C, LhsLayoutV>& lhs, const Matrix<T, R, C, RhsLayoutV>& rhs)
    {
        return !(lhs == rhs);
    }

    template <typename T, int R, int C, MatrixLayout LhsLayoutV, MatrixLayout RhsLayoutV>
    Matrix<T, R, C, LhsLayoutV> operator+(const Matrix<T, R, C, LhsLayoutV>& lhs, const Matrix<T, R, C, RhsLayoutV>& rhs)
    {
        Matrix<T, R, C, LhsLayoutV> result(lhs);
        return (result += rhs);
    }
    template <typename T, int R, int C, MatrixLayout LayoutV>
    Matrix<T, R, C, LayoutV> operator+(const Matrix<T, R, C, LayoutV>& lhs, const T& rhs)
    {
        Matrix<T, R, C, LayoutV> result(lhs);
        return (result += rhs);
    }
    template <typename T, int R, int C, MatrixLayout LhsLayoutV, MatrixLayout RhsLayoutV>
    Matrix<T, R, C, LhsLayoutV> operator-(const Matrix<T, R, C, LhsLayoutV>& lhs, const Matrix<T, R, C, RhsLayoutV>& rhs)
    {
        Matrix<T, R, C, LhsLayoutV> result(lhs);
        return (result -= rhs);
    }
    template <typename T, int R, int C, MatrixLayout LayoutV>
    Matrix<T, R, C, LayoutV> operator-(const Matrix<T, R, C, LayoutV>& lhs, const T& rhs)
    {
        Matrix<T, R, C, LayoutV> result(lhs);
        return (result -= rhs);
    }
    template <typename T, int R, int C, MatrixLayout LayoutV>
    Matrix<T, R, C, LayoutV> operator-(const Matrix<T, R, C, LayoutV>& m)
    {
        Matrix<T, R, C, LayoutV> result;
        for (int r = 0; r < R; ++r)
        {
            for (int c = 0; c < C; ++c)
            {
                result[r][c] = -m[r][c];
            }
        }
        return result;
    }
    template <typename T, int R, int C, int C2, MatrixLayout LhsLayoutV, MatrixLayout RhsLayoutV>
    Matrix<T, R, C2, LhsLayoutV> operator*(const Matrix<T, R, C, LhsLayoutV>& lhs, const Matrix<T, C, C2, RhsLayoutV>& rhs)
    {
        Matrix<T, R, C2, LhsLayoutV> result;
        for (int r = 0; r < R; ++r)
        {
            for (int c = 0; c < C2; ++c)
            {
                T resultElement = T(0);
                for (int i = 0; i < C; ++i)
                {
                    resultElement += lhs[r][i] * rhs[i][c];
                }
                result[r][c] = resultElement;
            }
        }
        return result;
    }
    template <typename T, int R, int C, MatrixLayout LayoutV>
    typename Matrix<T, R, C, LayoutV>::ColVecType operator*(const Matrix<T, R, C, LayoutV>& lhs, const typename Matrix<T, R, C, LayoutV>::RowVecType& rhs)
    {
        typename Matrix<T, R, C, LayoutV>::ColVecType result;
        for (int r = 0; r < R; ++r)
        {
            T resultElement = 0;
            for (int i = 0; i < C; ++i)
            {
                resultElement += lhs[r][i] * rhs[i];
            }
            result[r] = resultElement;
        }
        return result;
    }
    template <typename T, MatrixLayout LayoutV>
    Vec3<T> operator*(const Matrix<T, 4, 4, LayoutV>& lhs, const Vec3<T>& rhs)
    {
        Vec3<T> result;
        for (int r = 0; r < 3; ++r)
        {
            T resultElement = 0;
            for (int i = 0; i < 3; ++i)
            {
                resultElement += lhs[r][i] * rhs[i];
            }
            resultElement += lhs[r][i];
            result[r] = resultElement;
        }
        return result;
    }
    template <typename T, int R, int C, MatrixLayout LayoutV>
    Matrix<T, R, C, LayoutV> operator*(const Matrix<T, R, C, LayoutV>& lhs, const T& rhs)
    {
        Matrix<T, R, C, LayoutV> result(lhs);
        return (result *= rhs);
    }
    template <typename T, int R, int C, MatrixLayout LayoutV>
    Matrix<T, R, C, LayoutV> operator/(const Matrix<T, R, C, LayoutV>& lhs, const T& rhs)
    {
        Matrix<T, R, C, LayoutV> result(lhs);
        return (result /= rhs);
    }
}
