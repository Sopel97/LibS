#pragma once

#include "LibS/Random/Fwd.h"

#include <map>
#include <random>
#include <vector>
#include <iterator>
#include <algorithm>

namespace ls
{
    template <typename ValueT>
    struct TrainedSequenceExtender
    {
        // Markov Chains based
    private:
        using DistributionType = std::discrete_distribution<>;
        using ValueType = ValueT;

        struct Node
        {
            DistributionType distribution;
            std::vector<ValueType> options;

            Node(std::map<ValueType, int>&& histogram)
            {
                const auto numElements = histogram.size();

                options.reserve(numElements);

                std::vector<int> counts;
                counts.reserve(numElements);

                for (auto&& el : std::move(histogram))
                {
                    options.emplace_back(std::move(el.first));
                    counts.emplace_back(el.second);
                }

                distribution = std::discrete_distribution<>(counts.begin(), counts.end());
            }

            template <typename RngT>
            const ValueType& sample(RngT&& rng) const
            {
                return options[distribution(rng)];
            }

        };

        struct Sequence
        {
            template <typename InputItT>
            Sequence(InputItT first, InputItT last) :
                elements(first, last)
            {

            }

            Sequence(const Sequence&) = default;
            Sequence(Sequence&&) noexcept = default;
            Sequence& operator=(const Sequence&) = default;
            Sequence& operator=(Sequence&&) noexcept = default;

            std::vector<ValueType> elements;

            auto begin() const { return elements.cbegin(); }
            auto end() const { return elements.cend(); }
        };

        template <typename InputItT>
        struct SequenceView
        {
            SequenceView(InputItT first, InputItT last) :
                first(first),
                last(last)
            {

            }

            SequenceView(const SequenceView&) = default;
            SequenceView(SequenceView&&) noexcept = default;
            SequenceView& operator=(const SequenceView&) = default;
            SequenceView& operator=(SequenceView&&) noexcept = default;

            InputItT first;
            InputItT last;

            InputItT begin() const { return first; }
            InputItT end() const { return last; }
        };

        struct SequenceCompare
        {
            using is_transparent = void;

            template <typename Seq1T, typename Seq2T>
            bool operator()(const Seq1T& lhs, const Seq2T& rhs) const noexcept
            {
                return std::lexicographical_compare(std::begin(lhs), std::end(lhs), std::begin(rhs), std::end(rhs));
            }
        };

    public:
        template <typename InputItT>
        TrainedSequenceExtender(InputItT first, InputItT last, int depth) :
            m_depthZeroNode(singleValueHistogram(first, last)),
            m_fallbackNode(singleValueHistogram(first, last)),
            m_nodesByDepth(depth + 1),
            m_depth(depth)
        {
            trainFromContainerRange(first, last, depth);
        }

        template <typename BidirItT, typename RngT>
        const ValueType& nextAfter(BidirItT first, BidirItT last, RngT&& rng) const
        {
            const int length = static_cast<int>(std::distance(first, last));
            if (length == 0)
            {
                return m_depthZeroNode.sample(rng);
            }

            const int d = std::min(m_depth, length);
            first = std::prev(last, d);

            while (first != last)
            {
                const auto& thisDepthNodes = m_nodesByDepth[d];
                auto iter = thisDepthNodes.find(SequenceView(first, last));
                if (iter != thisDepthNodes.end())
                {
                    return iter->second.sample(rng);
                }

                ++first;
            }

            return m_fallbackNode.sample(rng);
        }

        template <typename ContainerT, typename RngT>
        ContainerT generate(int size, RngT&& rng) const
        {
            ContainerT sequence;

            for (int i = 0; i < size; ++i)
            {
                const ValueType& next = nextAfter(std::begin(sequence), std::end(sequence), rng);
                sequence.push_back(next);
            }

            return sequence;
        }

        template <template <typename> typename ContainerT, typename RngT>
        ContainerT<ValueType> generate(int size, RngT&& rng) const
        {
            return generate<ContainerT<ValueType>>(size, rng);
        }

        template <typename ContainerT, typename RngT>
        void append(ContainerT& cont, int n, RngT&& rng) const
        {
            for (int i = 0; i < n; ++i)
            {
                const ValueType& next = nextAfter(std::begin(cont), std::end(cont), rng);
                cont.push_back(next);
            }
        }

    private:
        int m_depth;
        Node m_depthZeroNode;
        Node m_fallbackNode;
        std::vector<std::map<Sequence, Node, SequenceCompare>> m_nodesByDepth;

        template <typename InputIt>
        void trainFromContainerRange(InputIt first, InputIt last, int depth)
        {
            std::map<ValueType, int> firstElementHistogram = generateFirstElementHistogram(first, last);

            for (int d = 1; d <= depth; ++d)
            {
                generateNodes(first, last, d);
            }
        }

        template <typename InputIt>
        std::map<ValueType, int> singleValueHistogram(InputIt first, InputIt last)
        {
            std::map<ValueType, int> histogram;

            for (; first != last; ++first)
            {
                const auto& el = *first;
                const auto innerLast = std::end(el);
                for (auto innerFirst = std::begin(el); innerFirst != innerLast; ++innerFirst)
                {
                    ++histogram[*innerFirst];
                }
            }

            return histogram;
        }

        template <typename InputIt>
        std::map<ValueType, int> generateFirstElementHistogram(InputIt first, InputIt last)
        {
            std::map<ValueType, int> histogram;

            for (; first != last; ++first)
            {
                const auto& el = *first;
                ++histogram[*std::begin(el)];
            }

            return histogram;
        }

        template <typename InputIt>
        void generateNodes(InputIt first, InputIt last, int depth)
        {
            std::map<Sequence, std::map<ValueType, int>, SequenceCompare> histograms;

            for (; first != last; ++first)
            {
                const auto& el = *first;
                auto innerFirst = std::begin(el);
                const auto innerLast = std::end(el);
                if (std::distance(innerFirst, innerLast) <= depth)
                {
                    continue;
                }

                auto innerSecond = std::next(innerFirst, depth);
                for (; innerSecond != innerLast; ++innerFirst, ++innerSecond)
                {
                    auto iter = histograms.find(SequenceView(innerFirst, innerLast));
                    if (iter == histograms.end())
                    {
                        ++histograms[Sequence(innerFirst, innerSecond)][*innerSecond];
                    }
                    else
                    {
                        ++(iter->second[*innerSecond]);
                    }
                }
            }

            for (auto&& histogram : std::move(histograms))
            {
                m_nodesByDepth[depth].emplace(std::move(histogram.first), std::move(histogram.second));
            }
        }
    };
}
