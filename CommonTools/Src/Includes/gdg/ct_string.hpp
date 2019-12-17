#ifndef GDG_COMMON_TOOLS_CT_STRING_HPP_
#define GDG_COMMON_TOOLS_CT_STRING_HPP_

#include <exception>
#include <string_view>
#include <algorithm>
#include <execution>
#include <type_traits>
#include <iostream>

namespace gdg
{
template<typename CharT, std::size_t Capacity>
class basic_ct_string
{
// Ctors
public:
    constexpr basic_ct_string()
    {}

    constexpr basic_ct_string(std::basic_string_view<CharT> s)
    {
        if (s.length() >= capacity_)
            throw std::invalid_argument{ "Source string invalid length" };

        copy(s);
    }

// Element access
public:
    constexpr CharT& at(std::size_t n)
    {
        if (n > length_)
            throw std::out_of_range{ "Index out of bounds" };

        return buffer_[n];
    }

    constexpr const CharT& at(std::size_t n) const
    {
        if (n > length_)
            throw std::out_of_range{ "Index out of bounds" };

        return buffer_[n];
    }

    constexpr CharT& operator[](std::size_t n)
    {
        return buffer_[n];
    }

    constexpr const CharT& operator[](std::size_t n) const
    {
        return buffer_[n];
    }

    constexpr CharT& front()
    {
        return buffer_[0];
    }

    constexpr const CharT& front() const
    {
        return buffer_[0];
    }

    constexpr CharT& back()
    {
        return buffer_[length_ - 1];
    }

    constexpr const CharT& back() const
    {
        return buffer_[length_ - 1];
    }

    constexpr CharT* data()
    {
        return buffer_;
    }

    constexpr const CharT* data() const
    {
        return buffer_;
    }

    constexpr const CharT* c_str() const
    {
        return buffer_;
    }

    constexpr operator std::basic_string_view<CharT>() const
    {
        return buffer_;
    }


// Iterators
public:
    constexpr CharT* begin()
    {
        return buffer_;
    }

    constexpr CharT* end()
    {
        return buffer_ + length_;
    }

    constexpr const CharT* cbegin() const
    {
        return buffer_;
    }

    constexpr const CharT* cend() const
    {
        return buffer_ + length_;
    }

// Capacity
public:
    constexpr bool empty() const
    {
        return !length_;
    }

    constexpr std::size_t length() const
    {
        return length_;
    }

    constexpr std::size_t size() const
    {
        return length_;
    }

    constexpr std::size_t capacity() const
    {
        return capacity_;
    }

// Operations
public:
    constexpr basic_ct_string<CharT, Capacity>& append(std::string_view s)
    {
        if (s.length() + length() >= Capacity)
            throw std::invalid_argument{ "Source string invalid length" };

        copy(s);

        return *this;
    }

private:
    constexpr void copy(std::string_view s)
    {
        const auto bgn{ s.cbegin() }, end{ s.cend() };
        auto dst{ buffer_ + length_ };

        for (auto src{ bgn }; src != end;)
            *dst++ = *src++;

        *dst = 0;
        length_ += std::distance(bgn, end);
    }

private:
    CharT buffer_[Capacity]{ 0 };
    std::size_t capacity_{ Capacity };
    std::size_t length_{ 0 };
};

template<std::size_t N>
using ct_string = basic_ct_string<char, N>;

template<std::size_t N>
using ct_wstring = basic_ct_string<wchar_t, N>;

template<typename CharT, std::size_t N>
std::basic_ostream<CharT>& operator << (std::basic_ostream<CharT> &s, basic_ct_string<CharT, N> o)
{
    s << static_cast<std::basic_string_view<CharT>>(o);
    return s;
}

template<typename CharT, std::size_t N>
constexpr auto make_ct_string(const CharT(&s)[N])
{
    return basic_ct_string<CharT, N>{s};
}

#include <gdg/Details/ct_string_internal.hpp>

template<typename CharT, std::size_t LN, std::size_t RN>
constexpr auto operator << (const basic_ct_string<CharT, LN> &l, const CharT(&r)[RN])
{
    return basic_ct_string<CharT, LN + RN - 1>{ l }.append(r);
}

template<typename CharT, std::size_t N>
constexpr auto operator << (const basic_ct_string<CharT, N> &l, std::false_type)
{
    constexpr std::string_view val{ "false" };
    return basic_ct_string<CharT, N + val.length()>{ l }.append(val);
}

template<typename CharT, std::size_t N>
constexpr auto operator << (const basic_ct_string<CharT, N> &l, std::true_type)
{
    constexpr std::string_view val{ "true" };
    return basic_ct_string<CharT, N + val.length()>{ l }.append(val);
}

template<typename CharT, std::size_t N, typename ParamType, ParamType ParamValue>
constexpr auto operator << (const basic_ct_string<CharT, N> &l, std::integral_constant<ParamType, ParamValue>)
{
    return basic_ct_string<CharT, N + std::numeric_limits<std::decay_t<ParamType>>::digits10>{ l }.append(details_::int2ctstr<CharT, ParamType, ParamValue>());
}
} // gdg

#endif /*GDG_COMMON_TOOLS_CT_STRING_HPP_*/
