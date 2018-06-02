#pragma once

#include "Value.h"

#include <optional>
#include <vector>
#include <set>

namespace ls
{
    namespace json
    {
        template <typename...>
        struct Reader;

        template <typename T>
        inline T fromJson(const Value& val)
        {
            return Reader<T>::fromJson(val);
        }

        template <typename T, typename U>
        inline T fromJson(const Value& val, U&& defaultValue)
        {
            if (val.exists())
            {
                return fromJson<T>(val);
            }
            else
            {
                return T(std::forward<U>(defaultValue));
            }
        }

        template <>
        struct Reader<std::string>
        {
            static std::string fromJson(const Value& val)
            {
                return val.getString();
            }
        };

        template <>
        struct Reader<bool>
        {
            static bool fromJson(const Value& val)
            {
                return val.getBool();
            }
        };

        template <typename T>
        struct Reader<T>
        {
            static T fromJson(const Value& val)
            {
                if constexpr (std::is_integral_v<T>)
                {
                    return static_cast<T>(val.getInt());
                }
                else if constexpr (std::is_floating_point_v<T>)
                {
                    return static_cast<T>(val.getDouble());
                }
                else
                {
                    static_assert(false, "This reader only supports integral and floating point types");
                }
            }
        };

        template <typename T>
        struct Reader<std::optional<T>>
        {
            static std::optional<T> fromJson(const Value& val)
            {
                if (val.exists())
                {
                    return json::fromJson<T>(val);
                }
                else
                {
                    return std::nullopt;
                }
            }
        };

        template <typename T>
        struct Reader<std::vector<T>>
        {
            static std::vector<T> fromJson(const Value& val)
            {
                std::vector<T> vec;

                const auto& arr = val.getArray();
                const size_t size = arr.size();
                vec.reserve(size);

                for (const auto& el : arr)
                {
                    vec.emplace_back(json::fromJson<T>(el));
                }

                return vec;
            }
        };


        template <typename T>
        struct Reader<std::set<T>>
        {
            static std::set<T> fromJson(const Value& val)
            {
                std::set<T> set;

                const auto& arr = val.getArray();

                for (const auto& el : arr)
                {
                    set.emplace(json::fromJson<T>(el));
                }

                return set;
            }
        };
    }
}
