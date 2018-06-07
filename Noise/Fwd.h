#pragma once

#include <cstdint>

namespace ls
{
    template <typename T, int DimV>
    struct NoiseSampler;

    template <int SizeV, typename IntT = std::uint32_t>
    struct PermTable;

    struct OriginalPerlinPermTable;

    template <typename T, int DimV>
    struct ValueDerivativePair;

    template <typename T, typename HashT = OriginalPerlinPermTable>
    struct PerlinNoise;
    using PerlinNoiseD = PerlinNoise<double>;
    using PerlinNoiseF = PerlinNoise<float>;

    template <typename T, typename Hasher = OriginalPerlinPermTable>
    struct SimplexNoise;
    using SimplexNoiseD = SimplexNoise<double>;
    using SimplexNoiseF = SimplexNoise<float>;
}
