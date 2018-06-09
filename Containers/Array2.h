#pragma once

#include "LibS/Detail.h"

#include "Fwd.h"

#include <utility>
#include <memory>
#include <stack>
#include <tuple>
#include <iterator>

namespace ls
{
    namespace detail
    {
        struct Index2
        {
            using SizeType = ::ls::detail::SizeType;

            SizeType x, y;
        };

        template <typename T, SizeType WidthV = dynamicExtent, SizeType HeightV = dynamicExtent>
        struct Array2Enumerate
        {
            static_assert(WidthV > 0 && HeightV > 0);

        public:
            using SizeType = ::ls::detail::SizeType;

            Array2Enumerate(T* data) :
                m_data(data)
            {

            }

            struct iterator
            {
            public:
                using self_type = iterator;
                using value_type = std::tuple<SizeType, SizeType, T&>;
                using reference = value_type & ;
                using pointer = value_type * ;
                using iterator_category = std::input_iterator_tag;
                using difference_type = std::ptrdiff_t;

                iterator(T* data) :
                    m_ptr(data),
                    m_x(0),
                    m_y(0)
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
                    ++m_y;
                    if (m_y >= HeightV)
                    {
                        ++m_x;
                        m_y = 0;
                    }

                    return *this;
                }
                value_type operator*() { return { m_x, m_y, *m_ptr }; }
                bool operator==(const self_type& rhs) { return m_ptr == rhs.m_ptr; }
                bool operator!=(const self_type& rhs) { return m_ptr != rhs.m_ptr; }
            private:
                T * m_ptr;
                SizeType m_x;
                SizeType m_y;
            };

            struct const_iterator
            {
            public:
                using self_type = const_iterator;
                using value_type = std::tuple<SizeType, SizeType, const T&>;
                using reference = value_type & ;
                using pointer = value_type * ;
                using iterator_category = std::input_iterator_tag;
                using difference_type = std::ptrdiff_t;

                const_iterator(const T* data) :
                    m_ptr(data),
                    m_x(0),
                    m_y(0)
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
                    ++m_y;
                    if (m_y >= HeightV)
                    {
                        ++m_x;
                        m_y = 0;
                    }

                    return *this;
                }
                value_type operator*() { return { m_x, m_y, *m_ptr }; }
                bool operator==(const self_type& rhs) { return m_ptr == rhs.m_ptr; }
                bool operator!=(const self_type& rhs) { return m_ptr != rhs.m_ptr; }
            private:
                const T* m_ptr;
                SizeType m_x;
                SizeType m_y;
            };

            iterator begin()
            {
                return iterator(m_data);
            }

            iterator end()
            {
                return iterator(m_data + WidthV*HeightV);
            }

            const_iterator begin() const
            {
                return const_iterator(m_data);
            }

            const_iterator end() const
            {
                return const_iterator(m_data + WidthV * HeightV);
            }

            const_iterator cbegin() const
            {
                return const_iterator(m_data);
            }

