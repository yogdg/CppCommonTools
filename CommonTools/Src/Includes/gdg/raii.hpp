#ifndef GDG_COMMON_TOOLS_RAII_HPP_
#define GDG_COMMON_TOOLS_RAII_HPP_

#include <cstdlib>
#include <type_traits>
#include <functional>
#include <gdg/pre_processor.hpp>

namespace gdg
{
///
///
///
template<typename R, bool Dismissable>
struct resource
{
    ///
    ///
    resource(std::function<R()> acquirer, std::function<void(R)> releaser)
    {
        resource_ = acquirer();
        dismissed_ = false;
        releaser_ = MOVE(releaser);
    }

    ///
    ///
    ~resource() noexcept
    {
        if (!dismissed_)
            releaser_(resource_);
    }

    ///
    ///
    resource(resource &&r) noexcept
    {
        swap(*this, r);
    }

    ///
    ///
    resource(const resource &) = delete;

    ///
    ///
    resource& operator=(resource &&r) noexcept
    {
        swap(*this, r);
        return *this;
    }

    ///
    ///
    resource& operator=(const resource &) = delete;

    ///
    ///
    template<typename = std::enable_if_t<Dismissable>>
    void dismiss() noexcept
    {
        dismissed_ = true;
    }

    ///
    ///
    operator R& () noexcept
    {
        return resource_;
    }

    ///
    ///
    operator const R& () const noexcept
    {
        return resource_;
    }

    ///
    ///
    R& get() noexcept
    {
        return resource_;
    }

    ///
    ///
    const R& get() const noexcept
    {
        return resource_;
    }

    ///
    ///
    friend void swap(resource<R, Dismissable> &l, resource<R, Dismissable> &r) noexcept
    {
        using std::swap;
        swap(l.resource_, r.resource_);
        swap(l.dismissed_, r.dismissed_);
        swap(l.releaser_, r.releaser_);
    }

private:
    //
    R resource_;
    //
    bool dismissed_{ true };
    //
    std::function<void(R)> releaser_;
};

///
///
///
template<bool Dismissable>
struct resource<void, Dismissable>
{
    ///
    ///
    resource(std::function<void()> acquirer, std::function<void()> releaser)
    {
        acquirer();
        dismissed_ = false;
        releaser_ = MOVE(releaser);
    }

    ///
    ///
    ~resource() noexcept
    {
        if (!dismissed_)
            releaser_();
    }

    ///
    ///
    resource(resource &&r) noexcept
    {
        swap(*this, r);
    }

    ///
    ///
    resource(const resource &) = delete;

    ///
    ///
    resource& operator=(resource &&r) noexcept
    {
        swap(*this, r);
        return *this;
    }

    ///
    ///
    resource& operator=(const resource &) = delete;

    ///
    ///
    template<typename = std::enable_if_t<Dismissable>>
    void dismiss() noexcept
    {
        dismissed_ = true;
    }

    ///
    ///
    friend void swap(resource<void, Dismissable> &l, resource<void, Dismissable> &r) noexcept
    {
        using std::swap;
        swap(l.dismissed_, r.dismissed_);
        swap(l.releaser_, r.releaser_);
    }

private:
    //
    bool dismissed_{ true };
    //
    std::function<void()> releaser_;
};

///
///
///
template<typename O, typename C, typename R = std::invoke_result_t<O>>
auto raii(O &&o, C &&c)
{
    return resource<R, true>{FWD(o), FWD(c)};
}

///
///
///
template<typename O, typename C, typename R = std::invoke_result_t<O>>
auto nd_raii(O &&o, C &&c)
{
    return resource<R, false>{FWD(o), FWD(c)};
}

///
///
///
template<typename C>
auto finally(C &&c)
{
    return nd_raii{ []() {}, FWD(c) };
}

} // gdg

#endif /*GDG_COMMON_TOOLS_RAII_HPP_*/
