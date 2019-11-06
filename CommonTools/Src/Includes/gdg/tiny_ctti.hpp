#ifndef GDG_COMMON_TOOLS_TINY_CTTI_HPP_
#define GDG_COMMON_TOOLS_TINY_CTTI_HPP_

#include <string_view>
#include <string>
#include <sstream>
#include <iostream>
#include <gdg/pre_processor.hpp>
#include <gdg/string_utils.hpp>
#include <gdg/exception.hpp>
#include <gdg/source_location.hpp>
#include <gdg/string_utils.hpp>

namespace gdg::ctti
{
///
///
///
class bad_stream_data : public gdg::basic_exception
{
public:
    ///
    ///
    ///
    bad_stream_data(source_location throwpt, std::string_view msg, source_location callpt)
        : basic_exception{ MOVE(throwpt), MOVE(msg), callpt }
    { }

    ///
    ///
    ///
    template<std::size_t N>
    bad_stream_data(source_location throwpt, std::string_view message, const std::array<std::string_view, N> &expected_values, source_location callpt)
        : basic_exception{ MOVE(throwpt), setup_message(MOVE(message), expected_values), MOVE(callpt) }
    {}

private:
    template<std::size_t N>
    std::string setup_message(std::string_view message, const std::array<std::string_view, N> &expected_values);
};

#include <gdg/Details/tiny_ctti_internal.hpp>

///
///
///
template<typename T>
static constexpr auto name_of{ details_::get_name_of<T>() };

///
///
///
template<typename DescriptorT, typename EnumT>
std::istream& stream_to_enum(std::istream& stream, EnumT &out)
{
    static_assert(DescriptorT::strings.size() == DescriptorT::values.size());

    std::string rv;
    stream >> rv;
    out = details_::unroll_array_and_compare<0, DescriptorT::strings.size()>(rv, DescriptorT::strings, DescriptorT::values);

    return stream;
}

template<typename DescriptorT, typename EnumT>
std::ostream& enum_to_stream(std::ostream &stream, EnumT value)
{
    static_assert(DescriptorT::strings.size() == DescriptorT::values.size());

    return details_::unroll_array_and_stream<0, DescriptorT::strings.size()>(stream, value, DescriptorT::strings, DescriptorT::values);
}

} // gdg::ctti

#endif /*GDG_COMMON_TOOLS_TINY_CTTI_HPP_*/
