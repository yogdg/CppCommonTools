#ifndef GDG_COMMON_TOOLS_DETAILS_TINY_RANGES_INTERNAL_HPP_
#define GDG_COMMON_TOOLS_DETAILS_TINY_RANGES_INTERNAL_HPP_

#include <cstdlib>
#include <gdg/pre_processor.hpp>

namespace gdg::ranges
{
namespace details_gdg_
{
template<typename T>
struct container_view
{
    constexpr container_view(T &&vec, std::size_t offset)
        : vec_{ vec }
        , offset_{ offset }
    { }

    constexpr decltype(auto) begin()
    {
        return vec_.begin() + offset_;
    }

    constexpr decltype(auto) end()
    {
        return vec_.end();
    }

    T &vec_;
    const std::size_t offset_;
};

template<typename T, typename Clause>
struct where_iterator_
{
    where_iterator_(T bgn, T end, Clause clause)
        : it_{ std::move(bgn) }
        , end_{ std::move(end) }
        , clause_{ std::move(clause) }
    {
        calculate_next_(); // bgn may not be the first valid iterator.
    }

    where_iterator_& operator++()
    {
        ++it_;
        calculate_next_();
        return *this;
    }

    bool operator != (const where_iterator_<T, Clause> rhs)
    {
        return it_ != rhs.it_;
    }

    decltype(auto) operator* ()
    {
        return *it_;
    }

private:
    void calculate_next_()
    {
        if (it_ == end_ || clause_(*it_))
            return;

        ++it_;
        calculate_next_();
    }

    T it_;
    T end_;
    Clause clause_;
};

template<typename T, typename SelectT>
struct select_iterator_
{
    select_iterator_(T bgn, T end, SelectT selector)
        : it_{ std::move(bgn) }
        , end_{ std::move(end) }
        , selector_{ std::move(selector) }
    {}

    select_iterator_& operator++()
    {
        ++it_;
        return *this;
    }

    bool operator != (const select_iterator_<T, SelectT> rhs)
    {
        return it_ != rhs.it_;
    }

    decltype(auto) operator* ()
    {
        return selector_(*it_);
    }

private:
    T it_;
    T end_;
    SelectT selector_;
};

template<typename T>
struct from_container_
{
    from_container_(T &elem)
        : elem_{ elem }
    {}

    decltype(auto) begin()
    {
        return elem_.begin();
    }

    decltype(auto) end()
    {
        return elem_.end();
    }

    T &elem_;
};

template<typename T, typename Clause>
struct where_container_
{
    where_container_(T bgn, T endd, Clause clause)
        : bgn_{ std::move(bgn) }
        , end_{ std::move(endd) }
        , clause_{ std::move(clause) }
    {}

    decltype(auto) begin()
    {
        return where_iterator_{ bgn_, end_, clause_ };
    }

    decltype(auto) end()
    {
        return where_iterator_{ end_, end_, clause_ };
    }

    T bgn_;
    T end_;
    Clause clause_;
};

template<typename T, typename SelectT>
struct select_container_
{
    select_container_(T bgn, T endd, SelectT selector)
        : bgn_{ std::move(bgn) }
        , end_{ std::move(endd) }
        , selector_{ std::move(selector) }
    {}

    decltype(auto) begin()
    {
        return select_iterator_{ bgn_, end_, selector_ };
    }

    decltype(auto) end()
    {
        return select_iterator_{ end_, end_, selector_ };
    }

    T bgn_;
    T end_;
    SelectT selector_;
};

template<typename T, typename OnEachT>
struct for_each_container_
{
    for_each_container_(T bgn, T endd, OnEachT on_each)
        : bgn_{ std::move(bgn) }
        , end_{ std::move(endd) }
        , on_each_{ std::move(on_each) }
    {}

    auto begin()
    {
        return bgn_;
    }

    auto end()
    {
        return end_;
    }

    for_each_container_& apply()
    {
        for (; bgn_ != end_; ++bgn_)
            on_each_(*bgn_);

        return *this;
    }

    T bgn_;
    T end_;
    OnEachT on_each_;
};
} // details_gdg_

template<typename T>
struct from
{
    from(T& elem)
        : elem_{ elem }
    {}

    decltype(auto) begin()
    {
        return elem_.begin();
    }

    decltype(auto) end()
    {
        return elem_.end();
    }

    T& elem_;
};

template<typename Clause>
struct where
{
    where(Clause clause)
        : clause_{ std::move(clause) }
    {}

    Clause clause_;
};

template<typename SelectT>
struct select
{
    select(SelectT gen_fx)
        : gen_fx_{ std::move(gen_fx) }
    {}

    SelectT gen_fx_;
};

template<typename OnEachT>
struct for_each
{
    for_each(OnEachT on_each_fx)
        : on_each_fx_{ std::move(on_each_fx) }
    {}

    OnEachT on_each_fx_;
};

} // gdg::ranges

#endif /*GDG_COMMON_TOOLS_DETAILS_TINY_RANGES_INTERNAL_HPP_*/
