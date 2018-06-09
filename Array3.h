#pragma once

#include "Fwd.h"
#include "Detail.h"

#include <utility>
#include <memory>
#include <tuple>
#include <iterator>

namespace ls
{
    namespace detail
    {
        template <typename T, SizeType WidthV = dynamicExtent, SizeType HeightV = dynamicExtent, SizeType DepthV = dynamicExtent>
        struct Array3Enumerate
        {
            static_assert(WidthV > 0 && HeightV > 0 && DepthV > 0);

        public:
            using SizeType = ::ls::detail::SizeType;

            Array3Enumerate(T* data) :
                m_data(data)
            {

            }

            struct iterator
            {
            public:
                using self_type = iterator;
                using value_type = std::tuple<SizeType, SizeType, SizeType, T&>;
                using reference = value_type & ;
                using pointer = value_type * ;
                using iterator_category = std::input_iterator_tag;
                using difference_type = std::ptrdiff_t;

                iterator(T* data) :
                    m_ptr(data),
                    m_x(0),
                    m_y(0),
                    m_z(0)
                {
                }
                self_type operator++(int)
                {
                    self_type i = *this;

                    operator++();

                    return i;
                }
                self_type operator++()
                {
                    ++m_ptr;
                    ++m_z;
                    if (m_z >= DepthV)
                    {
                        ++m_y;
                        m_z = 0;
                        if (m_y >= HeightV)
                        {
                            ++m_x;
                            m_y = 0;
                        }
                    }

                    return *this;
                }
                value_type operator*() { return { m_x, m_y, m_z, *m_ptr }; }
                bool operator==(const self_type& rhs) { return m_ptr == rhs.m_ptr; }
                bool operator!=(const self_type& rhs) { return m_ptr != rhs.m_ptr; }
            private:
                T * m_ptr;
                SizeType m_x;
                SizeType m_y;
                SizeType m_z;
            };

            struct const_iterator
            {
            public:
                using self_type = const_iterator;
                using value_type = std::tuple<SizeType, SizeType, SizeType, const T&>;
                using reference = value_type & ;
                using pointer = value_type * ;
                using iterator_category = std::input_iterator_tag;
                using difference_type = std::ptrdiff_t;

                const_iterator(const T* data) :
                    m_ptr(data),
                    m_x(0),
                    m_y(0),
                    m_z(0)
                {
                }
                self_type operator++(int)
                {
                    self_type i = *this;

                    operator++();

                    return i;
                }
                self_type operator++()
                {
                    ++m_ptr;
                    ++m_z;
                    if (m_z >= DepthV)
                    {
                        ++m_y;
                        m_z = 0;
                        if (m_y >= HeightV)
                        {
                            ++m_x;
                            m_y = 0;
                        }
                    }

                    return *this;
                }
                value_type operator*() { return { m_x, m_y, m_z, *m_ptr }; }
                bool operator==(const self_type& rhs) { return m_ptr == rhs.m_ptr; }
                bool operator!=(const self_type& rhs) { return m_ptr != rhs.m_ptr; }
            private:
                const T * m_ptr;
                SizeType m_x;
                SizeType m_y;
                SizeType m_z;
            };

            iterator begin()
            {
                return iterator(m_data);
            }

            iterator end()
            {
                return iterator(m_data + WidthV * HeightV * DepthV);
            }

            const_iterator begin() const
            {
                return const_iterator(m_data);
            }

            const_iterator end() const
            {
                return const_iterator(m_data + WidthV * HeightV * DepthV);
            }

            const_iterator cbegin() const
            {
                return const_iterator(m_data);
            }

            const_iterator cend() const
            {
                return const_iterator(m_data + WidthV * HeightV * DepthV);
            }
        private:
            T * m_data;
        };

        template <typename T>
        struct Array3Enumerate<T, dynamicExtent, dynamicExtent, dynamicExtent>
        {
        public:
            using SizeType = ::ls::detail::SizeType;

            Array3Enumerate(T* data, SizeType width, SizeType height, SizeType depth) :
                m_data(data),
                m_width(width),
                m_height(height),
                m_depth(depth)
            {

            }

            struct iterator
            {
            public:
                using self_type = iterator;
                using value_type = std::tuple<SizeType, SizeType, SizeType, T&>;
                using reference = value_type & ;
                using pointer = value_type * ;
                using iterator_category = std::input_iterator_tag;
                using difference_type = std::ptrdiff_t;

