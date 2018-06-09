#pragma once

#include <cstddef>

namespace ls
{
    namespace detail
    {
        using SizeType = std::ptrdiff_t;
        constexpr SizeType dynamicExtent = -1;
    }
}
