#ifndef GDG_COMMON_TOOLS_DETAILS_META_TOOLS_INTERNAL_HPP_
#define GDG_COMMON_TOOLS_DETAILS_META_TOOLS_INTERNAL_HPP_

#include <utility>
#include <tuple>
#include <type_traits>
#include <optional>
#include <vector>
#include <array>
#include <memory>

#include <gdg/pre_processor.hpp>

namespace gdg::meta_tools::details_
{
template<std::size_t Idx, typename OnEachT, typename TupleT>
constexpr void for_each_(TupleT &&t, OnEachT &&on_each)
{
    FWD(on_each)(std::get<Idx>(FWD(t)), Idx);

    if constexpr (Idx != 0)
        for_each_<Idx - 1>(FWD(t), FWD(on_each));
}

// https://jguegant.github.io/blogs/tech/sfinae-introduction.html
template <typename UnnamedType>
struct is_valid_container
{
private:
    template <typename... Params> constexpr auto test_validity(int) -> decltype(std::declval<UnnamedType>()(std::declval<Params>()...), std::true_type())
    {
        return std::true_type();
    }

    template <typename... Params> constexpr std::false_type test_validity(...)
    {
        return std::false_type();
    }

public:
    template <typename... Params> constexpr auto operator()(Params&& ...)
    {
        return test_validity<Params...>(int());
    }
};

} // gdg::meta_tools::details_

#endif /*GDG_COMMON_TOOLS_DETAILS_META_TOOLS_INTERNAL_HPP_*/