                iterator(T* data, SizeType height, SizeType depth) :
                    m_ptr(data),
                    m_x(0),
                    m_y(0),
                    m_z(0),
                    m_height(height),
                    m_depth(depth)
                {
                }
                self_type operator++(int)
                {
                    self_type i = *this;

                    operator++();

                    return i;
                }
                self_type operator++()
                {
                    ++m_ptr;
                    ++m_z;
                    if (m_z >= m_depth)
                    {
                        ++m_y;
                        m_z = 0;
                        if (m_y >= m_height)
                        {
                            ++m_x;
                            m_y = 0;
                        }
                    }

                    return *this;
                }
                value_type operator*() { return { m_x, m_y, m_z, *m_ptr }; }
                bool operator==(const self_type& rhs) { return m_ptr == rhs.m_ptr; }
                bool operator!=(const self_type& rhs) { return m_ptr != rhs.m_ptr; }
            private:
                T * m_ptr;
                SizeType m_x;
                SizeType m_y;
                SizeType m_z;
                SizeType m_height;
                SizeType m_depth;
            };

            struct const_iterator
            {
            public:
                using self_type = const_iterator;
                using value_type = std::tuple<SizeType, SizeType, SizeType, const T&>;
                using reference = value_type & ;
                using pointer = value_type * ;
                using iterator_category = std::input_iterator_tag;
                using difference_type = std::ptrdiff_t;

                const_iterator(const T* data, SizeType height, SizeType depth) :
                    m_ptr(data),
                    m_x(0),
                    m_y(0),
                    m_z(0),
                    m_height(height),
                    m_depth(depth)
                {
                }
                self_type operator++(int)
                {
                    self_type i = *this;

                    operator++();

                    return i;
                }
                self_type operator++()
                {
                    ++m_ptr;
                    ++m_z;
                    if (m_z >= m_depth)
                    {
                        ++m_y;
                        m_z = 0;
                        if (m_y >= m_height)
                        {
                            ++m_x;
                            m_y = 0;
                        }
                    }

                    return *this;
                }
                value_type operator*() { return { m_x, m_y, m_z, *m_ptr }; }
                bool operator==(const self_type& rhs) { return m_ptr == rhs.m_ptr; }
                bool operator!=(const self_type& rhs) { return m_ptr != rhs.m_ptr; }
            private:
                T * m_ptr;
                SizeType m_x;
                SizeType m_y;
                SizeType m_z;
                SizeType m_height;
                SizeType m_depth;
            };

            iterator begin()
            {
                return iterator(m_data, m_height, m_depth);
            }

            iterator end()
            {
                return iterator(m_data + m_width * m_height * m_depth, m_height, m_depth);
            }

            const_iterator begin() const
            {
                return const_iterator(m_data, m_height, m_depth);
            }

            const_iterator end() const
            {
                return const_iterator(m_data + m_width * m_height * m_depth, m_height, m_depth);
            }

            const_iterator cbegin() const
            {
                return const_iterator(m_data, m_height, m_depth);
            }

            const_iterator cend() const
            {
                return const_iterator(m_data + m_width * m_height * m_depth, m_height, m_depth);
            }
        private:
            T * m_data;
            SizeType m_width;
            SizeType m_height;
            SizeType m_depth;
        };
    }

    template <
        typename, 
        detail::SizeType = detail::dynamicExtent, 
        detail::SizeType = detail::dynamicExtent, 
        detail::SizeType = detail::dynamicExtent, 
        ArrayStorageType = ArrayStorageType::Dynamic
    >
    struct Array3;

    template <typename T>
    struct Array3<T, detail::dynamicExtent, detail::dynamicExtent, detail::dynamicExtent, ArrayStorageType::Dynamic>
    {
        using ValueType = T;
        using SizeType = detail::SizeType;
        using iterator = T * ;
        using const_iterator = const T*;

        Array3() noexcept :
        m_data(nullptr),
            m_width(0),
            m_height(0),
            m_depth(0)
        {

        }

        Array3(SizeType width, SizeType height, SizeType depth) :
            m_width(width),
            m_height(height),
            m_depth(depth)
        {
            m_data = std::make_unique<T[]>(size());
        }

        Array3(SizeType width, SizeType height, SizeType depth, const T& initValue) :
            m_width(width),
            m_height(height),
            m_depth(depth)
        {
            const SizeType totalSize = size();
            m_data = std::make_unique<T[]>(totalSize);
            for (SizeType i = 0; i < totalSize; ++i)
            {
                m_data[i] = initValue;
            }
        }

        Array3(const Array3& other) :
            m_width(other.m_width),
            m_height(other.m_height),
            m_depth(other.m_depth)
        {
            const SizeType totalSize = size();
            m_data = std::make_unique<T[]>(totalSize);
            for (SizeType i = 0; i < totalSize; ++i)
            {
                m_data[i] = other.m_data[i];
            }
        }

        Array3(Array3&& other) noexcept :
        m_data(std::move(other.m_data)),
            m_width(std::move(other.m_width)),
            m_height(std::move(other.m_height)),
            m_depth(std::move(other.m_depth))
        {
            other.m_data = nullptr;
        }

