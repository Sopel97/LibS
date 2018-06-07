#pragma once

#include <random>

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

