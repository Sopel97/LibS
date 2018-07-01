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
    }
}
