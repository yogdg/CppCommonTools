#ifndef GDG_COMMON_TOOLS_TINY_PROPETIES_HPP_
#define GDG_COMMON_TOOLS_TINY_PROPETIES_HPP_

#include <string_view>
#include <unordered_map>
#include <filesystem>
#include <fstream>
#include <optional>
#include <sstream>
#include <tuple>
#include <system_error>
#include <execution>

#include <gdg/exception.hpp>
#include <gdg/lexical_cast.hpp>
#include <gdg/string_utils.hpp>

namespace gdg::properties
{

///
///
///
class properties_base_exception : public gdg::basic_exception
{
public:
    properties_base_exception(
        source_location throwpoint
        , std::string message)
        : basic_exception
        {
            MOVE(throwpoint)
            , MOVE(message)
        }
    { }

    virtual ~properties_base_exception() noexcept
    {}
};

///
///
///
class ill_formed_file : public properties_base_exception
{
public:
    ill_formed_file(
        source_location throwpoint
        , const std::filesystem::path &file
        , std::size_t lineno
        , std::string_view line
        , std::string_view message )
        : properties_base_exception
        {
            MOVE(throwpoint)
            , gdg::string_utils::make_string<char>("")
                << message
                << " File ["
                << file
                << "] Line ["
                << line << "] ("
                << lineno
                << ')'
        }
    { }
};

///
///
///
class file_not_found_exception : public properties_base_exception
{
public:
    file_not_found_exception(
        source_location throwpoint
        , const std::filesystem::path &file
        , std::error_code err)
        : properties_base_exception
        {
            MOVE(throwpoint)
            , gdg::string_utils::make_string<char>()
                << "Could't open file ["
                << file
                << "] because of ["
                << err.message()
                << "]"
        }
        , error{ err }
    {}

    const std::error_code error;
};

///
///
///
class key_not_found : public properties_base_exception
{
public:
    key_not_found(
        source_location throwpoint
        , std::string_view key)
        : properties_base_exception
        {
            MOVE(throwpoint)
            , gdg::string_utils::make_string<char>()
                << "Key ["
                << key
                << "] not found"
        }
    { }
};

///
///
///
class properties
{
public:
    ///
    ///
    properties() = default;
    ///
    ///
    properties(const properties&) = default;
    ///
    ///
    properties(properties&&) = default;
    ///
    ///
    properties& operator=(const properties&) = default;
    ///
    ///
    properties& operator=(properties&&) = default;
    ///
    ///
    explicit properties(std::nothrow_t, std::filesystem::path file)
    {
        load(std::nothrow, MOVE(file));
    }
    ///
    ///
    explicit properties(std::filesystem::path file)
    {
        load(MOVE(file));
    }

public:
    ///
    ///
    void load(std::nothrow_t, std::filesystem::path file);
    ///
    ///
    void load(std::filesystem::path file);
    ///
    ///
    void load(std::nothrow_t)
    {
        load(std::nothrow, m_File);
    }
    ///
    ///
    void load()
    {
        load(m_File);
    }
    ///
    ///
    bool save(std::nothrow_t, const std::filesystem::path &file);
    ///
    ///
    void save(const std::filesystem::path &file)
    {
        if (!save(std::nothrow, file))
            THROW(::gdg::properties::file_not_found_exception, m_File, std::make_error_code(static_cast<std::errc>(errno)));
    }
    ///
    ///
    bool save(std::nothrow_t)
    {
        return save(std::nothrow, m_File);
    }
    ///
    ///
    void save()
    {
        save(m_File);
    }

public:
    ///
    ///
    template<typename T>
    std::optional<T> get(std::nothrow_t, const std::string &key) const
    {
        if (const auto it{ m_Data.find(key) }; it != m_Data.end())
            return gdg::lexical_cast<T>(it->second);

        return {};
    }
    ///
    ///
    template<typename T>
    T get(const std::string &key) const
    {
        if (const auto ret{ get<T>(std::nothrow, key) }; ret)
            return *ret;

        THROW(::gdg::properties::key_not_found, key);
    }
    ///
    ///
    template<typename T>
    T get(const std::string &key, T &&defvalue) const
    {
        if (const auto ret{ get<T>(std::nothrow, key) }; ret)
            return *ret;

        return defvalue;
    }
    ///
    ///
    template<typename T>
    void insert_or_assign(std::string key, const T &value)
    {
        m_Data.emplace(std::move(key), gdg::lexical_cast<std::string>(value));
    }
    ///
    ///
    bool erase(std::nothrow_t, const std::string& key)
    {
        return m_Data.erase(key) == 1;
    }
    ///
    ///
    void erase(const std::string& key)
    {
        if (!erase(std::nothrow, key))
            THROW(::gdg::properties::key_not_found, key);
    }
    ///
    ///
    void clear()
    {
        m_Data.clear();
    }

public:
    ///
    ///
    bool empty() const noexcept
    {
        return m_Data.empty();
    }
    ///
    ///
    auto size() const noexcept
    {
        return m_Data.size();
    }

public:
    ///
    ///
    std::string operator [](const std::string &key) const
    {
        return get<std::string>(key, "");
    }
    ///
    ///
    decltype(auto) begin() const noexcept
    {
        return m_Data.begin();
    }
    ///
    ///
    decltype(auto) end() const noexcept
    {
        return m_Data.end();
    }

public:
    ///
    ///
    friend void swap(properties& lhr, properties& rhr)
    {
        std::swap(lhr.m_Data, rhr.m_Data);
        std::swap(lhr.m_File, rhr.m_File);
    }

private:
    std::filesystem::path m_File;
    std::unordered_map<std::string, std::string> m_Data;
};

#include <gdg/details/tiny_properties_internal.hpp>

} // gdg::properties

#endif /*GDG_COMMON_TOOLS_TINY_PROPETIES_HPP_*/
