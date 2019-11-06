#ifndef GDG_COMMON_TOOLS_LEXICAL_CAST_HPP_
#define GDG_COMMON_TOOLS_LEXICAL_CAST_HPP_

#include <sstream>
#include <type_traits>
#include <string_view>
#include <iomanip>

#include <gdg/Exception.hpp>
#include <gdg/source_location.hpp>
#include <gdg/meta_tools.hpp>

namespace gdg
{
///
///
///
class lexical_cast_exception : public basic_exception
{
public:
    lexical_cast_exception(source_location tp, std::string_view msg)
        : basic_exception{ tp, msg }
    {}
};

#include <gdg/Details/lexical_cast_internal.hpp>


///
///
///
template<typename D, typename S, typename ImbueT = void>
D lexical_cast(const S &s)
{
    using S_ = std::remove_reference_t<S>;
    using D_ = D;

    if constexpr (std::is_convertible_v<D, S>)
        return static_cast<D>(s);
    else
        return details_::converter<ImbueT, D_, S_>::convert(s);
}

///
///
///
template<typename D, typename ImbueT = void>
D lexical_cast(const char *s)
{
    using D_ = D;

    if constexpr (std::is_convertible_v<D, decltype(s)>)
        return static_cast<D>(s);
    else
        return lexical_cast<D, std::string_view, ImbueT>({ s });
}

///
///
///
template<typename D, typename ImbueT = void>
constexpr D lexical_cast(const wchar_t *s)
{
    if constexpr (std::is_convertible_v<D, decltype(s)>)
        return static_cast<D>(s);
    else
        return lexical_cast<D, std::string_view, ImbueT>({ s });
}

} // gdg

#endif /*GDG_COMMON_TOOLS_LEXICAL_CAST_HPP_*/
