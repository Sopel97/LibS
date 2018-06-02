#pragma once

#include <iostream>
#include <vector>
#include <utility>
#include <memory>
#include <stack>
#include <tuple>
#include <functional>

namespace ls
{
    namespace detail
    {
        constexpr size_t array3DynamicDimension = std::numeric_limits<size_t>::max();
        struct Array3DynamicStorageTag;
        struct Array3AutomaticStorageTag;

        template <typename T, size_t Width = array3DynamicDimension, size_t Height = array3DynamicDimension, size_t Depth = array3DynamicDimension>
        struct Array3Enumerate
        {
            static_assert(Width > 0 && Height > 0 && Depth > 0);

        public:
            Array3Enumerate(T* data) :
                m_data(data)
            {

            }

            struct iterator
            {
            public:
                using self_type = iterator;
                using value_type = std::tuple<size_t, size_t, size_t, T&>;
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
                    if (m_z >= Depth)
                    {
                        ++m_y;
                        m_z = 0;
                        if (m_y >= Height)
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
                size_t m_x;
                size_t m_y;
                size_t m_z;
            };

            struct const_iterator
            {
            public:
                using self_type = const_iterator;
                using value_type = std::tuple<size_t, size_t, size_t, const T&>;
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
                    if (m_z >= Depth)
                    {
                        ++m_y;
                        m_z = 0;
                        if (m_y >= Height)
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
                size_t m_x;
                size_t m_y;
                size_t m_z;
            };

            iterator begin()
            {
                return iterator(m_data);
            }

            iterator end()
            {
                return iterator(m_data + Width * Height * Depth);
            }

            const_iterator begin() const
            {
                return const_iterator(m_data);
            }

            const_iterator end() const
            {
                return const_iterator(m_data + Width * Height * Depth);
            }

            const_iterator cbegin() const
            {
                return const_iterator(m_data);
            }

            const_iterator cend() const
            {
                return const_iterator(m_data + Width * Height * Depth);
            }
        private:
            T * m_data;
        };

        template <typename T>
        struct Array3Enumerate<T, array3DynamicDimension, array3DynamicDimension, array3DynamicDimension>
        {
        public:
            Array3Enumerate(T* data, size_t width, size_t height, size_t depth) :
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
                using value_type = std::tuple<size_t, size_t, size_t, T&>;
                using reference = value_type & ;
                using pointer = value_type * ;
                using iterator_category = std::input_iterator_tag;
                using difference_type = std::ptrdiff_t;

                iterator(T* data, size_t height, size_t depth) :
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
                size_t m_x;
                size_t m_y;
                size_t m_z;
                size_t m_height;
                size_t m_depth;
            };

            struct const_iterator
            {
            public:
                using self_type = const_iterator;
                using value_type = std::tuple<size_t, size_t, size_t, const T&>;
                using reference = value_type & ;
                using pointer = value_type * ;
                using iterator_category = std::input_iterator_tag;
                using difference_type = std::ptrdiff_t;

                const_iterator(const T* data, size_t height, size_t depth) :
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
                size_t m_x;
                size_t m_y;
                size_t m_z;
                size_t m_height;
                size_t m_depth;
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
            size_t m_width;
            size_t m_height;
            size_t m_depth;
        };
    }

    template <typename, size_t = detail::array3DynamicDimension, size_t = detail::array3DynamicDimension, size_t = detail::array3DynamicDimension, typename = detail::Array3DynamicStorageTag>
    struct Array3;

    template <typename T>
    struct Array3<T, detail::array3DynamicDimension, detail::array3DynamicDimension, detail::array3DynamicDimension, detail::Array3DynamicStorageTag>
    {
        using ValueType = T;
        using iterator = T * ;
        using const_iterator = const T*;

        Array3() noexcept :
        m_data(nullptr),
            m_width(0),
            m_height(0),
            m_depth(0)
        {

        }

        Array3(size_t width, size_t height, size_t depth) :
            m_width(width),
            m_height(height),
            m_depth(depth)
        {
            m_data = std::make_unique<T[]>(size());
        }

        Array3(size_t width, size_t height, size_t depth, const T& initValue) :
            m_width(width),
            m_height(height),
            m_depth(depth)
        {
            const size_t totalSize = size();
            m_data = std::make_unique<T[]>(totalSize);
            for (size_t i = 0; i < totalSize; ++i)
            {
                m_data[i] = initValue;
            }
        }

