
#include "logger.h"
#include "file.h"
#include "utils.h"

#include <regex>
#include <set>

namespace one_header_gen {

/*!
 * \brief file_t constructor
 * \param[in] path Path to file
 */
file_t::file_t(const fs::path &path) :
    filename(path.string())
{
    std::ifstream fileStream(path);
    std::string line;

    while (std::getline(fileStream, line))
    {
        lines.emplace_back(std::move(line));
    }

    LOG("Read ", lines.size(), " lines from file ", filename);
}

file_t::file_t(const std::string &str)
{
    std::istringstream stream(str);
    std::string line;
    while(std::getline(stream, line))
    {
        lines.emplace_back(std::move(line));
    }

    LOG("Read ", lines.size(), " lines from string");
}

/*!
 * \brief Convert file to one string
 */
std::string file_t::to_string() const
{
    std::string result;
    for (const auto &line: lines)
    {
        result += line + "\n";
    }
    return result;
}

/*!
 * \brief Write file to path
 * \param[in] path Path to file
 */
void file_t::write(const fs::path &path) const
{
    std::ofstream fileStream(path, std::ofstream::trunc);
    fileStream << to_string();
}

/*!
 * \brief   Delete file description in Doxygen format
 * \details 1. Find line with "{SLASH}*!"
 *          2. Find line with "\file"
 *          3. Find line with "*\/"
 *          4. Delete
 */
void file_t::delete_file_description()
{
    std::size_t begin = 0, size;
    bool isFileDescription = false;

    size = lines.size();
    for (std::size_t i = 0; i < size; ++i) {
        if (utils::is_begin_of_doxygen_comment(lines[i]))
        {
            begin = i;
        }
        isFileDescription = isFileDescription || utils::is_doxygen_description(lines[i]);
        if (utils::is_end_of_comment(lines[i]))
        {
            if (isFileDescription)
            {
                lines.erase(lines.cbegin() + begin, lines.cbegin() + i + 1);
                size -= i - begin + 1;
                isFileDescription = false;
            }
        }
    }
}

/*!
 * \brief Move lines with "#include" to begin of file
 */
void file_t::move_includes() {
    const auto r = std::regex(R"((#include[ \t]*[<][a-zA-Z0-9\._/]*[>]).*)");
    size_t size = lines.size();

    std::set<std::string> includes;
    std::smatch match;

    for (size_t i = 0; i < size; ++i) {
        if (std::regex_match(lines[i], match, r)) {
            includes.insert(match[1]);
            lines[i].erase();
            --i;
        }
    }

    lines.insert(lines.begin(), includes.begin(), includes.end());
}

void file_t::replace_all_occurancies(const std::string &pattern, const std::string &replacer)
{
    const auto r = std::regex ( pattern  );
    std::smatch match;

    for(auto &line : lines)
    {
        if(std::regex_match(line, match, r)) {
            std::ostringstream ostream;
            std::regex_replace(
                    std::ostreambuf_iterator<char>(ostream),
                    line.begin(),
                    line.end(),
                    r,
                    replacer);
            line = ostream.str();
        }
    }
}

/*!
 * \brief Concatenate files
 * \param[in] rhs Right hand side file
 * \return    Result file
 */
const file_t &file_t::operator+=(const file_t &rhs) {
    lines.insert(lines.end(), rhs.lines.begin(), rhs.lines.end());
    LOG("Add ", rhs.lines.size(), " lines to file ", filename, ", now it contain ", lines.size(), " lines");
    return *this;
}

/*!
 * \brief Add string to end of file
 * \param[in] rhs Added string
 * \return    Result file
 */
const file_t &file_t::operator+=(const std::string &rhs) {
    std::stringstream stream(rhs);
    std::string line;

    size_t delta = 0;

    while (std::getline(stream, line)) {
        ++delta;
        lines.push_back(line);
    }

    LOG("Add ", delta, " lines to file ", filename, ", now it contain ", lines.size(), " lines");

    return *this;
}

/*!
 * \brief Insert file to position
 * \param[in] position Position for insert
 * \param[in] file     Inserted file
 */
void file_t::insert(const std::size_t position, const file_t &file) {
    lines.insert(lines.begin() + position, file.lines.begin(), file.lines.end());
    LOG("Added ", file.lines.size(), " lines from file ", file.filename, " instead #include line");
}

/*!
 * \brief Clear file
 */
void file_t::clear() {
    lines.clear();
    LOG("Clear file ", filename, ", now it contain ", lines.size(), " lines");
}

bool operator==(const one_header_gen::file_t& lhs, const one_header_gen::file_t& rhs)
{
    return lhs.lines == rhs.lines;
}

} // namespace one_header_gen

