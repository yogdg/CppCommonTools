#ifndef GDG_COMMON_TOOLS_SERIALIZATION_FRAMEWORK_HPP_
#define GDG_COMMON_TOOLS_SERIALIZATION_FRAMEWORK_HPP_

namespace gdg::serialization
{
///
/// Base fields definition. Each specialization of this struct must implemement
/// a static constexpr std::tuple<...> members () method. The returned std::tuple
/// defines the fields to (de)serialize the/in type T.
/// The structure of the tuple is:
/// std::tuple<
///     std::tuple<pointer_to_member 1 [, std::true/false _type] [, imbue_type] [, std::tuple<extra data particular to (de)serialization method>]> [,
///     ...,
///     std::tuple<pointer_to_member N [, std::true/false _type] [, imbue_type] [, std::tuple<extra data particular to (de)serialization method>]> ]
/// >
/// pointer_to_member: pointer to member (U::*A).
/// [std::true/false _type]: optional, indicates whether the field is a compound type (std::true_type), or not (std::false_type). If not specified
/// std::false_type is assumed. Note: a compound type is basically a struct not wrapped in a unique_ptr.
/// [imbue_type]: optional, Imbue type to use in case a string to type is required. If not specified, the Imbue used in the call to (de)serialize
/// is used.
/// [std::tuple<extra data particular to (de)serialization method>]: optional, extra tuple to be sent to the (Des/S)erializerT
/// 
template<typename T>
struct fields {};

namespace details_
{

///
///
///
template<typename DefaultImbueT, typename DestT, typename SerializerT, typename SrcT>
constexpr void serialize(DestT &destination, SrcT &&source);


///
///
///
template<typename DestT, typename DefaultImbueT, typename DeserializerT, typename SrcT>
constexpr DestT deserialize(SrcT &&source);

} // details_

} // gdg::serialization

#include <gdg/Details/serialization_framework_internal.hpp>

#endif /*GDG_COMMON_TOOLS_SERIALIZATION_FRAMEWORK_HPP_*/
