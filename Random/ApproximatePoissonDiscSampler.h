#pragma once

#include "../Fwd.h"

#include <cstdint>
#include <functional>
#include <optional>
#include <random>
#include <string>
#include <bitset>

namespace ls
{

    // the distribution parameter is to allow for example sampling over arbitrary shapes in higher dimensions without rejection sampling
    // uniform real distribution over n-dimensional cuboid will be specialcased with a faster O(n) algorithm.
    // Mitchell’s Best-Candidate algorithm can be improved by using space partitioning to find the closest point (O(n log n) instead of O(n^2))
    // making the whole algorithm O(n^2 log n) instead of O(n^3)
    // NOTE: and since early exit was added it may be even better in average case
    // allows setting number of candidates constant, so the overall complexity can be O(n log n) after optimizations mentioned above
    // names are subject for change
    // in the end is expected to have features:
    //  - fast approximate point distribution over n-dim cuboid (using Bridson’s algorithm)
    //  - optimized approximate point distribution over arbitrary point distribution (using Mitchell’s Best-Candidate)
    //  - accurate fill of n-dim cuboid with constant min distance constraint (fixed size circles)
    //    (possibly using https://staffwww.dcs.shef.ac.uk/people/S.Maddock/research/gamito/papers/poisson.pdf)
    //    (option to generate at most N points)
    //  - accurate fill of n-dim cuboid with varying min distance constraint (per point) (varying circle sizes)
    //    a variation of above or https://gist.github.com/mbostock/6224050
    // other links:
    //  - https://bl.ocks.org/mbostock/dbb02448b0f93e4c82c3 another bridson's
    namespace detail
    {
        struct DefaultKFunc
        {
            int operator()(int n) const
            {
                return n + 1;
            }
        };
    }
    template <typename T, typename DistributionT, typename KFuncT>
    struct ApproximatePoissonDiscSampler1
    {
        template <typename DistributionFwdT>
        ApproximatePoissonDiscSampler1(DistributionFwdT&& d) :
            m_distribution(std::forward<DistributionFwdT>(d)),
            m_kFunc(KFuncT{})
        {

        }

        template <typename DistributionFwdT, typename KFuncFwdT>
        ApproximatePoissonDiscSampler1(DistributionFwdT&& d, KFuncFwdT&& kFunc) :
            m_distribution(std::forward<DistributionFwdT>(d)),
            m_kFunc(std::forward<KFuncFwdT>(kFunc))
        {

        }

        template <typename DistributionFwdT, typename RngT>
        ApproximatePoissonDiscSampler1(DistributionFwdT&& d, int numSamples, RngT&& rng) :
            m_distribution(std::forward<DistributionFwdT>(d)),
            m_kFunc(KFuncT{})
        {
            m_samples.reserve(numSamples);
            addSamples(numSamples, std::forward<RngT>(rng));
        }

        template <typename DistributionFwdT, typename KFuncFwdT, typename RngT>
        ApproximatePoissonDiscSampler1(DistributionFwdT&& d, int numSamples, RngT&& rng, KFuncFwdT&& kFunc) :
            m_distribution(std::forward<DistributionFwdT>(d)),
            m_kFunc(std::forward<KFuncFwdT>(kFunc))
        {
            m_samples.reserve(numSamples);
            addSamples(numSamples, std::forward<RngT>(rng));
        }

        // Mitchell’s Best-Candidate algorithm without any further algorithmic optimizations
        template <typename RngT>
        void addSamples(int numSamples, RngT&& rng)
        {
            for (int i = 0; i < numSamples; ++i)
            {
                const int numCandidates = m_kFunc(static_cast<int>(m_samples.size()));
                // TODO: generalize as area of distribution
                m_samples.emplace_back(chooseFurthestPoint(numCandidates, std::forward<RngT>(rng), (m_distribution.max() - m_distribution.min()) / (m_samples.size() + numSamples)));
            }
        }

        void clear()
        {
            m_samples.clear();
        }

        const std::vector<T>& samples() const
        {
            return m_samples;
        }

    private:
        DistributionT m_distribution;
        KFuncT m_kFunc;
        std::vector<T> m_samples;

        static T distance(const T& lhs, const T& rhs)
        {
            using std::abs;
            return abs(rhs - lhs);
        }

        template <typename RngT>
        T chooseFurthestPoint(int maxNumSamples, RngT&& rng, T minEarlyExitDistance) const
        {
            if (maxNumSamples == 1 || m_samples.size() == 0)
            {
                return m_distribution(rng);
            }

            T furthestCandidate;
            T longestDistance = static_cast<T>(-1);

            for (int i = 0; i < maxNumSamples; ++i)
            {
                auto candidate = m_distribution(rng);
                T shortestDistanceThisPoint = std::numeric_limits<T>::max();
                for (auto&& sample : m_samples)
                {
                    shortestDistanceThisPoint = std::min(shortestDistanceThisPoint, distance(candidate, sample));
                }

                if (shortestDistanceThisPoint > longestDistance)
                {
                    furthestCandidate = candidate;
                    longestDistance = shortestDistanceThisPoint;
                    if (longestDistance > minEarlyExitDistance)
                    {
                        break;
                    }
                }
            }

            return furthestCandidate;
        }
    };
}
