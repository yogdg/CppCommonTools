#ifndef GDG_COMMON_TOOLS_META_TOOLS_HPP_
#define GDG_COMMON_TOOLS_META_TOOLS_HPP_

#include <gdg/Details/meta_tools_internal.hpp>

namespace gdg::meta_tools
{
///
///
///
template<typename OnEachT, typename ...Args, typename = std::enable_if_t<sizeof...(Args) >= 1>>
constexpr void for_each(std::tuple<Args...> &tuple, OnEachT &&on_each)
{
    details_::for_each_<sizeof...(Args) - 1>(tuple, FWD(on_each));
}
///
///
///
template<typename OnEachT, typename ...Args>
constexpr void for_each(const std::tuple<Args...> &tuple, OnEachT&& on_each)
{
    details_::for_each_<sizeof...(Args) - 1>(tuple, FWD(on_each));
}


///
///
///
template<typename T>
using remove_rcv_t = std::remove_cv_t<std::remove_reference_t<T>>;


///
///
///
template<typename T>
struct is_unique_ptr : std::false_type
{};
template<typename T, typename D>
struct is_unique_ptr<std::unique_ptr<T, D>> : std::true_type
{};
template<typename T>
inline constexpr auto is_unique_ptr_v = is_unique_ptr<T>::value;


///
///
///
template<typename T>
struct is_optional : std::false_type
{};
template<typename T>
struct is_optional<std::optional<T>> : std::true_type
{};
template<typename T>
inline constexpr auto is_optional_v = is_optional<T>::value;


///
///
///
template<typename T>
struct is_vector : std::false_type
{};
template<typename T>
struct is_vector<std::vector<T>> : std::true_type
{};
template<typename T>
inline constexpr auto is_vector_v = is_vector<T>::value;


///
///
///
template<typename T>
struct is_array : std::false_type
{};
template<typename T, std::size_t N>
struct is_array<std::array<T, N>> : std::true_type
{};
template<typename T>
inline constexpr auto is_array_v = is_array<T>::value;


///
///
///
template<typename... T>
struct is_tuple : std::false_type
{};
template<typename... T>
struct is_tuple<std::tuple<T...>> : std::true_type
{};
template<typename T>
inline constexpr auto is_tuple_v = is_tuple<T>::value;


///
///
///
template<typename T>
struct member_element
{};
template<typename T, typename U>
struct member_element<T U::*>
{
    typedef T type;
};
template<typename T>
using member_element_t = typename member_element<T>::type;

///
///
///
template<typename T>
struct wrapper
{
    constexpr wrapper()
    {}

    typedef T type;
};

///
///
///
template<typename UnnamedType>
constexpr auto is_valid(UnnamedType&&)
{
    return details_::is_valid_container<UnnamedType>();
}

enum class operation
{
    or_
    , and_
    , xor_
    , add
    , sub
    , mul
    , div
    , quo
};

template<typename T, operation Operation>
struct do_operation;

#define DEFINE_DO_OPERATION(operation_enum, oper)\
    template<typename T>\
    struct do_operation<T, operation_enum>\
    {\
        constexpr T operator ()(T l, T r) const\
        {\
            return l oper r;\
        }\
    }

///
///
///
DEFINE_DO_OPERATION(operation::or_, |);

///
///
///
DEFINE_DO_OPERATION(operation::and_, &);

///
///
///
DEFINE_DO_OPERATION(operation::xor_, ^);

///
///
///
DEFINE_DO_OPERATION(operation::add, +);

///
///
///
DEFINE_DO_OPERATION(operation::sub, -);

///
///
///
DEFINE_DO_OPERATION(operation::mul, *);

///
///
///
DEFINE_DO_OPERATION(operation::div, /);

///
///
///
DEFINE_DO_OPERATION(operation::quo, % );


///
///
///
template<typename EnumT, operation Operation, typename IntermediateT = std::size_t>
constexpr EnumT operator_on_enum (EnumT l, EnumT r)
{
    constexpr auto fxy{ do_operation<IntermediateT, Operation>{} };
    return static_cast<EnumT>(fxy(static_cast<IntermediateT>(l), static_cast<IntermediateT>(r)));
}

} // gdg::meta_tools

#endif /*GDG_COMMON_TOOLS_META_TOOLS_HPP_*/
