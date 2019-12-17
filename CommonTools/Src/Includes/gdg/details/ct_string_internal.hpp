namespace details_
{
template<typename CharT, typename T, T value>
constexpr auto int2ctstr()
{
    constexpr auto reqsz{ std::numeric_limits<std::decay_t<T>>::digits10 };
    CharT sz[reqsz + 1]{ 0 };

    T d{ value };
    for (std::size_t i{ reqsz - 1}; d >= 10; --i)
    {
        T r{ d % 10 };
        if constexpr (std::is_same_v<CharT, char>)
            sz[i] = static_cast<CharT>('0' + r);
        else
            sz[i] = static_cast<CharT>(L'0' + r);

        d = d / 10;
    }

    if constexpr (std::is_same_v<CharT, char>)
        *sz = static_cast<CharT>('0' + d);
    else
        *sz = static_cast<CharT>(L'0' + d);

    return make_ct_string(sz);
}
} // details_
