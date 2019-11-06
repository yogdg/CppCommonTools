#ifndef GDG_COMMON_TOOLS_DETAILS_CTTI_INTERNAL_HPP_
#define GDG_COMMON_TOOLS_DETAILS_CTTI_INTERNAL_HPP_

template<std::size_t N>
std::string bad_stream_data::setup_message(std::string_view message, const std::array<std::string_view, N> &expected_values)
{
    if constexpr (N > 0)
    {
        std::stringstream ss;

        ss << message << " Expected values: ";
        std::copy(expected_values.begin(), expected_values.end(), std::ostream_iterator<std::string_view>(ss, ","));

        return ss.str();
    }

    return message;
}

namespace details_
{
template<typename T>
constexpr std::string_view get_name_of()
{
    constexpr std::string_view function{ __FUNCSIG__ };

    size_t cnt{ 0 };
    auto looking{ false };
    auto from{ function.rend() }, to{ from };
    for (auto it{ function.rbegin() }; it != function.rend(); ++it)
    {
        if (*it == '<')
        {
            if (!--cnt)
                from = it;
        }
        else if (*it == '>' && !cnt++)
        {
            looking = true;
            to = it;
        }

        if (looking && !cnt)
            return std::string_view
        {
            function.data() + std::distance(from, function.rend())
            , static_cast<std::size_t>(std::distance(to, from)) - 1
        };
    }

    throw std::invalid_argument{ "__FUNCSIG__ unparseable" };
}

template<std::size_t I, std::size_t N, typename EnumT>
EnumT unroll_array_and_compare(const std::string &rv, const std::array<std::string_view, N> &svalues, const std::array<EnumT, N> &values)
{
    if constexpr (I == N)
    {
        (void)values;
        using namespace gdg::string_utils::literals;
        constexpr auto name{ get_name_of<EnumT>() };
        THROW(gdg::bad_stream_data, std::string{ "Invalid "_ms << name << " value [" << rv << "]." }, svalues);
    }
    else
    {
        if (rv == svalues[I])
            return values[I];

        return unroll_array_and_compare<I + 1, N>(rv, svalues, values);
    }
}

template<std::size_t I, std::size_t N, typename EnumT>
std::ostream& unroll_array_and_stream(std::ostream &stream, EnumT value, const std::array<std::string_view, N> &svalues, const std::array<EnumT, N> &values)
{
    if constexpr (I == N)
    {
        (void)svalues;
        (void)values;
        stream << get_name_of<EnumT>() << " [" << value << ']';
        return stream;
    }
    else
    {
        if (value != values[I])
            return unroll_array_and_stream<I + 1, N, EnumT>(stream, value, svalues, values);

        stream << svalues[I];
        return stream;
    }
}
} // details_

#endif /*GDG_COMMON_TOOLS_DETAILS_CTTI_INTERNAL_HPP_*/
