#ifndef GDG_COMMON_TOOLS_TINY_RANGES_HPP_
#define GDG_COMMON_TOOLS_TINY_RANGES_HPP_

#include <gdg/details/tiny_ranges_internal.hpp>

namespace gdg::ranges
{
///
///
///
template<typename T, template<class, class...> class C, class... Args>
constexpr auto operator | (const C<T, Args...> &vec, std::size_t offset)
{
    return details_gdg_::container_view<decltype(vec)>{ vec, offset };
}

///
///
///
template<typename T, template<class, class...> class C, class... Args>
constexpr auto operator | (C<T, Args...> &vec, std::size_t offset)
{
    return details_gdg_::container_view<decltype(vec)>{ vec, offset };
}

///
///
///
template<typename prv_link_t, typename Clause>
constexpr auto operator | (prv_link_t &&prv_link, where<Clause> c)
{
    return details_gdg_::where_container_<decltype(prv_link.begin()), Clause>{ prv_link.begin(), prv_link.end(), std::move(c.clause_) };
}

///
///
///
template<typename prv_link_t, typename SelectT>
constexpr auto operator | (prv_link_t &&prv_link, select<SelectT> g)
{
    return details_gdg_::select_container_<decltype(prv_link.begin()), SelectT>{ prv_link.begin(), prv_link.end(), std::move(g.gen_fx_) };
}

///
///
///
template<typename prv_link_t, typename OnEachT>
constexpr auto operator | (prv_link_t &&prv_link, for_each<OnEachT> f)
{
    return details_gdg_::for_each_container_<decltype(prv_link.begin()), OnEachT>{ prv_link.begin(), prv_link.end(), std::move(f.on_each_fx_) }.apply();
}

} // gdg::ranges

#endif /*GDG_COMMON_TOOLS_TINY_RANGES_HPP_*/
