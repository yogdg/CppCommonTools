#ifndef GDG_COMMON_TOOLS_SOURCE_LOCATION_HPP_
#define GDG_COMMON_TOOLS_SOURCE_LOCATION_HPP_

#include <string_view>
#include <iostream>
#include <limits>
#include <gdg/pre_processor.hpp>

namespace gdg
{
template<typename CharT, typename = std::enable_if_t<std::is_same_v<CharT, char> || std::is_same_v<CharT, wchar_t>>>
constexpr std::basic_string_view<CharT> extract_folders_from_path(std::basic_string_view<CharT> File, std::size_t count);


///
///
///
#ifndef SOURCELOCEATION_FILE_FOLDERS_COUNT
#define SOURCELOCEATION_FILE_FOLDERS_COUNT std::numeric_limits<std::size_t>::max()
#endif


///
///
///
template<typename CharT, typename = std::enable_if_t<std::is_same_v<CharT, char> || std::is_same_v<CharT, wchar_t>>>
struct basic_source_location
{
public:
    ///
    ///
    ///
    constexpr basic_source_location() noexcept
        : m_Line{0}
    { }

    ///
    ///
    ///
    constexpr basic_source_location(std::basic_string_view<CharT> file, std::size_t line, std::basic_string_view<CharT> function) noexcept
        : m_Line{ line }
        , m_File{ std::move(extract_folders_from_path(MOVE(file), SOURCELOCEATION_FILE_FOLDERS_COUNT)) }
        , m_Function{ MOVE(function) }
    { }

public:
    ///
    ///
    ///
    constexpr auto line() const noexcept
    {
        return m_Line;
    }
    ///
    ///
    ///
    constexpr auto file() const noexcept
    {
        return m_File;
    }
    ///
    ///
    ///
    constexpr auto function() const noexcept
    {
        return m_Function;
    }

public:
    ///
    ///
    ///
    constexpr bool operator== (const basic_source_location &sl) const noexcept
    {
        return std::tie(sl.m_File, sl.m_Line, sl.m_Function) == std::tie(m_File, m_Line, m_Function);
    }
    ///
    ///
    ///
    constexpr bool operator != (const basic_source_location &sl) const noexcept
    {
        return !(*this == sl);
    }

private:
    std::size_t m_Line;
    std::basic_string_view<CharT> m_File;
    std::basic_string_view<CharT> m_Function;
};


///
///
///
using source_location = basic_source_location<char>;


///
///
///
using wsource_location = basic_source_location<wchar_t>;


///
///
///
#define current_source_location() ::gdg::source_location{ __FILE__, __LINE__CONSTEXPR, __FUNCTION__ }
#define wcurrent_source_location() ::gdg::wsource_location{ __FILEW__, __LINE__CONSTEXPR, __FUNCTIONW__ }


///
///
///
template<typename CharT>
std::basic_ostream<CharT>& operator << (std::basic_ostream<CharT> &s, basic_source_location<CharT> sl)
{
    if constexpr (std::is_same_v<CharT, char>)
        s << '[' << sl.file() << ':' << sl.line() << " (" << sl.function() << ")]";
    else
        s << L'[' << sl.file() << L':' << sl.line() << L" (" << sl.function() << L")]";

    return s;
}


///
///
///
template<typename CharT, typename>
constexpr std::basic_string_view<CharT> extract_folders_from_path(std::basic_string_view<CharT> File, std::size_t count)
{
    if (count >= File.size())
        return File;

    std::size_t i{ 0 };
    for (auto it{ File.crbegin() }; it != File.crend(); ++it)
    {
        if constexpr (std::is_same_v<CharT, char>)
        {
            if (*it == '\\' || *it == '/')
                i++;
        }
        else
        {
            if (*it == L'\\' || *it == L'/')
                i++;
        }

        if (i == count + 1)
        {
            const auto off{ static_cast<std::size_t>(std::distance(it + 1, File.crend())) };
            return std::basic_string_view<CharT>{ off + File.data() + 1, File.size() - off - 1 };
        }

    }

    return File;
}

} // gdg

#endif /*GDG_COMMON_TOOLS_SOURCE_LOCATION_HPP_*/
