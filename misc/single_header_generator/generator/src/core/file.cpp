
#include "logger.h"
#include "file.h"
#include "utils.h"

#include <iterator>

namespace one_header_gen {

/*!
 * \brief file_t constructor
 * \param[in] path Path to file
 */
//file_t::file_t(const fs::path &path) :
//      path(path)
//    , filename(path.string())
//{
//    std::ifstream fileStream(path);
//    std::string line;
//    while (std::getline(fileStream, line))
//    {
//        lines.emplace_back(std::move(line));
//    }

//    LOG("Read ", lines.size(), " lines from file ", filename);
//}

//file_t::file_t(const std::string &str)
//{
//    std::istringstream stream(str);
//    std::string line;
//    while(std::getline(stream, line))
//    {
//        lines.emplace_back(std::move(line));
//    }
//
//    LOG("Read ", lines.size(), " lines from string");
//}

file_t::file_t(const std::string_view path) :
    path(path)
{
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
//void file_t::write(const fs::path &write_path) const
//{
//    std::ofstream fileStream(write_path, std::ofstream::trunc);
//    fileStream << to_string();
//}

/*!
 * \brief Concatenate files
 * \param[in] rhs Right hand side file
 * \return    Result file
 */
const file_t &file_t::operator+=(const file_t &rhs)
{
    lines.insert(lines.end(), rhs.lines.begin(), rhs.lines.end());
    LOG("Add ", rhs.lines.size(), " lines to file ", path, ", now it contain ", lines.size(), " lines");
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

    LOG("Add ", delta, " lines to file ", path, ", now it contain ", lines.size(), " lines");

    return *this;
}

/*!
 * \brief Insert file to position
 * \param[in] position Position for insert
 * \param[in] file     Inserted file
 */
void file_t::insert(const std::size_t position, const file_t &file)
{
    if (position > lines.size()) throw std::invalid_argument("Position could not be more than lines number");
    lines.reserve(file.lines.size());
    lines.insert(lines.begin() + position, file.lines.begin(), file.lines.end());
    LOG("Added ", file.lines.size(), " lines from file ", file.path, " instead #include line");
}

/*!
 * \brief Clear file
 */
//void file_t::clear() {
//    lines.clear();
//    LOG("Clear file ", filename, ", now it contain ", lines.size(), " lines");
//}

std::vector<std::string>& file_t::get_lines()
{
    return lines;
}

//const std::string_view file_t::get_name() const
//{
//    return filename;
//}

const std::string_view file_t::get_path() const
{
    return path;
}

//bool operator==(const one_header_gen::file_t& lhs, const one_header_gen::file_t& rhs)
//{
//    return lhs.lines == rhs.lines;
//}

} // namespace one_header_gen

