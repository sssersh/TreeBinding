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
struct file_t {
    std::string filename;
    std::vector<std::string> lines; /*!< Lines */

    file_t() = default;
    file_t(const fs::path &path);
    file_t(const std::string &lines);

    std::string to_string() const;
    void write(const fs::path &path) const;

    void delete_file_description();
    void move_includes();
    void replace_all_occurancies_in_single_line(const std::string &pattern, const std::string &replacer);

    const file_t& operator+=(const file_t &rhs);
    const file_t& operator+=(const std::string &rhs);
    void insert(const std::size_t position, const file_t &file);

    void clear();
    friend bool operator==(const one_header_gen::file_t& lhs, const one_header_gen::file_t& rhs);
};

bool operator==(const one_header_gen::file_t& lhs, const one_header_gen::file_t& rhs);

} // namespace one_header_gen

#endif // _FILE_H_
