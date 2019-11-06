#ifndef GDG_COMMON_TOOLS_PREPROCESSOR_HPP_
#define GDG_COMMON_TOOLS_PREPROCESSOR_HPP_

namespace gdg
{
///
///
///
#define CAT(X,Y) PRV_CAT_IMPL_(X,Y)
#define PRV_CAT_IMPL_(X,Y) X##Y


///
///
///
#define __LINE__CONSTEXPR int(CAT(__LINE__,U))


///
///
///
#define FWD(x) ::std::forward<decltype(x)>(x)


///
///
///
#define MOVE(x) ::std::move(x)

} // gdg

#endif /*GDG_COMMON_TOOLS_PREPROCESSOR_HPP_*/
