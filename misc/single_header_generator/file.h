#ifndef _FILE_H_
#define _FILE_H_

#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

namespace one_header_gen
{

/*!
 * \brief Structure for represent source file as array of lines
 */
class file_t {
public:
    file_t() = default;
    file_t(const fs::path &path);
    file_t(const std::string &lines);

    void write(const fs::path &path) const;
    void insert(const std::size_t position, const file_t &file);
    void clear();
    std::string to_string() const;
    std::vector<std::string>& get_lines();
    const std::string& get_name() const;
    const fs::path& get_path() const;

    const file_t& operator+=(const file_t &rhs);
    const file_t& operator+=(const std::string &rhs);
    friend bool operator==(const one_header_gen::file_t& lhs, const one_header_gen::file_t& rhs);

private:
    fs::path    path;
    std::string filename;
    std::vector<std::string> lines; /*!< Lines */
};

using file_ptr_t = std::shared_ptr<file_t>;

bool operator==(const one_header_gen::file_t& lhs, const one_header_gen::file_t& rhs);

} // namespace one_header_gen

#endif // _FILE_H_