            const_iterator cend() const
            {
                return const_iterator(m_data + WidthV * HeightV);
            }
        private:
            T* m_data;
        };

        template <typename T>
        struct Array2Enumerate<T, dynamicExtent, dynamicExtent>
        {
        public:
            using SizeType = ::ls::detail::SizeType;

            Array2Enumerate(T* data, SizeType width, SizeType height) :
                m_data(data),
                m_width(width),
                m_height(height)
            {

            }

            struct iterator
            {
            public:
                using self_type = iterator;
                using value_type = std::tuple<SizeType, SizeType, T&>;
                using reference = value_type&;
                using pointer = value_type*;
                using iterator_category = std::input_iterator_tag;
                using difference_type = std::ptrdiff_t;

                iterator(T* data, SizeType height) :
                    m_ptr(data), 
                    m_x(0), 
                    m_y(0), 
                    m_height(height) 
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
                    ++m_y;
                    if (m_y >= m_height)
                    {
                        ++m_x;
                        m_y = 0;
                    }

                    return *this; 
                }
                value_type operator*() { return {m_x, m_y, *m_ptr}; }
                bool operator==(const self_type& rhs) { return m_ptr == rhs.m_ptr; }
                bool operator!=(const self_type& rhs) { return m_ptr != rhs.m_ptr; }
            private:
                T* m_ptr;
                SizeType m_x;
                SizeType m_y;
                SizeType m_height;
            };

            struct const_iterator
            {
            public:
                using self_type = const_iterator;
                using value_type = std::tuple<SizeType, SizeType, const T&>;
                using reference = value_type & ;
                using pointer = value_type * ;
                using iterator_category = std::input_iterator_tag;
                using difference_type = std::ptrdiff_t;

                const_iterator(const T* data, SizeType height) :
                    m_ptr(data),
                    m_x(0),
                    m_y(0),
                    m_height(height)
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
                    ++m_y;
                    if (m_y >= m_height)
                    {
                        ++m_x;
                        m_y = 0;
                    }

                    return *this;
                }
                value_type operator*() { return { m_x, m_y, *m_ptr }; }
                bool operator==(const self_type& rhs) { return m_ptr == rhs.m_ptr; }
                bool operator!=(const self_type& rhs) { return m_ptr != rhs.m_ptr; }
            private:
                const T* m_ptr;
                SizeType m_x;
                SizeType m_y;
                SizeType m_height;
            };

            iterator begin()
            {
                return iterator(m_data, m_height);
            }

            iterator end()
            {
                return iterator(m_data + m_width * m_height, m_height);
            }

            const_iterator begin() const
            {
                return const_iterator(m_data, m_height);
            }

            const_iterator end() const
            {
                return const_iterator(m_data + m_width * m_height, m_height);
            }

            const_iterator cbegin() const
            {
                return const_iterator(m_data, m_height);
            }

            const_iterator cend() const
            {
                return const_iterator(m_data + m_width * m_height, m_height);
            }

        private:
            T * m_data;
            SizeType m_width;
            SizeType m_height;
        };
    }

    template <typename, detail::SizeType = detail::dynamicExtent, detail::SizeType = detail::dynamicExtent, ArrayStorageType = ArrayStorageType::Dynamic>
    struct Array2;

    template <typename T>
    struct Array2<T, detail::dynamicExtent, detail::dynamicExtent, ArrayStorageType::Dynamic>
    {
        using ValueType = T;
        using SizeType = detail::SizeType;

        using iterator = T * ;
        using const_iterator = const T*;

        Array2() noexcept :
        m_data(nullptr),
            m_width(0),
            m_height(0)
        {

        }

        Array2(SizeType width, SizeType height) :
            m_width(width),
            m_height(height)
        {
            m_data = std::make_unique<T[]>(size());
        }

        Array2(SizeType width, SizeType height, const T& initValue) :
            m_width(width),
            m_height(height)
        {
            const SizeType totalSize = size();
            m_data = std::make_unique<T[]>(totalSize);
            for (SizeType i = 0; i < totalSize; ++i)
            {
                m_data[i] = initValue;
            }
        }

        Array2(const Array2& other) :
            m_width(other.m_width),
            m_height(other.m_height)
        {
            const SizeType totalSize = size();
            m_data = std::make_unique<T[]>(totalSize);
            for (SizeType i = 0; i < totalSize; ++i)
            {
                m_data[i] = other.m_data[i];
            }
        }

        Array2(Array2&& other) noexcept :
        m_data(std::move(other.m_data)),
            m_width(std::move(other.m_width)),
            m_height(std::move(other.m_height))
        {
            other.m_data = nullptr;
        }

        Array2& operator= (Array2&& other) noexcept
        {
            m_data = std::move(other.m_data);
            m_width = std::move(other.m_width);
            m_height = std::move(other.m_height);

            other.m_data = nullptr;

            return *this;
        }

        Array2& operator= (const Array2& other)
        {
            return operator=(Array2(other));
        }

        const T& operator() (SizeType x, SizeType y) const
        {
            return m_data[index(x, y)];
        }
        T& operator() (SizeType x, SizeType y)
        {
            return m_data[index(x, y)];
        }
        const T& at(SizeType x, SizeType y) const
        {
            return m_data[index(x, y)];
        }
        T& at(SizeType x, SizeType y)
        {
            return m_data[index(x, y)];
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

        detail::Array2Enumerate<T> enumerate()
        {
            return { m_data.get(), m_width, m_height };
        }

        detail::Array2Enumerate<const T> cenumerate() const
        {
            return { m_data.get(), m_width, m_height };
        }

        detail::Array2Enumerate<const T> enumerate() const
        {
            return { m_data.get(), m_width, m_height };
        }

        SizeType width() const
        {
            return m_width;
        }

        SizeType height() const
        {
            return m_height;
        }

        SizeType size() const
        {
            return m_width * m_height;
        }

        void fill(const T& value)
        {
            std::fill(begin(), end(), value);
        }

        void swap(Array2& other) noexcept
        {
            Array2 temp(std::move(*this));
            *this = std::move(other);
            other = std::move(temp);
        }

        template <typename Comp = std::equal_to<T>>
        void floodFill(SizeType x, SizeType y, const T& value, Comp comp = Comp{})
        {
            SizeType width = m_width;
            SizeType height = m_height;

            std::stack<detail::Index2> coordsStack;
            coordsStack.push(detail::Index2{ x, y });
            const T initialCell = at(x, y);

            while (!coordsStack.empty())
            {
                const detail::Index2 ind = coordsStack.top();
                coordsStack.pop();
                T& thisCell = at(ind.x, ind.y);

                if (comp(value, thisCell)) continue;

                thisCell = value;
                if (ind.x > 0 && comp(at(ind.x - 1, ind.y), initialCell)) coordsStack.push(detail::Index2{ ind.x - 1, ind.y });
                if (ind.y > 0 && comp(at(ind.x, ind.y - 1), initialCell)) coordsStack.push(detail::Index2{ ind.x, ind.y - 1 });
                if (ind.x < width - 1 && comp(at(ind.x + 1, ind.y), initialCell)) coordsStack.push(detail::Index2{ ind.x + 1, ind.y });
                if (ind.y < height - 1 && comp(at(ind.x, ind.y + 1), initialCell)) coordsStack.push(detail::Index2{ ind.x, ind.y + 1 });
            }
        }

    protected:
        std::unique_ptr<T[]> m_data;
        SizeType m_width;
        SizeType m_height;

        SizeType index(SizeType x, SizeType y) const
        {
            return x * m_height + y;
        }
    };


    template <typename T, detail::SizeType WidthV, detail::SizeType HeightV>
    struct Array2<T, WidthV, HeightV, ArrayStorageType::Dynamic>
    {
        static_assert(WidthV > 0 && HeightV > 0);

        using ValueType = T;
        using SizeType = detail::SizeType;

        using iterator = T * ;
        using const_iterator = const T*;

        Array2()
        {
            m_data = std::make_unique<T[]>(size());
        }

        Array2(const T& initValue)
        {
            const SizeType totalSize = size();
            m_data = std::make_unique<T[]>(totalSize);
            for (SizeType i = 0; i < totalSize; ++i)
            {
                m_data[i] = initValue;
            }
        }

        Array2(const Array2& other)
        {
            constexpr SizeType totalSize = size();
            m_data = std::make_unique<T[]>(totalSize);
            for (SizeType i = 0; i < totalSize; ++i)
            {
                m_data[i] = other.m_data[i];
            }
        }

        Array2(Array2&& other) noexcept :
        m_data(std::move(other.m_data))
        {
            other.m_data = nullptr;
        }

        Array2& operator= (Array2&& other) noexcept
        {
            m_data = std::move(other.m_data);

            other.m_data = nullptr;

            return *this;
        }

        Array2& operator= (const Array2& other)
        {
            return operator=(Array2(other));
        }

        const T& operator() (SizeType x, SizeType y) const
        {
            return m_data[index(x, y)];
        }
        T& operator() (SizeType x, SizeType y)
        {
            return m_data[index(x, y)];
        }
        const T& at(SizeType x, SizeType y) const
        {
            return m_data[index(x, y)];
        }
        T& at(SizeType x, SizeType y)
        {
            return m_data[index(x, y)];
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

        detail::Array2Enumerate<T, WidthV, HeightV> enumerate()
        {
            return { m_data.get() };
        }

        detail::Array2Enumerate<const T, WidthV, HeightV> cenumerate() const
        {
            return { m_data.get() };
        }

        detail::Array2Enumerate<const T, WidthV, HeightV> enumerate() const
        {
            return { m_data.get() };
        }

        SizeType width() const
        {
            return WidthV;
        }

        SizeType height() const
        {
            return HeightV;
        }

        static constexpr SizeType size()
        {
            return WidthV * HeightV;
        }

        void fill(const T& value)
        {
            std::fill(begin(), end(), value);
        }

        void swap(Array2& other) noexcept
        {
            Array2 temp(std::move(*this));
            *this = std::move(other);
            other = std::move(temp);
        }

        template <typename Comp = std::equal_to<T>>
        void floodFill(SizeType x, SizeType y, const T& value, Comp comp = Comp{})
        {
            std::stack<detail::Index2> coordsStack;
            coordsStack.push(detail::Index2{ x, y });
            const T initialCell = at(x, y);

            while (!coordsStack.empty())
            {
                const detail::Index2 ind = coordsStack.top();
                coordsStack.pop();
                T& thisCell = at(ind.x, ind.y);

                if (comp(value, thisCell)) continue;

                thisCell = value;
                if (ind.x > 0 && comp(at(ind.x - 1, ind.y), initialCell)) coordsStack.push(detail::Index2{ ind.x - 1, ind.y });
                if (ind.y > 0 && comp(at(ind.x, ind.y - 1), initialCell)) coordsStack.push(detail::Index2{ ind.x, ind.y - 1 });
                if (ind.x < WidthV - 1 && comp(at(ind.x + 1, ind.y), initialCell)) coordsStack.push(detail::Index2{ ind.x + 1, ind.y });
                if (ind.y < HeightV - 1 && comp(at(ind.x, ind.y + 1), initialCell)) coordsStack.push(detail::Index2{ ind.x, ind.y + 1 });
            }
        }

    protected:
        std::unique_ptr<T[]> m_data;

        SizeType index(SizeType x, SizeType y) const
        {
            return x * HeightV + y;
        }
    };


    template <typename T, detail::SizeType WidthV, detail::SizeType HeightV>
    struct Array2<T, WidthV, HeightV, ArrayStorageType::Automatic>
    {
        static_assert(WidthV > 0 && HeightV > 0);

        using ValueType = T;
        using SizeType = detail::SizeType;
        using iterator = T * ;
        using const_iterator = const T*;

        constexpr Array2() noexcept :
        m_data{}
        {
        }

        constexpr Array2(const T& initValue) :
            Array2()
        {
            const SizeType totalSize = size();
            for (SizeType i = 0; i < totalSize; ++i)
            {
                m_data[i] = initValue;
            }
        }

        constexpr Array2(const Array2& other) = default;
        constexpr Array2(Array2&& other) noexcept = default;
        constexpr Array2& operator= (Array2&& other) noexcept = default;
        constexpr Array2& operator= (const Array2& other) = default;

        constexpr const T& operator() (SizeType x, SizeType y) const
        {
            return m_data[x][y];
        }
        constexpr T& operator() (SizeType x, SizeType y)
        {
            return m_data[x][y];
        }
        constexpr const T& at(SizeType x, SizeType y) const
        {
            return m_data[x][y];
        }
        constexpr T& at(SizeType x, SizeType y)
        {
            return m_data[x][y];
        }

        constexpr const T* data() const
        {
            return &(m_data[0][0]);
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

        detail::Array2Enumerate<T, WidthV, HeightV> enumerate()
        {
            return { data() };
        }

        detail::Array2Enumerate<const T, WidthV, HeightV> cenumerate() const
        {
            return { data() };
        }

        detail::Array2Enumerate<const T, WidthV, HeightV> enumerate() const
        {
            return { data() };
        }

        constexpr SizeType width() const
        {
            return WidthV;
        }

        constexpr SizeType height() const
        {
            return HeightV;
        }

        static constexpr SizeType size()
        {
            return WidthV * HeightV;
        }

        constexpr void fill(const T& value)
        {
            std::fill(begin(), end(), value);
        }

        constexpr void swap(Array2& other) noexcept
        {
            Array2 temp(std::move(*this));
            *this = std::move(other);
            other = std::move(temp);
        }

        template <typename Comp = std::equal_to<T>>
        void floodFill(SizeType x, SizeType y, const T& value, Comp comp = Comp{})
        {
            std::stack<detail::Index2> coordsStack;
            coordsStack.push(detail::Index2{ x, y });
            const T initialCell = at(x, y);

            while (!coordsStack.empty())
            {
                const detail::Index2 ind = coordsStack.top();
                coordsStack.pop();
                T& thisCell = at(ind.x, ind.y);

                if (comp(value, thisCell)) continue;

                thisCell = value;
                if (ind.x > 0 && comp(at(ind.x - 1, ind.y), initialCell)) coordsStack.push(detail::Index2{ ind.x - 1, ind.y });
                if (ind.y > 0 && comp(at(ind.x, ind.y - 1), initialCell)) coordsStack.push(detail::Index2{ ind.x, ind.y - 1 });
                if (ind.x < WidthV - 1 && comp(at(ind.x + 1, ind.y), initialCell)) coordsStack.push(detail::Index2{ ind.x + 1, ind.y });
                if (ind.y < HeightV - 1 && comp(at(ind.x, ind.y + 1), initialCell)) coordsStack.push(detail::Index2{ ind.x, ind.y + 1 });
            }
        }

    protected:
        T m_data[WidthV][HeightV];

        constexpr T* data()
        {
            return &(m_data[0][0]);
        }
    };

    template <typename T, detail::SizeType W, detail::SizeType H, ArrayStorageType S>
    void swap(Array2<T, W, H, S>& lhs, Array2<T, W, H, S>& rhs) noexcept
    {
        lhs.swap(rhs);
    }

    template <typename T, detail::SizeType WidthV, detail::SizeType HeightV>
    using AutoArray2 = Array2<T, WidthV, HeightV, ArrayStorageType::Automatic>;

    /*
        template <class T>
        class Array2
        {
        private:

        public:
            using ValueType = T;

            class ConstCol;

            class Col
            {
            public:
                friend class ConstCol;

                using iterator = T*;
                using const_iterator = const T*;

                Col() = default;

                Col(T* dataCol, size_t size) noexcept :
                    m_dataCol(dataCol),
                    m_colSize(size)
                {
                }

                Col(const Col& col) = default;
                Col(Col&& col) = default;

                T& operator[] (int y)
                {
                    return *(m_dataCol + y);
                }
                const T& operator[] (int y) const
                {
                    return *(m_dataCol + y);
                }

                Col& operator= (const Col& col) = default;
                Col& operator= (Col && col) = default;

                Col& operator= (std::initializer_list<T> list)
                {
                    int i = 0;
                    for(const T& v : list)
                    {
                        operator[](i) = v;
                        ++i;
                    }

                    return *this;
                }

                const T* data() const
                {
                    return m_dataCol;
                }

                void fill(const T& value)
                {
                    std::fill(begin(), end(), value);
                }

                size_t size() const
                {
                    return m_colSize;
                }

                bool empty() const
                {
                    return m_dataCol == nullptr;
                }

                iterator begin()
                {
                    return m_dataCol;
                }

                iterator end()
                {
                    return m_dataCol + m_colSize;
                }
                const_iterator begin() const
                {
                    return m_dataCol;
                }

                const_iterator end() const
                {
                    return m_dataCol + m_colSize;
                }
                const_iterator cbegin() const
                {
                    return m_dataCol;
                }

                const_iterator cend() const
                {
                    return m_dataCol + m_colSize;
                }

                Col previousCol(int n = 1) const
                {
                    return Col(m_dataCol - (m_colSize * n), m_colSize);
                }
                Col nextCol(int n = 1) const
                {
                    return Col(m_dataCol + (m_colSize * n), m_colSize);
                }
                Col& moveToPreviousCol(int n = 1)
                {
                    m_dataCol -= (m_colSize * n);

                    return *this;
                }
                Col& moveToNextCol(int n = 1)
                {
                    m_dataCol += (m_colSize * n);

                    return *this;
                }

            private:
                T* m_dataCol;
                size_t m_colSize;
            };

            class ConstCol
            {
            public:
                using const_iterator = const T*;

                ConstCol() = default;

                ConstCol(const T* dataCol, size_t size) noexcept :
                    m_dataCol(dataCol),
                    m_colSize(size)
                {
                }

                ConstCol(const Col& col) noexcept :
                m_dataCol(col.m_dataCol),
                    m_colSize(col.m_colSize)
                {

                }

                ConstCol(const ConstCol& col) = default;
                ConstCol(ConstCol&& col) = default;

                const T& operator[] (int y) const
                {
                    return *(m_dataCol + y);
                }

                ConstCol& operator= (const Col& col)
                {
                    m_dataCol = col.m_dataCol;
                    m_colSize = col.m_colSize;
                    return *this;
                }

                ConstCol& operator= (Col && col)
                {
                    m_dataCol = std::move(col.m_dataCol);
                    m_colSize = std::move(col.m_colSize);
                    return *this;
                }

                const T* data() const
                {
                    return m_dataCol;
                }

                size_t size() const
                {
                    return m_colSize;
                }

                bool empty() const
                {
                    return m_dataCol == nullptr;
                }

                const_iterator begin() const
                {
                    return m_dataCol;
                }

                const_iterator end() const
                {
                    return m_dataCol + m_colSize;
                }
                const_iterator cbegin() const
                {
                    return m_dataCol;
                }

                const_iterator cend() const
                {
                    return m_dataCol + m_colSize;
                }

                ConstCol previousCol(int n = 1) const
                {
                    return ConstCol(m_dataCol - (m_colSize * n), m_colSize);
                }
                ConstCol nextCol(int n = 1) const
                {
                    return ConstCol(m_dataCol + (m_colSize * n), m_colSize);
                }
                ConstCol& moveToPreviousCol(int n = 1)
                {
                    m_dataCol -= (m_colSize * n);

                    return *this;
                }
                ConstCol& moveToNextCol(int n = 1)
                {
                    m_dataCol += (m_colSize * n);

                    return *this;
                }

            private:
                const T* m_dataCol;
                size_t m_colSize;
            };

            class ConstRow;

            class Row
            {
            public:
                friend class ConstRow;

                class iterator
                {
                public:
                    using difference_type = ptrdiff_t;
                    using value_type = T;
                    using reference = T&;
                    using pointer = T*;
                    using iterator_category = std::random_access_iterator_tag;

                    iterator() noexcept : m_start(nullptr), m_ptr(nullptr), m_jump(0) {}
                    iterator(const iterator& other) = default;
                    iterator(pointer ptr, difference_type jump) noexcept : m_start(ptr), m_ptr(ptr), m_jump(jump) {}

                    iterator&      operator=  (const iterator& other) = default;

                    iterator       operator++ (int) { iterator i = *this; m_ptr += m_jump; return i; }
                    iterator       operator-- (int) { iterator i = *this; m_ptr -= m_jump; return i; }
                    iterator&      operator++ () { m_ptr += m_jump; return *this; }
                    iterator&      operator-- () { m_ptr -= m_jump; return *this; }

                    iterator       operator+  (int n) { iterator i = *this; i.m_ptr += m_jump * n; return i; }
                    iterator       operator-  (int n) { iterator i = *this; i.m_ptr -= m_jump * n; return i; }
                    iterator&      operator+= (int n) { m_ptr += m_jump * n; return *this; }
                    iterator&      operator-= (int n) { m_ptr -= m_jump * n; return *this; }

                    difference_type operator-  (const iterator& n) { return (m_ptr - n.m_ptr) / m_jump; }

                    reference       operator[] (int n) { return m_start[m_jump * n]; }
                    reference       operator*  () { return *m_ptr; }
                    pointer         operator-> () { return m_ptr; }

                    bool            operator== (const iterator& rhs) { return m_ptr == rhs.m_ptr; }
                    bool            operator<  (const iterator& rhs) { return m_ptr < rhs.m_ptr; }
                    bool            operator>  (const iterator& rhs) { return m_ptr > rhs.m_ptr; }
                    bool            operator<= (const iterator& rhs) { return m_ptr <= rhs.m_ptr; }
                    bool            operator>= (const iterator& rhs) { return m_ptr >= rhs.m_ptr; }
                    bool            operator!= (const iterator& rhs) { return m_ptr != rhs.m_ptr; }

                private:
                    friend class const_iterator;

                    pointer m_start;
                    pointer m_ptr;
                    difference_type m_jump;
                };

                class const_iterator
                {
                public:
                    using difference_type = ptrdiff_t;
                    using value_type = T;
                    using const_reference = const T&;
                    using const_pointer = const T*;
                    using iterator_category = std::random_access_iterator_tag;

                    const_iterator() noexcept : m_start(nullptr), m_ptr(nullptr), m_jump(0) {}
                    const_iterator(const iterator& it) noexcept : m_start(it.m_start), m_ptr(it.m_ptr), m_jump(it.m_jump) {}
                    const_iterator(const const_iterator& other) = default;
                    const_iterator(const_pointer ptr, difference_type jump) noexcept : m_start(ptr), m_ptr(ptr), m_jump(jump) {}

                    const_iterator& operator=  (const const_iterator& other) = default;

                    const_iterator  operator++ (int) { const_iterator i = *this; m_ptr += m_jump; return i; }
                    const_iterator  operator-- (int) { const_iterator i = *this; m_ptr -= m_jump; return i; }
                    const_iterator& operator++ () { m_ptr += m_jump; return *this; }
                    const_iterator& operator-- () { m_ptr -= m_jump; return *this; }

                    const_iterator  operator+  (int n) { const_iterator i = *this; i.m_ptr += m_jump * n; return i; }
                    const_iterator  operator-  (int n) { const_iterator i = *this; i.m_ptr -= m_jump * n; return i; }
                    const_iterator& operator+= (int n) { m_ptr += m_jump * n; return *this; }
                    const_iterator& operator-= (int n) { m_ptr -= m_jump * n; return *this; }

                    difference_type operator-  (const const_iterator& n) { return (m_ptr - n.m_ptr) / m_jump; }

                    const_reference operator[] (int n) { return m_start[m_jump * n]; }
                    const_reference operator*  () { return *m_ptr; }
                    const_pointer   operator-> () { return m_ptr; }

                    bool            operator== (const const_iterator& rhs) { return m_ptr == rhs.m_ptr; }
                    bool            operator<  (const const_iterator& rhs) { return m_ptr < rhs.m_ptr; }
                    bool            operator>  (const const_iterator& rhs) { return m_ptr > rhs.m_ptr; }
                    bool            operator<= (const const_iterator& rhs) { return m_ptr <= rhs.m_ptr; }
                    bool            operator>= (const const_iterator& rhs) { return m_ptr >= rhs.m_ptr; }
                    bool            operator!= (const const_iterator& rhs) { return m_ptr != rhs.m_ptr; }

                private:
                    const_pointer m_start;
                    const_pointer m_ptr;
                    difference_type m_jump;
                };

                Row() = default;

                Row(T* dataRow, size_t size, size_t colSize) noexcept :
                    m_dataRow(dataRow),
                    m_rowSize(size),
                    m_colSize(colSize)
                {
                }

                Row(const Row& row) = default;
                Row(Row&& row) = default;

                T& operator[] (size_t x)
                {
                    return *(m_dataRow + x * m_colSize);
                }
                const T& operator[] (size_t x) const
                {
                    return *(m_dataRow + x * m_colSize);
                }
                Row& operator= (const Row& row) noexcept
                {
                    m_dataRow = row.m_dataRow;
                    m_rowSize = row.m_rowSize;
                    m_colSize = row.m_colSize;

                    return *this;
                }
                Row& operator= (Row && row) noexcept
                {
                    m_dataRow = std::move(row.m_dataRow);
                    m_rowSize = std::move(row.m_rowSize);
                    m_colSize = std::move(row.m_colSize);

                    return *this;
                }


                Row& operator= (std::initializer_list<T> list)
                {
                    size_t i = 0;
                    for(const T& v : list)
                    {
                        operator[](i) = v;
                        ++i;
                    }

                    return *this;
                }

                void fill(const T& value)
                {
                    std::fill(begin(), end(), value);
                }

                size_t size() const
                {
                    return m_rowSize;
                }

                bool empty() const
                {
                    return m_dataRow == nullptr;
                }

                iterator begin()
                {
                    return iterator(m_dataRow, m_colSize);
                }

                iterator end()
                {
                    return iterator(m_dataRow + m_rowSize * m_colSize, m_colSize);
                }
                const_iterator begin() const
                {
                    return const_iterator(m_dataRow, m_colSize);
                }

                const_iterator end() const
                {
                    return const_iterator(m_dataRow + m_rowSize * m_colSize, m_colSize);
                }
                const_iterator cbegin() const
                {
                    return const_iterator(m_dataRow, m_colSize);
                }

                const_iterator cend() const
                {
                    return const_iterator(m_dataRow + m_rowSize * m_colSize, m_colSize);
                }
                Row previousRow(int n = 1) const
                {
                    return Row(m_dataRow - n, m_rowSize, m_colSize);
                }
                Row nextRow(int n = 1) const
                {
                    return Row(m_dataRow + n, m_rowSize, m_colSize);
                }
                Row& moveToPreviousRow(int n = 1)
                {
                    m_dataRow -= n;

                    return *this;
                }
                Row& moveToNextRow(int n = 1)
                {
                    m_dataRow += n;

                    return *this;
                }
            private:
                T* m_dataRow;
                size_t m_rowSize;
                size_t m_colSize;
            };

            class ConstRow
            {
            public:
                ConstRow() = default;

                ConstRow(const T* dataRow, size_t size, size_t colSize) noexcept :
                    m_dataRow(dataRow),
                    m_rowSize(size),
                    m_colSize(colSize)
                {
                }

                ConstRow(const Row& row) noexcept :
                    m_dataRow(row.m_dataRow),
                    m_rowSize(row.m_rowSize),
                    m_colSize(row.m_colSize)
                {
                }

                ConstRow(const ConstRow& row) = default;
                ConstRow(ConstRow&& row) = default;

                const T& operator[] (size_t x) const
                {
                    return *(m_dataRow + x * m_colSize);
                }
                ConstRow& operator= (const ConstRow& row) noexcept
                {
                    m_dataRow = row.m_dataRow;
                    m_rowSize = row.m_rowSize;
                    m_colSize = row.m_colSize;

                    return *this;
                }
                ConstRow& operator= (ConstRow && row)
                {
                    m_dataRow = std::move(row.m_dataRow);
                    m_rowSize = std::move(row.m_rowSize);
                    m_colSize = std::move(row.m_colSize);

                    return *this;
                }

                size_t size() const
                {
                    return m_rowSize;
                }

                bool isEmpty() const
                {
                    return m_dataRow == nullptr;
                }

                typename Row::const_iterator begin() const
                {
                    return Row::const_iterator(m_dataRow, m_colSize);
                }

                typename Row::const_iterator end() const
                {
                    return Row::const_iterator(m_dataRow + m_rowSize * m_colSize, m_colSize);
                }
                typename Row::const_iterator cbegin() const
                {
                    return Row::const_iterator(m_dataRow, m_colSize);
                }

                typename Row::const_iterator cend() const
                {
                    return Row::const_iterator(m_dataRow + m_rowSize * m_colSize, m_colSize);
                }
                ConstRow previousRow(int n = 1) const
                {
                    return ConstRow(m_dataRow - n, m_rowSize, m_colSize);
                }
                ConstRow nextRow(int n = 1) const
                {
                    return ConstRow(m_dataRow + n, m_rowSize, m_colSize);
                }
                ConstRow& moveToPreviousRow(int n = 1)
                {
                    m_dataRow -= n;

                    return *this;
                }
                ConstRow& moveToNextRow(int n = 1)
                {
                    m_dataRow += n;

                    return *this;
                }
            private:
                const T* m_dataRow;
                size_t m_rowSize;
                size_t m_colSize;
            };

            using iterator = T*;
            using const_iterator = const T*;

            Array2() :
                m_data(nullptr),
                m_width(0),
                m_height(0)
            {

            }

            Array2(size_t width, size_t height) :
                m_width(width),
                m_height(height)
            {
                const size_t totalSize = width * height;
                m_data = std::make_unique<T[]>(totalSize);
            }

            Array2(size_t width, size_t height, const T& initValue) :
                m_width(width),
                m_height(height)
            {
                const size_t totalSize = width * height;
                m_data = std::make_unique<T[]>(totalSize);
                for(size_t i = 0; i < totalSize; ++i)
                {
                    m_data[i] = initValue;
                }
            }

            Array2(const Array2<T>& other) :
                m_width(other.m_width),
                m_height(other.m_height)
            {
                const size_t totalSize = m_width * m_height;
                m_data = std::make_unique<T[]>(totalSize);
                for(size_t i = 0; i < totalSize; ++i)
                {
                    m_data[i] = other.m_data[i];
                }
            }

            Array2(Array2<T>&& other) noexcept :
                m_data(std::move(other.m_data)),
                m_width(std::move(other.m_width)),
                m_height(std::move(other.m_height))
            {
                other.m_data = nullptr;
            }

            Array2(const std::initializer_list<std::initializer_list<T>>& list)
            {
                m_height = list.size();
                m_width = (*(list.begin())).size();
                const size_t totalSize = m_width * m_height;
                m_data = std::make_unique<T[]>(totalSize);
                int y = 0;
                for(const auto& row : list)
                {
                    int x = 0;
                    for(const auto& v : row)
                    {
                        at(x, y) = v;
                        ++x;
                    }
                    ++y;
                }
            }

            ConstCol operator[] (size_t x) const
            {
                return ConstCol(m_data.get() + x * m_height, m_height);
            }

            Col operator[] (size_t x)
            {
                return Col(m_data.get() + x * m_height, m_height);
            }

            const T& operator() (size_t x, size_t y) const
            {
                return m_data[x * m_height + y];
            }
            T& operator() (size_t x, size_t y)
            {
                return m_data[x * m_height + y];
            }
            const T& at(size_t x, size_t y) const
            {
                return m_data[x * m_height + y];
            }
            T& at(size_t x, size_t y)
            {
                return m_data[x * m_height + y];
            }

            Array2& operator= (Array2<T> && other) noexcept
            {
                m_data = std::move(other.m_data);
                m_width = other.m_width;
                m_height = other.m_height;

                other.m_data = nullptr;

                return *this;
            }

            Array2& operator= (const Array2<T>& other)
            {
                return operator=(std::move(Array2(other)));
            }

            Array2& operator= (const std::initializer_list<std::initializer_list<T>>& list)
            {
                return operator=(std::move(Array2(list)));
            }
            void fill(const T& value)
            {
                std::fill(begin(), end(), value);
            }
            template <class Comp = std::equal_to<T>>
            void floodFill(int x, int y, const T& value, Comp comp = std::equal_to<T>())
            {
                int width = m_width;
                int height = m_height;

                std::stack<IndexRegion2::Index> coordsStack;
                coordsStack.push(IndexRegion2::Index{x, y});
                const T initialCell = at(x, y);

                while(!coordsStack.empty())
                {
                    const IndexRegion2::Index ind = coordsStack.top();
                    coordsStack.pop();
                    T& thisCell = at(ind.x, ind.y);

                    if(comp(value, thisCell)) continue;

                    thisCell = value;
                    if(ind.x > 0 && comp(at(ind.x - 1, ind.y), initialCell)) coordsStack.push(IndexRegion2::Index{ind.x - 1, ind.y});
                    if(ind.y > 0 && comp(at(ind.x, ind.y - 1), initialCell)) coordsStack.push(IndexRegion2::Index{ind.x, ind.y - 1});
                    if(ind.x < width - 1 && comp(at(ind.x + 1, ind.y), initialCell)) coordsStack.push(IndexRegion2::Index{ind.x + 1, ind.y});
                    if(ind.y < height - 1 && comp(at(ind.x, ind.y + 1), initialCell)) coordsStack.push(IndexRegion2::Index{ind.x, ind.y + 1});
                }
            }

            void swap(Array2<T>& other)
            {
                Array2<T> temp(std::move(*this));
                *this = std::move(other);
                other = std::move(temp);
            }

            const T* data() const
            {
                return m_data.get();
            }

            size_t width() const
            {
                return m_width;
            }

            size_t height() const
            {
                return m_height;
            }

            std::pair<size_t, size_t> size() const
            {
                return std::pair<size_t, size_t>(m_width, m_height);
            }

            bool empty() const
            {
                return m_data == nullptr;
            }

            iterator begin()
            {
                return m_data.get();
            }

            iterator end()
            {
                const size_t totalSize = m_width * m_height;
                return m_data.get() + totalSize;
            }
            const_iterator begin() const
            {
                return m_data.get();
            }

            const_iterator end() const
            {
                const size_t totalSize = m_width * m_height;
                return m_data.get() + totalSize;
            }
            const_iterator cbegin() const
            {
                return m_data.get();
            }

            const_iterator cend() const
            {
                const size_t totalSize = m_width * m_height;
                return m_data.get() + totalSize;
            }

            Row row(int n)
            {
                return Row(m_data.get() + n, m_width, m_height);
            }

            ConstRow row(int n) const
            {
                return ConstRow(m_data.get() + n, m_width, m_height);
            }

            ConstRow crow(int n) const
            {
                return ConstRow(m_data.get() + n, m_width, m_height);
            }

            Col col(int n)
            {
                return operator[](n);
            }

            ConstCol col(int n) const
            {
                return operator[](n);
            }

            ConstCol ccol(int n) const
            {
                return operator[](n);
            }

            IndexRegion2 possibleIndices() const
            {
                return IndexRegion2(0, 0, m_width - 1, m_height - 1);
            }

        private:
            std::unique_ptr<T[]> m_data;
            size_t m_width;
            size_t m_height;
        };

        template <class T>
        void swap(Array2<T>& lhs, Array2<T>& rhs) noexcept
        {
            lhs.swap(rhs);
        }
    */
}