        Array3& operator= (Array3&& other) noexcept
        {
            m_data = std::move(other.m_data);
            m_width = std::move(other.m_width);
            m_height = std::move(other.m_height);
            m_depth = std::move(other.m_depth);

            other.m_data = nullptr;

            return *this;
        }

        Array3& operator= (const Array3& other)
        {
            return operator=(Array3(other));
        }

        const T& operator() (SizeType x, SizeType y, SizeType z) const
        {
            return m_data[index(x, y, z)];
        }
        T& operator() (SizeType x, SizeType y, SizeType z)
        {
            return m_data[index(x, y, z)];
        }
        const T& at(SizeType x, SizeType y, SizeType z) const
        {
            return m_data[index(x, y, z)];
        }
        T& at(SizeType x, SizeType y, SizeType z)
        {
            return m_data[index(x, y, z)];
        }

        const T* data() const
        {
            return m_data.get();
        }

        bool isEmpty() const
        {
            return m_data == nullptr;
        }

        iterator begin()
        {
            return m_data.get();
        }

        iterator end()
        {
            return m_data.get() + size();
        }
        const_iterator begin() const
        {
            return m_data.get();
        }

        const_iterator end() const
        {
            return m_data.get() + size();
        }
        const_iterator cbegin() const
        {
            return m_data.get();
        }

        const_iterator cend() const
        {
            return m_data.get() + size();
        }

        detail::Array3Enumerate<T> enumerate()
        {
            return { m_data.get(), m_width, m_height, m_depth };
        }

        detail::Array3Enumerate<const T> cenumerate() const
        {
            return { m_data.get(), m_width, m_height, m_depth };
        }

        detail::Array3Enumerate<const T> enumerate() const
        {
            return { m_data.get(), m_width, m_height, m_depth };
        }

        SizeType width() const
        {
            return m_width;
        }

        SizeType height() const
        {
            return m_height;
        }

        SizeType depth() const
        {
            return m_depth;
        }

        SizeType size() const
        {
            return m_width * m_height * m_depth;
        }

        void fill(const T& value)
        {
            std::fill(begin(), end(), value);
        }

        void swap(Array3& other) noexcept
        {
            Array3 temp(std::move(*this));
            *this = std::move(other);
            other = std::move(temp);
        }

    protected:
        std::unique_ptr<T[]> m_data;
        SizeType m_width;
        SizeType m_height;
        SizeType m_depth;

        SizeType index(SizeType x, SizeType y, SizeType z) const
        {
            return (x * m_height + y) * m_depth + z;
        }
    };

    template <typename T, detail::SizeType WidthV, detail::SizeType HeightV, detail::SizeType DepthV>
    struct Array3<T, WidthV, HeightV, DepthV, ArrayStorageType::Dynamic>
    {
        static_assert(WidthV > 0 && HeightV > 0 && DepthV > 0);

        using ValueType = T;
        using SizeType = detail::SizeType;
        using iterator = T * ;
        using const_iterator = const T*;

        Array3()
        {
            m_data = std::make_unique<T[]>(size());
        }

        Array3(const T& initValue) :
            Array3()
        {
            for (SizeType i = 0; i < size(); ++i)
            {
                m_data[i] = initValue;
            }
        }

        Array3(const Array3& other)
        {
            constexpr SizeType totalSize = size();
            m_data = std::make_unique<T[]>(totalSize);
            for (SizeType i = 0; i < totalSize; ++i)
            {
                m_data[i] = other.m_data[i];
            }
        }

        Array3(Array3&& other) noexcept :
        m_data(std::move(other.m_data))
        {
            other.m_data = nullptr;
        }

        Array3& operator= (Array3&& other) noexcept
        {
            m_data = std::move(other.m_data);

            other.m_data = nullptr;

            return *this;
        }

        Array3& operator= (const Array3& other)
        {
            return operator=(Array3(other));
        }

        const T& operator() (SizeType x, SizeType y, SizeType z) const
        {
            return m_data[index(x, y, z)];
        }
        T& operator() (SizeType x, SizeType y, SizeType z)
        {
            return m_data[index(x, y, z)];
        }
        const T& at(SizeType x, SizeType y, SizeType z) const
        {
            return m_data[index(x, y, z)];
        }
        T& at(SizeType x, SizeType y, SizeType z)
        {
            return m_data[index(x, y, z)];
        }

        SizeType width() const
        {
            return WidthV;
        }

        SizeType height() const
        {
            return HeightV;
        }

        SizeType depth() const
        {
            return DepthV;
        }

        static constexpr SizeType size()
        {
            return WidthV * HeightV * DepthV;
        }

        const T* data() const
        {
            return m_data.get();
        }

        bool isEmpty() const
        {
            return m_data == nullptr;
        }

        iterator begin()
        {
            return m_data.get();
        }

