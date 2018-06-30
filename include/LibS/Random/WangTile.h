#pragma once

#include "Fwd.h"

#include <cstdint>
#include <functional>
#include <optional>
#include <random>
#include <string>
#include <bitset>
#include <utility>
#include <vector>

namespace ls
{
    struct WangTile2
    {
    private:
        static constexpr int m_bitsPerGroupId = 8;
        static constexpr std::uint8_t m_wildcard = static_cast<std::uint8_t>((1 << m_bitsPerGroupId) - 1);
    public:

        constexpr static std::uint8_t wildcard()
        {
            return m_wildcard;
        }

        constexpr static std::uint8_t maxGroupId()
        {
            return m_wildcard - 1;
        }

        constexpr WangTile2() noexcept :
            m_north(m_wildcard),
            m_south(m_wildcard),
            m_east(m_wildcard),
            m_west(m_wildcard)
        {

        }

        constexpr WangTile2(std::uint8_t north, std::uint8_t south, std::uint8_t east, std::uint8_t west) :
            m_north(north),
            m_south(south),
            m_east(east),
            m_west(west)
        {

        }

        constexpr WangTile2(const WangTile2&) = default;
        constexpr WangTile2(WangTile2&&) = default;
        constexpr WangTile2& operator=(const WangTile2&) = default;
        constexpr WangTile2& operator=(WangTile2&&) = default;
        ~WangTile2() = default;



        constexpr std::uint32_t id() const
        {
            return m_north
                + (m_south << m_bitsPerGroupId)
                + (m_east << m_bitsPerGroupId * 2)
                + (m_west << m_bitsPerGroupId * 3);
        }

        constexpr bool operator==(const WangTile2& other) const noexcept
        {
            return id() == other.id();
        }

        constexpr bool operator!=(const WangTile2& other) const noexcept
        {
            return id() != other.id();
        }

        constexpr bool operator<(const WangTile2& other) const noexcept
        {
            return id() < other.id();
        }

        constexpr std::uint8_t north() const
        {
            return m_north;
        }

        constexpr std::uint8_t south() const
        {
            return m_south;
        }

        constexpr std::uint8_t east() const
        {
            return m_east;
        }

        constexpr std::uint8_t west() const
        {
            return m_west;
        }

        constexpr bool isFitting(WangTile2 northTile, WangTile2 southTile, WangTile2 eastTile, WangTile2 westTile) const
        {
            if (northTile.m_south != m_wildcard && m_north != m_wildcard && northTile.m_south != m_north)
            {
                return false;
            }
            if (southTile.m_north != m_wildcard && m_south != m_wildcard && southTile.m_north != m_south)
            {
                return false;
            }
            if (eastTile.m_west != m_wildcard && m_east != m_wildcard && eastTile.m_west != m_east)
            {
                return false;
            }
            if (westTile.m_east != m_wildcard && m_west != m_wildcard && westTile.m_east != m_west)
            {
                return false;
            }

            return true;
        }

    private:
        std::uint8_t m_north;
        std::uint8_t m_south;
        std::uint8_t m_east;
        std::uint8_t m_west;
    };


    struct WangTile3
    {
    private:
        static constexpr int m_bitsPerGroupId = 5;
        static constexpr std::uint8_t m_wildcard = static_cast<std::uint8_t>((1 << m_bitsPerGroupId) - 1);
    public:

        constexpr static std::uint8_t wildcard()
        {
            return m_wildcard;
        }

        constexpr static std::uint8_t maxGroupId()
        {
            return m_wildcard - 1;
        }

        constexpr WangTile3() noexcept :
            m_north(m_wildcard),
            m_south(m_wildcard),
            m_east(m_wildcard),
            m_west(m_wildcard),
            m_top(m_wildcard),
            m_bottom(m_wildcard)
        {

        }

        constexpr WangTile3(std::uint8_t north, std::uint8_t south, std::uint8_t east, std::uint8_t west, std::uint8_t top, std::uint8_t bottom) :
            m_north(north),
            m_south(south),
            m_east(east),
            m_west(west),
            m_top(top),
            m_bottom(bottom)
        {

        }

        constexpr WangTile3(const WangTile3&) = default;
        constexpr WangTile3(WangTile3&&) = default;
        constexpr WangTile3& operator=(const WangTile3&) = default;
        constexpr WangTile3& operator=(WangTile3&&) = default;
        ~WangTile3() = default;

        constexpr std::uint32_t id() const
        {
            return m_north
                + (m_south << m_bitsPerGroupId)
                + (m_east << m_bitsPerGroupId * 2)
                + (m_west << m_bitsPerGroupId * 3)
                + (m_top << m_bitsPerGroupId * 4)
                + (m_bottom << m_bitsPerGroupId * 5);
        }

        constexpr bool operator==(const WangTile3& other) const noexcept
        {
            return id() == other.id();
        }

