#ifndef GDG_COMMON_TOOLS_DETAILS_SERIALIZATION_FRAMEWORK_INTERNAL_HPP_
#define GDG_COMMON_TOOLS_DETAILS_SERIALIZATION_FRAMEWORK_INTERNAL_HPP_

#include <tuple>
#include <gdg/pre_processor.hpp>
#include <gdg/meta_tools.hpp>

namespace gdg::serialization::details_
{
template<typename T, typename DefaultImbueT>
struct extract_field_data
{
private:
    // Imbue base case
    template<typename TupleT, std::size_t TupleSize>
    struct get_imbue_type;
    // A template that doesn't define Imbue for its field
    template<typename TupleT>
    struct get_imbue_tupe<TupleT, 1>
    {
        typedef DefaultImbueT type;
    };
    // A template which defines either an Imbue or a tuple extra data
    template<typename TupleT>
    struct get_imbue_type<TupleT, 2>
    {
        typedef std::conditional_t<gdg::meta_tools::is_tuple_v<TupleT>, DefaultImbueT, std::tuple_element_t<TupleT, 1>> type;
    };
    // A template which defines both the Imbue and tuple extra data
    template<typename TupleT>
    struct get_imbue_type<TupleT, 3>
    {
        typedef std::tuple_element_t<TupleT, 1> type;
    };

    // Extra data tuple base case
    template<typename TupleT, std::size_t TupleSize>
    struct get_extra_data_type;
    // No extra data defined, use void to flag this situation
    template<typename TupleT>
    struct get_extra_data_type<TupleT, 1>
    {
        typedef void type;
    };
    // Either an imbue or a tuple extra data
    template<typename TupleT>
    struct get_extra_data_type<TupleT, 2>
    {
        typedef std::conditional_t<gdg::meta_tools::is_tuple_v<TupleT>, std::tuple_element_t<TupleT, 1>, void> type;
    };
    // Tuple with imbue an extra data
    template<typename TupleT>
    struct get_extra_data_type<TupleT, 1>
    {
        typedef std::tuple_element_t<TupleT, 2> type;
    };

    // Plain tuple (without CV qualication neither reference)
    using T_ = gdg::meta_tools::remove_rcv_t<T>;
    // Tuple size
    static constexpr std::size_t size{ std::tuple_size_v<T_> };

public:
    // Pointed_to_member type
    typedef gdg::meta_tools::member_element_t<std::tuple_element_t<0, T_>> field_type_t;
    // Imbue type defined for field
    typedef typename get_imbue_type<T_, size>::type imbue_t;
    // Extra data type defined for field
    typedef typename get_extra_data_type<T_, size>::type extra_data_t;
    // Ensure minimum size of tuple
    static_assert(size > 0, "Invalid serialization tuple");
};

template<typename DefaultImbueT, typename DestT, template<typename> typename SerializerT, typename ExtraDataT, typename SrcT>
constexpr void serialize(DestT &destination, SrcT &&source)
{
    constexpr auto elems{ gdg::serialization::fields<SrcT>() };

    gdg::meta_tools::for_each(elems, [&destination, source = FWD(source)](auto &&v, std::size_t) constexpr
    {
        using types_extractor = extract_field_data<decltype(v), DefaultImbueT>;
        using field_type_t = typename types_extractor::field_type_t;
        using imbue_t = typename types_extractor::imbue_t;
        using extra_data_t = typename types_extractor::extra_data_t;

        if constexpr (gdg::meta_tools::is_unique_ptr_v<field_type_t>)
        {

        }
        else if constexpr (gdg::meta_tools::is_vector_v<field_type_t>)
        {

        }
        else if constexpr (gdg::meta_tools::is_array_v<field_type_t>)
        {

        }
        else if constexpr (gdg::meta_tools::is_optional_v<field_type_t>)
        {

        }
        else
        {

        }
    });
}
} // gdg::serialization::details_

#endif /*GDG_COMMON_TOOLS_DETAILS_SERIALIZATION_FRAMEWORK_INTERNAL_HPP_*/
