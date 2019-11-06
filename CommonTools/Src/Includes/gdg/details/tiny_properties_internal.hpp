#ifndef GDG_COMMON_TOOLS_DETAILS_PROPERTIES_INTERNAL_HPP_
#define GDG_COMMON_TOOLS_DETAILS_PROPERTIES_INTERNAL_HPP_

namespace details_
{
inline std::tuple<bool, std::string_view, std::string_view> parse_line_(std::string_view line)
{
    line = gdg::string_utils::ltrim(line);

    if (line.empty() || line[0] == '!' || line[0] == '#')
        return { true, {}, {} };

    const auto it
    {
        std::find_if(std::execution::seq, line.cbegin(), line.cend(), [](decltype(line)::value_type c)
        {
            return c == '=' || c == ':' || c == ' ';
        })
    };

    if (it == line.cend())
        return { false, {}, {} };

    return
    {
        true
        , {
            line.data()
            , static_cast<size_t>(std::distance(line.cbegin(),it))
        }
        , {
            line.data() + std::distance(line.cbegin(), it) + 1
            , static_cast<size_t>(std::distance(it, line.cend()) - 1)
        }
    };
}

inline std::string do_query_value_(std::ifstream &stream, std::size_t &lineno, std::string_view read_line)
{
    std::stringstream ss;
    std::string line{ read_line };

    do
    {
        read_line = line;

        if (read_line.empty() || *(read_line.rbegin()) != '\\')
        {
            ss << read_line;
            break;
        }
        else
            ss << read_line.substr(0, read_line.size() - 1) << "\r\n";

        ++lineno;
    } while (std::getline(stream, line));

    return ss.str();
}

inline std::string query_value_(std::ifstream &stream, std::size_t &lineno, std::string_view read_line)
{
    if (read_line.empty() || *(read_line.rbegin()) != '\\')
        return std::string{ read_line };

    return do_query_value_(stream, lineno, std::move(read_line));
}

inline std::string query_value_(std::ifstream &stream, std::string_view read_line)
{
    std::size_t dmy{ 0 };
    return query_value_(stream, dmy, std::move(read_line));
}

inline std::string multiline_string_to_file_format_(std::string value)
{
    for (auto pos = value.find_first_of('\r');
        pos != std::string::npos;
        pos = value.find_first_of('\r', pos + 3))
        value.replace(pos, 2, "\\\r\n");

    return value;
}

} // details_

inline void properties::load(std::nothrow_t, std::filesystem::path file)
{
    m_File = MOVE(file);
    clear();

    std::ifstream stream{ m_File };

    for (std::string line; std::getline(stream, line);)
        if (const auto& [valid, key, value] { details_::parse_line_(line) }; valid && !key.empty())
            m_Data.try_emplace(std::string{ key }, details_::query_value_(stream, value));
}

inline void properties::load(std::filesystem::path file)
{
    properties tmp;

    tmp.m_File = MOVE(file);

    std::ifstream stream{ tmp.m_File };

    if (!stream.is_open())
        THROW(::gdg::properties::file_not_found_exception, file, std::make_error_code(static_cast<std::errc>(errno)));

    std::size_t i{ 1 };
    for (std::string line; std::getline(stream, line); ++i)
        if (const auto& [valid, key, value] { details_::parse_line_(line) }; !valid)
            THROW(::gdg::properties::ill_formed_file, tmp.m_File, i, line, "Unparseable line.");
        else if (key.empty())
            continue;
        else if (const auto& [dummy, inserted] { tmp.m_Data.try_emplace(std::string{ key }, details_::query_value_(stream, i, value)) }; !inserted)
            THROW(::gdg::properties::ill_formed_file, tmp.m_File, i, line, "Duplicated key.");

    swap(*this, tmp);
}

bool properties::save(std::nothrow_t, const std::filesystem::path& file)
{
    std::ofstream stream{ file, std::ios::binary };

    if (!stream.is_open())
        return false;

    for (const auto& [k, v] : m_Data)
        stream << k << "=" << details_::multiline_string_to_file_format_(v) << "\r\n";

    return true;
}

#endif /*GDG_COMMON_TOOLS_DETAILS_PROPERTIES_INTERNAL_HPP_*/