        constexpr bool operator!=(const WangTile3& other) const noexcept
        {
            return id() != other.id();
        }

        constexpr bool operator<(const WangTile3& other) const noexcept
        {
            return id() < other.id();
        }

        constexpr std::uint8_t north() const
        {
            return m_north;
        }

        constexpr std::uint8_t south() const
        {
            return m_south;
        }

        constexpr std::uint8_t east() const
        {
            return m_east;
        }

        constexpr std::uint8_t west() const
        {
            return m_west;
        }

        constexpr std::uint8_t top() const
        {
            return m_top;
        }

        constexpr std::uint8_t bottom() const
        {
            return m_bottom;
        }

        constexpr bool isFitting(WangTile3 northTile, WangTile3 southTile, WangTile3 eastTile, WangTile3 westTile, WangTile3 topTile, WangTile3 bottomTile) const
        {
            if (northTile.m_south != m_wildcard && m_north != m_wildcard && northTile.m_south != m_north)
            {
                return false;
            }
            if (southTile.m_north != m_wildcard && m_south != m_wildcard && southTile.m_north != m_south)
            {
                return false;
            }
            if (eastTile.m_west != m_wildcard && m_east != m_wildcard && eastTile.m_west != m_east)
            {
                return false;
            }
            if (westTile.m_east != m_wildcard && m_west != m_wildcard && westTile.m_east != m_west)
            {
                return false;
            }
            if (topTile.m_bottom != m_wildcard && m_top != m_wildcard && topTile.m_bottom != m_top)
            {
                return false;
            }
            if (bottomTile.m_top != m_wildcard && m_bottom != m_wildcard && bottomTile.m_top != m_bottom)
            {
                return false;
            }

            return true;
        }

    private:
        std::uint32_t m_north : m_bitsPerGroupId;
        std::uint32_t m_south : m_bitsPerGroupId;
        std::uint32_t m_east : m_bitsPerGroupId;
        std::uint32_t m_west : m_bitsPerGroupId;
        std::uint32_t m_top : m_bitsPerGroupId;
        std::uint32_t m_bottom : m_bitsPerGroupId;
    };


    // TODO: lookup could be optimized
    struct WangTileSet2
    {
        using StorageType = std::vector<WangTile2>;

        using iterator = typename StorageType::iterator;
        using const_iterator = typename StorageType::const_iterator;
        using reverse_iterator = typename StorageType::reverse_iterator;
        using const_reverse_iterator = typename StorageType::const_reverse_iterator;

        WangTileSet2() = default;

        template <typename IterT>
        WangTileSet2(IterT begin, IterT end) :
            m_tiles(std::move(begin), std::move(end))
        {

        }

        WangTileSet2(WangTileSet2&) = default;
        WangTileSet2(WangTileSet2&&) = default;
        WangTileSet2& operator=(WangTileSet2&) = default;
        WangTileSet2& operator=(WangTileSet2&&) = default;
        ~WangTileSet2() = default;

        void addTile(const WangTile2& tile)
        {
            m_tiles.push_back(tile);
        }

        template <typename... ArgsTs>
        void emplaceTile(ArgsTs&&... args)
        {
            m_tiles.emplace_back(std::forward<ArgsTs>(args)...);
        }

        std::vector<WangTile2> fittingTiles(WangTile2 northTile, WangTile2 southTile, WangTile2 eastTile, WangTile2 westTile) const
        {
            std::vector<WangTile2> result;

            for (auto&& tile : m_tiles)
            {
                if (tile.isFitting(northTile, southTile, eastTile, westTile))
                {
                    result.push_back(tile);
                }
            }

            return result;
        }

        template <typename RngT>
        std::optional<WangTile2> randomFittingTile(WangTile2 northTile, WangTile2 southTile, WangTile2 eastTile, WangTile2 westTile, RngT&& rng) const
        {
            switch (auto fitting = fittingTiles(northTile, southTile, eastTile, westTile); fitting.size())
            {
            case 0:
                return std::nullopt;
            case 1:
                return fitting[0];
            default:
                const std::uniform_int_distribution<int> dElementId(0, static_cast<int>(fitting.size()) - 1);
                return fitting[dElementId(std::forward<RngT>(rng))];
            }
        }

        std::optional<WangTile2> firstFittingTile(WangTile2 northTile, WangTile2 southTile, WangTile2 eastTile, WangTile2 westTile) const
        {
            if (auto fitting = fittingTiles(northTile, southTile, eastTile, westTile); !fitting.empty())
            {
                return fitting[0];
            }
            else
            {
                return std::nullopt;
            }
        }

        iterator begin()
        {
            return m_tiles.begin();
        }

        iterator end()
        {
            return m_tiles.end();
        }

        const_iterator begin() const
        {
            return m_tiles.cbegin();
        }

        const_iterator end() const
        {
            return m_tiles.cend();
        }

        const_iterator cbegin() const
        {
            return m_tiles.cbegin();
        }