        Array3(const Array3& other) :
            m_width(other.m_width),
            m_height(other.m_height),
            m_depth(other.m_depth)
        {
            const size_t totalSize = size();
            m_data = std::make_unique<T[]>(totalSize);
            for (size_t i = 0; i < totalSize; ++i)
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

        const T& operator() (size_t x, size_t y, size_t z) const
        {
            return m_data[index(x, y, z)];
        }
        T& operator() (size_t x, size_t y, size_t z)
        {
            return m_data[index(x, y, z)];
        }
        const T& at(size_t x, size_t y, size_t z) const
        {
            return m_data[index(x, y, z)];
        }
        T& at(size_t x, size_t y, size_t z)
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

        size_t width() const
        {
            return m_width;
        }

        size_t height() const
        {
            return m_height;
        }

        size_t depth() const
        {
            return m_depth;
        }

        size_t size() const
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
        size_t m_width;
        size_t m_height;
        size_t m_depth;

        size_t index(size_t x, size_t y, size_t z) const
        {
            return (x * m_height + y) * m_depth + z;
        }
    };

    template <typename T, size_t Width, size_t Height, size_t Depth>
    struct Array3<T, Width, Height, Depth, detail::Array3DynamicStorageTag>
    {
        static_assert(Width > 0 && Height > 0 && Depth > 0);

        using ValueType = T;
        using iterator = T * ;
        using const_iterator = const T*;

        Array3()
        {
            m_data = std::make_unique<T[]>(size());
        }

        Array3(const T& initValue) :
            Array3()
        {
            for (size_t i = 0; i < size(); ++i)
            {
                m_data[i] = initValue;
            }
        }

        Array3(const Array3& other)
        {
            constexpr size_t totalSize = size();
            m_data = std::make_unique<T[]>(totalSize);
            for (size_t i = 0; i < totalSize; ++i)
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

        const T& operator() (size_t x, size_t y, size_t z) const
        {
            return m_data[index(x, y, z)];
        }
        T& operator() (size_t x, size_t y, size_t z)
        {
            return m_data[index(x, y, z)];
        }
        const T& at(size_t x, size_t y, size_t z) const
        {
            return m_data[index(x, y, z)];
        }
        T& at(size_t x, size_t y, size_t z)
        {
            return m_data[index(x, y, z)];
        }

        size_t width() const
        {
            return Width;
        }

        size_t height() const
        {
            return Height;
        }

        size_t depth() const
        {
            return Depth;
        }

        static constexpr size_t size()
        {
            return Width * Height * Depth;
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

        detail::Array3Enumerate<T, Width, Height, Depth> enumerate()
        {
            return { m_data.get() };
        }

        detail::Array3Enumerate<const T, Width, Height, Depth> cenumerate() const
        {
            return { m_data.get() };
        }

        detail::Array3Enumerate<const T, Width, Height, Depth> enumerate() const
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

        size_t index(size_t x, size_t y, size_t z) const
        {
            return (x * Height + y) * Depth + z;
        }
    };

    template <typename T, size_t Width, size_t Height, size_t Depth>
    struct Array3<T, Width, Height, Depth, detail::Array3AutomaticStorageTag>
    {
        static_assert(Width > 0 && Height > 0 && Depth > 0);

        using ValueType = T;
        using iterator = T * ;
        using const_iterator = const T*;

        constexpr Array3() noexcept :
        m_data{}
        {
        }
        constexpr Array3(const T& initValue) :
            Array3()
        {
            constexpr size_t totalSize = size();
            for (size_t i = 0; i < totalSize; ++i)
            {
                m_data[i] = initValue;
            }
        }
        constexpr Array3(const Array3& other) = default;
        constexpr Array3(Array3&& other) noexcept = default;
        constexpr Array3& operator= (Array3&& other) noexcept = default;
        constexpr Array3& operator= (const Array3& other) = default;

        constexpr const T& operator() (size_t x, size_t y, size_t z) const
        {
            return m_data[x][y][z];
        }
        constexpr T& operator() (size_t x, size_t y, size_t z)
        {
            return m_data[x][y][z];
        }
        constexpr const T& at(size_t x, size_t y, size_t z) const
        {
            return m_data[x][y][z];
        }
        constexpr T& at(size_t x, size_t y, size_t z)
        {
            return m_data[x][y][z];
        }

        constexpr size_t width() const
        {
            return Width;
        }

        constexpr size_t height() const
        {
            return Height;
        }

        constexpr size_t depth() const
        {
            return Depth;
        }

        static constexpr size_t size()
        {
            return Width * Height * Depth;
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

        detail::Array3Enumerate<T, Width, Height, Depth> enumerate()
        {
            return { data() };
        }

        detail::Array3Enumerate<const T, Width, Height, Depth> cenumerate() const
        {
            return { data() };
        }

        detail::Array3Enumerate<const T, Width, Height, Depth> enumerate() const
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
        T m_data[Width][Height][Depth];

        constexpr T* data()
        {
            return &(m_data[0][0][0]);
        }
    };

    template <typename T, size_t W, size_t H, size_t D, typename V>
    void swap(Array3<T, W, H, D, V>& lhs, Array3<T, W, H, D, V>& rhs) noexcept
    {
        lhs.swap(rhs);
    }

    template <typename T, size_t Width, size_t Height, size_t Depth>
    using AutoArray3 = Array3<T, Width, Height, Depth, detail::Array3AutomaticStorageTag>;
}

