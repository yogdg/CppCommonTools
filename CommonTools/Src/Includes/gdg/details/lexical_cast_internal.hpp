#ifndef GDG_COMMON_TOOLS_DETAILS_LEXICAL_CAST_INTERNAL_HPP_
#define GDG_COMMON_TOOLS_DETAILS_LEXICAL_CAST_INTERNAL_HPP_

template<typename D, typename S, typename ImbueT>
D lexical_cast(const S& s);

template<typename D, typename ImbueT>
D lexical_cast(const char* s);

template<typename D, typename ImbueT>
constexpr D lexical_cast(const wchar_t* s);

namespace details_
{

template<typename I, typename C, typename D, typename S>
D do_convert (const S &s)
{
    std::basic_stringstream<C> ss;

    if constexpr (!std::is_same_v<I, void>)
    {
        auto has_manipulator{ gdg::meta_tools::is_valid([&ss](auto &&x) -> decltype(I::set_iomanipulators(ss)) {}) };

        if constexpr (decltype(has_manipulator(std::declval<I>()))::value)
            I::set_iomanipulators(ss);
        else
            ss << std::fixed << std::boolalpha << s;

        ss.imbue(std::locale{ std::locale{}, new I });
    }
    else
        ss << std::fixed << std::boolalpha << s;

    if constexpr (std::is_same_v<D, std::basic_string<C>>)
        return ss.str();
    else
    {
        if (D d; ss >> d)
            return d;

        THROW(lexical_cast_exception, "Couldn't cast to output type");
    }
};

template<typename I, typename D, typename S>
struct converter
{
    static D convert(const S &s)
    {
        return do_convert<I, char, D, S>(s);
    }
};

template<typename I, typename D>
struct converter<I, D, std::wstring_view>
{
    static D convert(const std::wstring_view &s)
    {
        return do_convert<I, wchar_t, D, std::wstring_view>(s);
    }
};

template<typename I, typename S>
struct converter<I, std::wstring, S>
{
    static std::wstring convert(const S &s)
    {
        return do_convert<I, wchar_t, std::wstring, S>(s);
    }
};

template<typename I>
struct converter<I, std::string, std::wstring_view>
{
    static std::string convert(const std::wstring_view &s)
    {
        auto cnt{ s.length() + 1 };
        std::string r;
        r.resize(s.length());
        if (wcstombs_s(&cnt, r.data(), cnt, s.data(), cnt - 1))
            THROW(lexical_cast_exception, std::error_code(errno, std::generic_category()).message());
        return r;
    }
};

template<typename I>
struct converter<I, std::string, std::wstring>
{
    static std::string convert(const std::wstring &s)
    {
        return converter<I, std::string, std::wstring_view>::convert(s);
    }
};

template<typename I>
struct converter<I, std::wstring, std::string_view>
{
    static std::wstring convert(const std::string_view &s)
    {
        auto cnt{ s.length() + 1 };
        std::wstring r;
        r.resize(s.length());
        if (mbstowcs_s(&cnt, r.data(), cnt, s.data(), cnt - 1))
            THROW(lexical_cast_exception, std::error_code(errno, std::generic_category()).message());
        return r;
    }
};

template<typename I>
struct converter<I, std::wstring, std::string>
{
    static std::wstring convert(const std::string &s)
    {
        return converter<I, std::wstring, std::string_view>::convert(s);
    }
};

template<typename I, typename D>
struct converter<I, D, const char*>
{
    static D convert(const char *s)
    {
        return gdg::lexical_cast<D, std::string_view, I>(s);
    }
};

template<typename I, typename D>
struct converter<I, D, const wchar_t*>
{
    static D convert(const wchar_t *s)
    {
        return gdg::lexical_cast<D, std::wstring_view, I>(s);
    }
};

} // details_

#endif /*GDG_COMMON_TOOLS_DETAILS_LEXICAL_CAST_INTERNAL_HPP_*/