        iterator end()
        {
            return m_data.get() + size();
        }
        const_iterator begin() const
        {
            return m_data.get();
        }

        const_iterator end() const
        {
            return m_data.get() + size();
        }
        const_iterator cbegin() const
        {
            return m_data.get();
        }

        detail::Array3Enumerate<T, WidthV, HeightV, DepthV> enumerate()
        {
            return { m_data.get() };
        }

        detail::Array3Enumerate<const T, WidthV, HeightV, DepthV> cenumerate() const
        {
            return { m_data.get() };
        }

        detail::Array3Enumerate<const T, WidthV, HeightV, DepthV> enumerate() const
        {
            return { m_data.get() };
        }

        const_iterator cend() const
        {
            return m_data.get() + size();
        }

        void fill(const T& value)
        {
            std::fill(begin(), end(), value);
        }

        void swap(Array3& other) noexcept
        {
            Array3 temp(std::move(*this));
            *this = std::move(other);
            other = std::move(temp);
        }

    protected:
        std::unique_ptr<T[]> m_data;

        SizeType index(SizeType x, SizeType y, SizeType z) const
        {
            return (x * HeightV + y) * DepthV + z;
        }
    };

    template <typename T, detail::SizeType WidthV, detail::SizeType HeightV, detail::SizeType DepthV>
    struct Array3<T, WidthV, HeightV, DepthV, ArrayStorageType::Automatic>
    {
        static_assert(WidthV > 0 && HeightV > 0 && DepthV > 0);

        using ValueType = T;
        using SizeType = detail::SizeType;
        using iterator = T * ;
        using const_iterator = const T*;

        constexpr Array3() noexcept :
        m_data{}
        {
        }
        constexpr Array3(const T& initValue) :
            Array3()
        {
            constexpr SizeType totalSize = size();
            for (SizeType i = 0; i < totalSize; ++i)
            {
                m_data[i] = initValue;
            }
        }
        constexpr Array3(const Array3& other) = default;
        constexpr Array3(Array3&& other) noexcept = default;
        constexpr Array3& operator= (Array3&& other) noexcept = default;
        constexpr Array3& operator= (const Array3& other) = default;

        constexpr const T& operator() (SizeType x, SizeType y, SizeType z) const
        {
            return m_data[x][y][z];
        }
        constexpr T& operator() (SizeType x, SizeType y, SizeType z)
        {
            return m_data[x][y][z];
        }
        constexpr const T& at(SizeType x, SizeType y, SizeType z) const
        {
            return m_data[x][y][z];
        }
        constexpr T& at(SizeType x, SizeType y, SizeType z)
        {
            return m_data[x][y][z];
        }

        constexpr SizeType width() const
        {
            return WidthV;
        }

        constexpr SizeType height() const
        {
            return HeightV;
        }

        constexpr SizeType depth() const
        {
            return DepthV;
        }

        static constexpr SizeType size()
        {
            return WidthV * HeightV * DepthV;
        }

        constexpr const T* data() const
        {
            return &(m_data[0][0][0]);
        }

        constexpr bool isEmpty() const
        {
            return false;
        }

        constexpr iterator begin()
        {
            return data();
        }

        constexpr iterator end()
        {
            return data() + size();
        }
        constexpr const_iterator begin() const
        {
            return data();
        }

        constexpr const_iterator end() const
        {
            return data() + size();
        }
        constexpr const_iterator cbegin() const
        {
            return data();
        }

        constexpr const_iterator cend() const
        {
            return data() + size();
        }

        detail::Array3Enumerate<T, WidthV, HeightV, DepthV> enumerate()
        {
            return { data() };
        }

        detail::Array3Enumerate<const T, WidthV, HeightV, DepthV> cenumerate() const
        {
            return { data() };
        }

        detail::Array3Enumerate<const T, WidthV, HeightV, DepthV> enumerate() const
        {
            return { data() };
        }

        constexpr void fill(const T& value)
        {
            std::fill(begin(), end(), value);
        }

        constexpr void swap(Array3& other) noexcept
        {
            Array3 temp(std::move(*this));
            *this = std::move(other);
            other = std::move(temp);
        }

    protected:
        T m_data[WidthV][HeightV][DepthV];

        constexpr T* data()
        {
            return &(m_data[0][0][0]);
        }
    };

    template <typename T, detail::SizeType W, detail::SizeType H, detail::SizeType D, ArrayStorageType S>
    void swap(Array3<T, W, H, D, S>& lhs, Array3<T, W, H, D, S>& rhs) noexcept
    {
        lhs.swap(rhs);
    }

    template <typename T, detail::SizeType WidthV, detail::SizeType HeightV, detail::SizeType DepthV>
    using AutoArray3 = Array3<T, WidthV, HeightV, DepthV, ArrayStorageType::Automatic>;
}

