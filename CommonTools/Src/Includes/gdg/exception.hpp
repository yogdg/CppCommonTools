#ifndef GDG_COMMON_TOOLS_EXCEPTION_HPP_
#define GDG_COMMON_TOOLS_EXCEPTION_HPP_

#include <stdexcept>
#include <string_view>
#include <string>
#include <memory>
#include <sstream>
#include <gdg/pre_processor.hpp>
#include <gdg/source_location.hpp>

namespace gdg
{
///
///
///
class basic_exception : public std::runtime_error
{
public:
    ///
    ///
    ///
    basic_exception(const source_location &throwpt, std::string_view msg)
        : basic_exception{ throwpt, MOVE(msg), throwpt }
    {}

    ///
    ///
    ///
    basic_exception(source_location throwpt, std::string_view msg, source_location callpt)
        : std::runtime_error{ msg.data() }
        , throw_point{ MOVE(throwpt) }
        , call_point{ MOVE(callpt) }
        , m_What{ BuildWhatString() }
    {}

    ///
    ///
    ///
    virtual ~basic_exception() noexcept
    {}

    ///
    ///
    ///
    virtual const char* what() const noexcept override
    {
        return m_What.data();
    }

    ///
    ///
    ///
    const source_location throw_point;
    ///
    ///
    ///
    const source_location call_point;

private:
    //
    std::string BuildWhatString() const
    {
        std::stringstream ss;
        ss << "Exception [" << std::runtime_error::what() << "]. ThrowPoint " << throw_point;

        if (throw_point != call_point)
            ss << ". CallPoint " << call_point;

        ss << '.';
        return ss.str();
    }
    //
    const std::string m_What;
};

#define THROW(type, ...) throw type{current_source_location(), __VA_ARGS__}

} // gdg

#endif /*GDG_COMMON_TOOLS_EXCEPTION_HPP_*/
