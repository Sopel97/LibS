#pragma once

namespace ls
{
    namespace detail
    {
        struct BinomialCoefficients
        {
            static constexpr int max = 64;
            static constexpr int size = max + 1;

            static constexpr double value(int n, int k)
            {
                return instance().m_coefficients[n][k];
            }
        protected:
            constexpr BinomialCoefficients() noexcept :
                m_coefficients{}
            {
                m_coefficients[0][0] = 1.0;
                for (int n = 1; n < size; ++n)
                {
                    for (int k = 0; k <= n; ++k)
                    {
                        const double left = k > 0 ? m_coefficients[n - 1][k - 1] : 0.0;
                        const double right = k < n ? m_coefficients[n - 1][k] : 0.0;
                        m_coefficients[n][k] = left + right;
                    }
                }
            }
            static constexpr BinomialCoefficients instance()
            {
                constexpr BinomialCoefficients inst{};
                return inst;
            }

            double m_coefficients[size][size];
        };

        template <typename T, int N>
        struct PascalTriangleLine
        {
            struct iterator
            {
                using difference_type = std::ptrdiff_t;
                using value_type = T;
                using pointer = const T*;
                using reference = const T&;
                using iterator_category = std::forward_iterator_tag;

                iterator(T value, int k) :
                    m_currentValue(value),
                    m_currentK(static_cast<T>(k))
                {
                }

                iterator& operator++()
                {
                    const T nextK = m_currentK + static_cast<T>(1);
                    m_currentValue *= (static_cast<T>(N + 1) / nextK) - 1;
                    m_currentK = nextK;

                    return *this;
                }
                iterator operator++(int)
                {
                    iterator old = *this;
                    ++(*this);
                    return old;
                }

                bool operator==(const iterator& other) const
                {
                    return m_currentK == other.m_currentK;
                }
                bool operator!=(const iterator& other) const
                {
                    return !(*this == other);
                }
                reference operator*()
                {
                    return m_currentValue;
                }

            private:
                T m_currentValue;
                T m_currentK; //we only expect small integers so the stored value will be precise. Stored as float to recuce amount of conversions.
            };

            struct reverse_iterator
            {
                using difference_type = std::ptrdiff_t;
                using value_type = T;
                using pointer = const T*;
                using reference = const T&;
                using iterator_category = std::forward_iterator_tag;

                reverse_iterator(T value, int k) :
                    m_currentValue(value),
                    m_currentK(static_cast<T>(k))
                {
                }

                reverse_iterator& operator++()
                {
                    const T nextK = m_currentK - static_cast<T>(1);
                    m_currentValue /= (static_cast<T>(N + 1) / nextK) - 1;
                    m_currentK = nextK;

                    return *this;
                }
                reverse_iterator operator++(int)
                {
                    iterator old = *this;
                    ++(*this);
                    return old;
                }

                bool operator==(const reverse_iterator& other) const
                {
                    return m_currentK == other.m_currentK;
                }
                bool operator!=(const reverse_iterator& other) const
                {
                    return !(*this == other);
                }
                reference operator*()
                {
                    return m_currentValue;
                }

            private:
                T m_currentValue;
                T m_currentK; //we only expect small integers so the stored value will be precise. Stored as float to recuce amount of conversions.
            };

            iterator begin() const
            {
                return iterator(static_cast<T>(1), 0);
            }

            iterator end() const
            {
                return iterator(static_cast<T>(1), N + 1);
            }

            reverse_iterator rbegin() const
            {
                return reverse_iterator(static_cast<T>(1), N);
            }

            reverse_iterator rend() const
            {
                return reverse_iterator(static_cast<T>(1), -1);
            }
        };
    }
}
