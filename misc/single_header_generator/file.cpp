
#include "file.h"

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
        lines.push_back(std::move(line));
    }

    LOG("Read ", lines.size(), " lines from file ", filename);
}

/*!
 * \brief Convert file to one string
 */
std::string file_t::toString() const
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
    fileStream << toString();
}

/*!
 * \brief   Delete file description in Doxygen format
 * \details 1. Find line with "{SLASH}*!"
 *          2. Find line with "\file"
 *          3. Find line with "*\/"
 *          4. Delete
 */
void file_t::deleteFileDescription()
{
    std::size_t begin = 0, size;
    bool isFileDescription = false;

    size = lines.size();
    for (std::size_t i = 0; i < size; ++i) {
        if (Utils::isBeginOfDoxygenComment(lines[i]))
        {
            begin = i;
        }
        isFileDescription = isFileDescription || Utils::isDoxygenFileDescription(lines[i]);
        if (Utils::isEndOfComment(lines[i]))
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
 * \brief Replace lines with "#include" in begin of file
 */
void file_t::reprlaceInludes() {
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

} // namespace one_header_gen
