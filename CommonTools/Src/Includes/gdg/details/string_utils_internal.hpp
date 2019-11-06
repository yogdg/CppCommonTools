#ifndef GDG_COMMON_TOOLS_DETAILS_STRING_UTILS_INTERNAL_HPP_
#define GDG_COMMON_TOOLS_DETAILS_STRING_UTILS_INTERNAL_HPP_

#include <string_view>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <regex>
#include <vector>
#include <cctype>
#include <cstring>
#include <gdg/pre_processor.hpp>

namespace gdg::string_utils::details_
{
template<typename CharT>
struct basic_string_maker
{
    basic_string_maker(const basic_string_maker&) = delete;
    basic_string_maker& operator=(const basic_string_maker&) = delete;
    basic_string_maker(basic_string_maker&&) = default;
    basic_string_maker& operator=(basic_string_maker&&) = delete;

    explicit basic_string_maker(std::basic_string_view<CharT> src)
    {
        m_Stream << src;
    }

    explicit basic_string_maker()
        : basic_string_maker{ "" }
    {}


    operator std::basic_string<CharT> () const
    {
        return m_Stream.str();
    }

    template<typename T>
    const basic_string_maker<CharT>& operator << (T &&t) const
    {
        m_Stream << FWD(t);
        return *this;
    }

private:
    mutable std::basic_stringstream<CharT> m_Stream;
};

template<typename CharT>
std::basic_string<CharT>& ltrim(std::basic_string<CharT> &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch)
    {
        return !std::isspace(ch) && ch != '\0';
    }));

    return s;
}

template<typename CharT>
std::basic_string<CharT>& rtrim(std::basic_string<CharT> &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch)
    {
        return !std::isspace(ch) && ch != '\0';
    }).base(), s.end());

    return s;
}

template<typename CharT>
std::basic_string_view<CharT> ltrim(const std::basic_string_view<CharT> &s)
{
    const auto it
    {
        std::find_if(s.begin(), s.end(), [](int ch)
        {
            return !std::isspace(ch) && ch != '\0';
        })
    };

    return
    {
        s.data() + std::distance(s.begin(), it)
        , static_cast<size_t>(std::distance(it, s.end()))
    };
}

template<typename CharT>
std::basic_string_view<CharT> rtrim(const std::basic_string_view<CharT> &s)
{
    const auto it
    {
        std::find_if(s.rbegin(), s.rend(), [](int ch)
        {
            return !std::isspace(ch) && ch != '\0';
        })
    };

    return
    {
        s.data()
        , static_cast<size_t>(std::distance(it, s.rend()))
    };
}

inline std::string& to_upper(std::string &s)
{
    std::transform(s.begin(), s.end(), s.begin(), [](char c) {return static_cast<char>(::toupper(c)); });
    return s;
}
inline std::wstring& to_upper(std::wstring &s)
{
    std::transform(s.begin(), s.end(), s.begin(), ::towupper);
    return s;
}


inline std::string& to_lower(std::string &s)
{
    std::transform(s.begin(), s.end(), s.begin(), [](char c) {return static_cast<char>(::tolower(c)); });
    return s;
}
inline std::wstring& to_lower(std::wstring &s)
{
    std::transform(s.begin(), s.end(), s.begin(), ::towlower);
    return s;
}


template<typename CharT>
std::basic_string<CharT> replace_all(std::basic_string<CharT> string, std::basic_string_view<CharT> lookfor, std::basic_string_view<CharT> replacewith)
{
    return std::regex_replace(string, std::basic_regex<CharT>(lookfor.data()), replacewith.data());
}


template<typename CharT>
std::vector<std::basic_string_view<CharT>> tokenize(std::basic_string_view<CharT> src, CharT separator)
{
    using ret_t = std::basic_string_view<CharT>;

    std::vector<ret_t> ret;
    
    std::size_t bgn{ 0 }, end{ 0 };
    for (; (end = src.find(separator, bgn)) != ret_t::npos; bgn = end + 1)
        ret.emplace_back(src.data() + bgn, end - bgn);

    if (const auto ln{ src.length() }; bgn < ln)
        ret.emplace_back(src.data() + bgn, ln - bgn);

    return ret;
}

inline std::string& adjust_size(std::string &src, std::size_t len)
{
    src.resize(len != std::string::npos ? len : std::strlen(src.data()));
    return src;
}
inline std::wstring& adjust_size(std::wstring &src, std::size_t len)
{
    src.resize(len != std::wstring::npos ? len : std::wcslen(src.data()));
    return src;
}


} // gdg::string_utils::details_

#endif /*GDG_COMMON_TOOLS_DETAILS_STRING_UTILS_INTERNAL_HPP_*/
