#pragma once

#include <string>
#include <fstream>
#include <type_traits>

namespace ls
{
    namespace detail
    {
        template <typename...>
        struct Overload;

        template <typename FuncT>
        struct Overload<FuncT> : FuncT
        {
            using FuncType = std::remove_cv_t<std::remove_reference_t<FuncT>>;
            using FuncType::operator();

            Overload(FuncT&& f) noexcept : 
                FuncT(std::forward<FuncT>(f))
            {
            }
        };

        template <typename FuncHeadT, typename... FuncTailTs>
        struct Overload<FuncHeadT, FuncTailTs...> : FuncHeadT, Overload<FuncTailTs...>
        {
            using RestType = Overload<FuncTailTs...>;
            using FuncHeadType = std::remove_cv_t<std::remove_reference_t<FuncHeadT>>;
            using RestType::operator();
            using FuncHeadType::operator();

            Overload(FuncHeadT&& f, FuncTailTs&&... fs) noexcept : 
                FuncHeadT(std::forward<FuncHeadT>(f)),
                RestType(std::forward<FuncTailTs>(fs)...)
            {
            }
        };
    }

    inline std::string readFile(const char* path)
    {
        std::fstream file(path, std::ios::in);
        if (!file.is_open())
        {
            return "";
        }

        std::string contents;
        file.seekg(0, std::ios::end);
        contents.resize(static_cast<size_t>(file.tellg()));
        file.seekg(0, std::ios::beg);
        file.read(contents.data(), contents.size());
        file.close();

        return contents;
    }

    template <typename... FuncTs>
    auto overload(FuncTs&&... funcs)
    {
        return detail::Overload<FuncTs...>(std::forward<FuncTs>(funcs)...);
    }

    template<unsigned P>
    struct Priority : Priority<P - 1> {};

    template<>
    struct Priority<0> {};
}
