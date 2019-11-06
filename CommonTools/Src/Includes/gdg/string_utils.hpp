#ifndef GDG_COMMON_TOOLS_STRING_UTILS_HPP_
#define GDG_COMMON_TOOLS_STRING_UTILS_HPP_

#include <gdg/Details/string_utils_internal.hpp>

namespace gdg::string_utils
{
///
///
///
template<typename CharT>
auto make_string()
{
    return details_::basic_string_maker<CharT>{};
}
///
///
///
template<typename CharT, std::size_t N>
auto make_string(CharT (&Src)[N])
{
    return details_::basic_string_maker{ std::basic_string_view{ Src, N} };
}
///
///
///
template<typename CharT>
auto make_string(std::basic_string_view<CharT> Src)
{
    return details_::basic_string_maker{ Src };
}


///
///
///
template<typename CharT>
std::basic_string<CharT>& ltrim(std::basic_string<CharT> &s)
{
    return details_::ltrim(s);
}
///
///
///
template<typename CharT>
std::basic_string<CharT> ltrim(std::basic_string<CharT> &&s)
{
    return ltrim(s);
}
///
///
///
template<typename CharT>
std::basic_string<CharT> ltrim(const std::basic_string<CharT> &s)
{
    return ltrim(std::basic_string<CharT>{ s });
}
///
///
///
template<typename CharT>
std::basic_string_view<CharT> ltrim(const std::basic_string_view<CharT> &s)
{
    return details_::ltrim(s);
}


///
///
///
template<typename CharT>
std::basic_string<CharT>& rtrim(std::basic_string<CharT> &s)
{
    return details_::rtrim(s);
}
///
///
///
template<typename CharT>
std::basic_string<CharT> rtrim(std::basic_string<CharT> &&s)
{
    return rtrim(s);
}
///
///
///
template<typename CharT>
std::basic_string<CharT> rtrim(const std::basic_string<CharT> &s)
{
    return rtrim(std::basic_string<CharT>{ s });
}
///
///
///
template<typename CharT>
std::basic_string_view<CharT> rtrim(const std::basic_string_view<CharT> &s)
{
    return details_::rtrim(s);
}


///
///
///
template<typename CharT>
std::basic_string<CharT>& trim(std::basic_string<CharT> &s)
{
    return ltrim(rtrim(s));
}
///
///
///
template<typename CharT>
std::basic_string<CharT> trim(std::basic_string<CharT> &&s)
{
    return trim(s);
}
///
///
///
template<typename CharT>
std::basic_string<CharT> trim(const std::basic_string<CharT> &s)
{
    return trim(std::basic_string<CharT>{ s });
}
///
///
///
template<typename CharT>
std::basic_string_view<CharT> trim(const std::basic_string_view<CharT> &s)
{
    return ltrim(rtrim(s));
}


///
///
///
template<typename CharT, typename = std::enable_if_t<std::is_same_v<CharT, char> || std::is_same_v<CharT, wchar_t>>>
std::basic_string<CharT>& to_upper(std::basic_string<CharT> &s)
{
    return details_::to_upper(s);
}
///
///
///
template<typename CharT, typename = std::enable_if_t<std::is_same_v<CharT, char> || std::is_same_v<CharT, wchar_t>>>
std::basic_string<CharT> to_upper(std::basic_string<CharT> &&s)
{
    return to_upper(s);
}
///
///
///
template<typename CharT, typename = std::enable_if_t<std::is_same_v<CharT, char> || std::is_same_v<CharT, wchar_t>>>
std::basic_string<CharT> to_upper(const std::basic_string<CharT> &s)
{
    return to_upper(std::basic_string<CharT>{ s });
}



///
///
///
template<typename CharT, typename = std::enable_if_t<std::is_same_v<CharT, char> || std::is_same_v<CharT, wchar_t>>>
std::basic_string<CharT>& to_lower(std::basic_string<CharT> &s)
{
    return details_::to_lower(s);
}
///
///
///
template<typename CharT, typename = std::enable_if_t<std::is_same_v<CharT, char> || std::is_same_v<CharT, wchar_t>>>
std::basic_string<CharT> to_lower(std::basic_string<CharT> &&s)
{
    return to_lower(s);
}
///
///
///
template<typename CharT, typename = std::enable_if_t<std::is_same_v<CharT, char> || std::is_same_v<CharT, wchar_t>>>
std::basic_string<CharT> to_lower(const std::basic_string<CharT> &s)
{
    return to_lower(std::basic_string<CharT>{ s });
}


///
///
///
template<typename CharT>
std::basic_string<CharT> replace_all(std::basic_string<CharT> &&string, std::basic_string_view<CharT> lookfor, std::basic_string_view<CharT> replacewith)
{
    return details_::replace_all(std::move(string), lookfor, replacewith);
}
///
///
///
template<typename CharT>
std::basic_string<CharT>& replace_all(std::basic_string<CharT> &string, std::basic_string_view<CharT> lookfor, std::basic_string_view<CharT> replacewith)
{
    return (string = replace_all(std::move(string), lookfor, replacewith));
}
///
///
///
template<typename CharT>
std::basic_string<CharT> replace_all(const std::basic_string<CharT> &string, std::basic_string_view<CharT> lookfor, std::basic_string_view<CharT> replacewith)
{
    return replace_all(std::basic_string<CharT>{ string }, lookfor, replacewith);
}


///
///
///
template<typename CharT>
std::vector<std::basic_string_view<CharT>> tokenize(std::basic_string_view<CharT> source, CharT token)
{
    return details_::tokenize(std::move(source), token);
}


///
///
///
template<typename CharT, typename = std::enable_if_t<std::is_same_v<CharT, char> || std::is_same_v<CharT, wchar_t>>>
std::basic_string<CharT>& adjust_size(std::basic_string<CharT> &src, std::size_t len = std::basic_string<CharT>::npos)
{
    return details_::adjust_size(src, len);
}
///
///
///
template<typename CharT, typename = std::enable_if_t<std::is_same_v<CharT, char> || std::is_same_v<CharT, wchar_t>>>
std::basic_string<CharT> adjust_size(std::basic_string<CharT> &&src, std::size_t len = std::basic_string<CharT>::npos)
{
    return adjust_size(src, len);
}
///
///
///
template<typename CharT, typename = std::enable_if_t<std::is_same_v<CharT, char> || std::is_same_v<CharT, wchar_t>>>
std::basic_string<CharT> adjust_size(const std::basic_string<CharT> &src, std::size_t len = std::basic_string<CharT>::npos)
{
    return adjust_size(src.data(), len);
}


///
///
///
namespace literals
{
///
///
///
auto operator"" _ms(const char *src, std::size_t srclen)
{
    return make_string(std::string_view{ src, srclen });
}
///
///
///
auto operator"" _ms(const wchar_t *src, std::size_t srclen)
{
    return make_string(std::wstring_view{ src, srclen });
}

} // literals
} // gdg::string_utils

#endif /*GDG_COMMON_TOOLS_STRING_UTILS_HPP_*/