        const_iterator cend() const
        {
            return m_tiles.cend();
        }

        reverse_iterator rbegin()
        {
            return m_tiles.rbegin();
        }

        reverse_iterator rend()
        {
            return m_tiles.rend();
        }

        const_reverse_iterator rbegin() const
        {
            return m_tiles.crbegin();
        }

        const_reverse_iterator rend() const
        {
            return m_tiles.crend();
        }

        const_reverse_iterator crbegin() const
        {
            return m_tiles.crbegin();
        }

        const_reverse_iterator crend() const
        {
            return m_tiles.crend();
        }

    private:
        StorageType m_tiles;
    };

    // TODO: lookup could be optimized
    struct WangTileSet3
    {
        using StorageType = std::vector<WangTile3>;

        using iterator = typename StorageType::iterator;
        using const_iterator = typename StorageType::const_iterator;
        using reverse_iterator = typename StorageType::reverse_iterator;
        using const_reverse_iterator = typename StorageType::const_reverse_iterator;

        WangTileSet3() = default;

        template <typename IterT>
        WangTileSet3(IterT begin, IterT end) :
            m_tiles(std::move(begin), std::move(end))
        {

        }

        WangTileSet3(WangTileSet3&) = default;
        WangTileSet3(WangTileSet3&&) = default;
        WangTileSet3& operator=(WangTileSet3&) = default;
        WangTileSet3& operator=(WangTileSet3&&) = default;
        ~WangTileSet3() = default;

        void addTile(const WangTile3& tile)
        {
            m_tiles.push_back(tile);
        }

        template <typename... ArgsTs>
        void emplaceTile(ArgsTs&&... args)
        {
            m_tiles.emplace_back(std::forward<ArgsTs>(args)...);
        }

        std::vector<WangTile3> fittingTiles(WangTile3 northTile, WangTile3 southTile, WangTile3 eastTile, WangTile3 westTile, WangTile3 topTile, WangTile3 bottomTile) const
        {
            std::vector<WangTile3> result;

            for (auto&& tile : m_tiles)
            {
                if (tile.isFitting(northTile, southTile, eastTile, westTile, topTile, bottomTile))
                {
                    result.push_back(tile);
                }
            }

            return result;
        }

        template <typename RngT>
        std::optional<WangTile3> randomFittingTile(WangTile3 northTile, WangTile3 southTile, WangTile3 eastTile, WangTile3 westTile, WangTile3 topTile, WangTile3 bottomTile, RngT&& rng) const
        {
            switch (auto fitting = fittingTiles(northTile, southTile, eastTile, westTile, topTile, bottomTile); fitting.size())
            {
            case 0:
                return std::nullopt;
            case 1:
                return fitting[0];
            default:
                const std::uniform_int_distribution<int> dElementId(0, static_cast<int>(fitting.size()) - 1);
                return fitting[dElementId(std::forward<RngT>(rng))];
            }
        }

        std::optional<WangTile3> firstFittingTile(WangTile3 northTile, WangTile3 southTile, WangTile3 eastTile, WangTile3 westTile, WangTile3 topTile, WangTile3 bottomTile) const
        {
            if (auto fitting = fittingTiles(northTile, southTile, eastTile, westTile, topTile, bottomTile); !fitting.empty())
            {
                return fitting[0];
            }
            else
            {
                return std::nullopt;
            }
        }

        iterator begin()
        {
            return m_tiles.begin();
        }

        iterator end()
        {
            return m_tiles.end();
        }

        const_iterator begin() const
        {
            return m_tiles.cbegin();
        }

        const_iterator end() const
        {
            return m_tiles.cend();
        }

        const_iterator cbegin() const
        {
            return m_tiles.cbegin();
        }

        const_iterator cend() const
        {
            return m_tiles.cend();
        }

        reverse_iterator rbegin()
        {
            return m_tiles.rbegin();
        }

        reverse_iterator rend()
        {
            return m_tiles.rend();
        }

        const_reverse_iterator rbegin() const
        {
            return m_tiles.crbegin();
        }

        const_reverse_iterator rend() const
        {
            return m_tiles.crend();
        }

        const_reverse_iterator crbegin() const
        {
            return m_tiles.crbegin();
        }

        const_reverse_iterator crend() const
        {
            return m_tiles.crend();
        }

    private:
        StorageType m_tiles;
    };
}

namespace std
{
    template<> struct hash<ls::WangTile2>
    {
        using argument_type = ls::WangTile2;
        using result_type = std::size_t;
        result_type operator()(argument_type const& wt) const noexcept
        {
            return std::hash<std::uint32_t>()(wt.id());
        }
    };
}

namespace std
{
    template<> struct hash<ls::WangTile3>
    {
        using argument_type = ls::WangTile3;
        using result_type = std::size_t;
        result_type operator()(argument_type const& wt) const noexcept
        {
            return std::hash<std::uint32_t>()(wt.id());
        }
    };
}